#include <winsock2.h>
#include <iostream>
#include <vector>
#include "Server.h"
#include "Include.h"
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

// 스레드 동기화
CRITICAL_SECTION cs;

// 클라이언트
vector<PLAYERINFO> g_vecplayer;
bool g_arrayconnected[MAX_USER]; // connected 배열 (id 부여 위함)
vector<SOCKET> g_vecsocket;

// monster 정보 보냈나 안보냈나 확인
bool bCreateMonster_check = false;
vector<MONSTERINFO> g_vecgreen(MAX_GREEN);
int greenposX[] = { HENESISCX * 0.5f,HENESISCX * 0.7f , HENESISCX * 0.6f ,HENESISCX * 0.7f,HENESISCX * 0.5f, HENESISCX * 0.4f };
int greenposY = HENESISCY - 460.f;
OBJECT_DIR greenDir[] = { DIR_LEFT, DIR_RIGHT ,DIR_RIGHT, DIR_LEFT, DIR_RIGHT, DIR_RIGHT };
int greenPtr[] = { 1,1,3,1,1,3 };

DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;

	// 클라이언트 정보 얻기
	int addrlen = sizeof(clientaddr);
	EnterCriticalSection(&cs);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);
	LeaveCriticalSection(&cs);

	int retval{ 0 };

	PACKETINFO packetinfo;
	PLAYERINFO playerinfo;


	
	// -------------------------------------------
	// 구조 : 고정 길이 + 가변 길이.
	while (true) {
		char buf[BUFSIZE];

		// 고정 길이.
		// 앞으로 받을 패킷 정보를 알아낸다.
		{
			ZeroMemory(&packetinfo, sizeof(packetinfo));
			EnterCriticalSection(&cs);
			retval = recvn(client_sock, buf, BUFSIZE, 0);
			LeaveCriticalSection(&cs);
			if (retval == SOCKET_ERROR) {
				err_display("packetinfo recv()");
				break;
			}
			else
				memcpy(&packetinfo, buf, sizeof(packetinfo));
		}

		// 가변 길이.
		switch (packetinfo.type) {
		case CS_PACKET_PLAYERINFO_INITIALLY:
		{
			// 클라이언트가 초기 설정 한 playerinfo를 recvn 한다.
			// 1. 초기 설정된 playeinfo를 수신한다.
			// 2. playerinfo의 나머지 멤버 변수들을 채워준다. (id, size, hp, mp, ..)
			// 3. 나머지 멤버 변수들이 채워진 playerinfo를 벡터에 push 한다.
			// 4. 클라이언트에게 "니 정보는 이거야!" 라고 send 하여 알린다.
			// 5. 나머지 다른 클라이언트에게 나의 playerinfo를 전송하여 나의 접속을 알린다. (2가 접속했다면 1에게, 1이 접속했다면 아무에게도 X)

			// --------------------Process---------------------
			// 1. 초기 설정된 playeinfo를 수신한다.
			{
				ZeroMemory(&buf, sizeof(buf));	 /// 재사용 할 거니까. 계속 비워줌.		
				EnterCriticalSection(&cs);
				retval = recvn(client_sock, buf, BUFSIZE, 0);
				LeaveCriticalSection(&cs);
				std::cout << "가변 길이 수신 - CS_PACKET_PLAYERINFO_INITIALLY" << endl;
				if (retval == SOCKET_ERROR) {
					err_display("intial playerinfo recv()");
					break;
				}
				else {
					/// 고정 길이 데이터에 recv 해온 메모리만큼 복사 한다.
					ZeroMemory(&playerinfo, sizeof(playerinfo));
					memcpy(&playerinfo, buf, sizeof(playerinfo));
				}
			}

			// 2. playerinfo의 나머지 멤버 변수들을 채워준다. (id, size, hp, mp, ..)
			{
				int id = -1;
				for (int i = 0; i < MAX_USER; ++i) {
					if (false == g_arrayconnected[i]) { /// 순차적으로 접근해서 연결 되지 않은 인덱스 i를 찾고, 
						g_arrayconnected[i] = true;
						id = i; /// 아이디를 부여한다.
						break;
					}
				}

				/// 유저 2인 제한.
				if (-1 == id) {
					std::cout << "user가 다 찼습니다." << std::endl;
					EnterCriticalSection(&cs);
					closesocket(client_sock);
					LeaveCriticalSection(&cs);
					break;
				}

				// 실제 playerinfo에 얻은 id를 부여한다.
				playerinfo.id = id;
				playerinfo.connected = true;
				playerinfo.ready = false;
				playerinfo.pt.x = 100.f; // 초기 좌표는 (100, 500)
				playerinfo.pt.y = 500.f;
				playerinfo.hp = 30000;
				playerinfo.size.cx = 100.f;
				playerinfo.size.cy = 100.f;
				playerinfo.attackAccValue = 0.f;
				playerinfo.money = 0;
			}

			// 3. 나머지 멤버 변수들이 채워진 playerinfo를 벡터에 push 한다.
			g_vecplayer.push_back(playerinfo);

			// 4. 클라이언트에게 "니 정보는 이거야!" 라고 send 하여 알린다.
			{
				// 고정 길이. 
				ZeroMemory(&packetinfo, sizeof(packetinfo));
				packetinfo.id = playerinfo.id;
				packetinfo.size = sizeof(playerinfo);
				packetinfo.type = SC_PACKET_YOUR_PLAYERINFO;
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &packetinfo, sizeof(packetinfo));
				EnterCriticalSection(&cs);
				retval = send(client_sock, buf, BUFSIZE, 0);
				LeaveCriticalSection(&cs);
				if (retval == SOCKET_ERROR)
					err_display("send() - 고정 길이 - SC_PACKET_YOUR_PLAYERINFO");

				// 가변 길이. 
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &(playerinfo), sizeof(playerinfo));
				EnterCriticalSection(&cs);
				retval = send(client_sock, buf, BUFSIZE, 0);
				LeaveCriticalSection(&cs);
				if (retval == SOCKET_ERROR)
					err_display("send() - 가변 길이 - SC_PACKET_YOUR_PLAYERINFO");
			}

			// 5. 나머지 클라이언트에게 나의 playerinfo를 전송하여 나의 접속을 알린다. (2가 접속했다면 1에게, 1이 접속했다면 아무에게도 X)
			{
				for (int i = 0; i < g_vecplayer.size(); ++i) {
					/// 나에게는 보낼 필요 없어.
					if (i == playerinfo.id)
						continue;
					/// 고정 길이.
					ZeroMemory(&packetinfo, sizeof(packetinfo));
					packetinfo.type = SC_PACKET_OTHER_PLAYERINFO;
					packetinfo.size = sizeof(playerinfo);
					packetinfo.id = playerinfo.id; /// playerinfo.id가 뉴비의 아이디임.
					ZeroMemory(buf, sizeof(buf));
					memcpy(buf, &packetinfo, sizeof(packetinfo));
					EnterCriticalSection(&cs);
					retval = send(g_vecsocket[i], buf, BUFSIZE, 0);	// 다른 클라이언트한테 보내는 거니까, g_vecsocket에서 접근해야겠지.
					LeaveCriticalSection(&cs);
					if (retval == SOCKET_ERROR) {
						err_display("send() - SC_PACKET_NEW_PLAYERINFO");
					}
					/// 가변 길이.
					ZeroMemory(buf, sizeof(buf)); /// 버퍼 재사용.
					memcpy(buf, &playerinfo, sizeof(g_vecplayer[i]));
					EnterCriticalSection(&cs);
					retval = send(g_vecsocket[i], buf, BUFSIZE, 0);
					LeaveCriticalSection(&cs);
					cout << "가변 길이 전송 - SC_PACKET_PLAYERINFO - " << i << "번째 클라이언트" << endl;
					if (retval == SOCKET_ERROR) {
						err_display("send() - SC_PACKET_PLAYERINFO");
					}
				}
			}

		}
		break;
		case CS_PACKET_PLAYERINFO_MOVE:
		{
			// 플레이어가 움직였을 때 패킷을 보내어 들어오는 부분.
			// 2. 가변 길이 패킷을 받아 playerinfo을 받는다.
			// 3. g_vecplayer[받은 playerinfo의 id] 에 접근하여 정보를 갱신한다.
			// 4. 다른 클라이언트에게도 보낸다. (SC_PACKET_OTHER_PLAYERINFO)

			// -------------------Process---------------------
			// 1. 고정 길이 패킷에서, 어느 클라에 해당되는지 id 알아옴.
			int id = packetinfo.id;
			// 2. 가변 길이 패킷을 받아 playerinfo를 받는다.
			PLAYERINFO tempplayerinfo;
			{
				ZeroMemory(&tempplayerinfo, sizeof(tempplayerinfo));
				ZeroMemory(buf, sizeof(buf));
				EnterCriticalSection(&cs);
				retval = recvn(client_sock, buf, BUFSIZE, 0);
				LeaveCriticalSection(&cs);
				if (retval == SOCKET_ERROR) {
					cout << "failed : recvn - CS_PACKET_PLAYERINFO_MOVE" << endl;
					break;
				}
				else
					memcpy(&tempplayerinfo, buf, sizeof(tempplayerinfo));
			}

			// 3. g_vecplayer[받은 playerinfo의 id]에 접근하여 정보를 갱신한다.
			{
				g_vecplayer[id] = tempplayerinfo;
			}

			// 4. 다른 클라이언트에게도 보낸다. (SC_PACKET_OTHER_PLAYERINFO)
			if (g_vecplayer.size() == 2) {// 2인 접속 시에만.
				// 고정 길이. 
				ZeroMemory(&packetinfo, sizeof(packetinfo));
				packetinfo.id = id;
				packetinfo.size = sizeof(PLAYERINFO);
				packetinfo.type = SC_PACKET_OTHER_PLAYERINFO;
				memcpy(buf, &packetinfo, sizeof(packetinfo));
				if (0 == id) {	// 0 정보는 1에게 보내야 한다.
					EnterCriticalSection(&cs);
					retval = send(g_vecsocket[id + 1], buf, BUFSIZE, 0);
					LeaveCriticalSection(&cs);
				}
				else if (1 == id) {
					EnterCriticalSection(&cs);
					retval = send(g_vecsocket[id - 1], buf, BUFSIZE, 0);
					LeaveCriticalSection(&cs);
				}
				if (retval == SOCKET_ERROR) {
					cout << "failed : send - 고정 - SC_PACKET_OTHER_PLAYERINFO" << endl;
					break;
				}

				// 가변 길이
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &(g_vecplayer[id]), sizeof(playerinfo));
				if (0 == id) {	// 0 정보는 1에게 보내야 한다.
					EnterCriticalSection(&cs);
					retval = send(g_vecsocket[id + 1], buf, BUFSIZE, 0);
					LeaveCriticalSection(&cs);
				}
				else if (1 == id) {
					EnterCriticalSection(&cs);
					retval = send(g_vecsocket[id - 1], buf, BUFSIZE, 0);
					LeaveCriticalSection(&cs);
				}
				if (retval == SOCKET_ERROR) {
					cout << "failed : send - 가변 - SC_PACKET_OTHER_PLAYERINFO" << endl;
					break;
				}
			}
		}
		break;
		case SC_PACKET_CLIENT_END:
		{
			// closesocket()
			EnterCriticalSection(&cs);
			closesocket(client_sock);
			LeaveCriticalSection(&cs);
			cout << "[클라이언트 정상 종료] IP 주소 (" << inet_ntoa(clientaddr.sin_addr) <<
				"), 포트 번호 (" << ntohs(clientaddr.sin_port) << ")" << endl;

			return 0;
		}
		break;
		}
	}

	
	closesocket(client_sock);
	cout << "[클라이언트 강제 종료] IP 주소 (" << inet_ntoa(clientaddr.sin_addr) <<
		"), 포트 번호 (" << ntohs(clientaddr.sin_port) << ")" << endl;

	return 0;
}

DWORD WINAPI MonsterThread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	char buf[BUFSIZE];
	bool monsterthread_start = false;
	// 클라이언트 정보 얻기
	int addrlen = sizeof(clientaddr);
	EnterCriticalSection(&cs);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);
	LeaveCriticalSection(&cs);

	int retval{ 0 };

	PACKETINFO packetinfo{};
	MONSTERINFO monsterinfo{};
	while (1)
	{
		for (int i = 0; i < MAX_USER; ++i) {
			if (g_arrayconnected[i] == true)
				monsterthread_start = true;
		}
		if (monsterthread_start == true)
		{
			for (int i = 0; i < MAX_GREEN ; ++i)
			{
				monsterinfo.id = i;
				monsterinfo.hp = 100;
				monsterinfo.money = 10;
				monsterinfo.pt.x = greenposX[i];
				monsterinfo.pt.y = greenposY;
				monsterinfo.dir = greenDir[i];
				monsterinfo.pattern = greenPtr[i];

				g_vecgreen[i] = monsterinfo;

				ZeroMemory(&packetinfo, sizeof(packetinfo));
				packetinfo.id = i;
				packetinfo.size = sizeof(MONSTERINFO);
				packetinfo.type = SC_PACKET_GRRENMUSH_INITIALLY;

				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &packetinfo, sizeof(packetinfo));
				EnterCriticalSection(&cs);
				retval = send(client_sock, buf, BUFSIZE, 0);
				LeaveCriticalSection(&cs);
				if (retval == SOCKET_ERROR) {
					err_display("send() - SC_PACKET_GRRENMUSH_INITIALLY");
				}
				EnterCriticalSection(&cs);
				retval = send(client_sock, (char*)&monsterinfo, sizeof(MONSTERINFO), 0);
				LeaveCriticalSection(&cs);

				if (retval == SOCKET_ERROR) {
					err_display("send()");
				}
			}
		}
	}
	return 0;
}

int main()
{
	// 공간 2개 예약.
	g_vecplayer.reserve(MAX_USER);

	int retval;

	// 임계 영역 초기화
	InitializeCriticalSection(&cs);

	// 윈속 초기화.
	InitializeNetwork();

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	// 주소 구조체 생성
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	// bind()
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread[2];

	while (true) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 socket을 배열에 담는다. (1201)
		g_vecsocket.push_back(client_sock);

		// 접속한 클라이언트 정보 출력
		cout << "[클라이언트 접속] IP 주소 (" << inet_ntoa(clientaddr.sin_addr) <<
			"), 포트 번호 (" << ntohs(clientaddr.sin_port) << ")" << endl;

		// 클라이언트 스레드 생성
		hThread[0] = CreateThread(NULL, 0, ClientThread, (LPVOID)client_sock, 0, NULL);
		if (hThread[0] == NULL)	closesocket(client_sock);
		else					CloseHandle(hThread[0]);

		//몬스터 스레드 생성
		hThread[1] = CreateThread(NULL, 0, MonsterThread, (LPVOID)client_sock, 0, NULL);
	}
	// 두 개의 스레드 종료 대기
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	// 임계 영역 삭제
	DeleteCriticalSection(&cs);

	// closesocket()
	closesocket(listen_sock);

	EndNetwork();

	return 0;
}

void InitializeNetwork()
{
	// 윈속 초기화.
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		err_quit("WSAStartup()");
}

void EndNetwork()
{
	// 윈속 종료
	WSACleanup();
}

int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);

		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;

		else if (received == 0)
			break;

		left -= received;
		ptr += received;
	}
	return len - left;
}


//DWORD WINAPI SendThread(LPVOID arg)
//{
//	return 0;
//
//	//SOCKET client_sock = (SOCKET)arg;
//	//PACKETINFO packetinfo;
//	//char buf[BUFSIZE];
//	//int retval{ 0 };
//
//	//while (true) {
//	//	for (int i = 0; i < g_vecplayer.size(); ++i) {
//	//		/// 고정 길이 데이터 전송,,
//	//		ZeroMemory(&packetinfo, sizeof(packetinfo)); /// packetinfo 재사용.
//	//		packetinfo.type = SC_PACKET_PLAYERINFO;
//	//		packetinfo.size = sizeof(g_vecplayer[i]);
//	//		packetinfo.id = i;
//	//		ZeroMemory(buf, sizeof(buf)); /// 버퍼 재사용.
//	//		memcpy(buf, &packetinfo, sizeof(packetinfo));
//	//		retval == send(client_sock, buf, BUFSIZE, 0);
//	//		cout << "고정 길이 전송 - SC_PACKET_PLAYERINFO - " << i << "번째 클라이언트" << endl;
//	//		if (retval == SOCKET_ERROR) {
//	//			err_display("send() - SC_PACKET_PLAYERINFO");
//	//		}
//
//	//		/// 가변 길이 데이터 전송,,
//	//		ZeroMemory(buf, sizeof(buf)); /// 버퍼 재사용.
//	//		memcpy(buf, &(g_vecplayer[i]), sizeof(g_vecplayer[i]));
//	//		retval == send(client_sock, buf, BUFSIZE, 0);
//	//		cout << "가변 길이 전송 - SC_PACKET_PLAYERINFO - " << i << "번째 클라이언트" << endl;
//	//		if (retval == SOCKET_ERROR) {
//	//			err_display("send() - SC_PACKET_PLAYERINFO");
//	//		}
//	//	}
//	//}
//}