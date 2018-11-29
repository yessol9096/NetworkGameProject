/*
MapleStory - Server
*/
#include <winsock2.h>
#include <iostream>
#include <vector>
#include "Server.h"
#include "Include.h"
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

// monster 정보 보냈나 안보냈나 확인
bool bCreateMonster_check = false;
vector<MONSTERINFO> g_vecgreen;
int greenposX[] = {  HENESISCX * 0.5f,HENESISCX * 0.7f , HENESISCX * 0.6f ,HENESISCX * 0.7f,HENESISCX * 0.5f, HENESISCX * 0.4f };
int greenposY = HENESISCY - 460.f;
OBJECT_DIR greenDir[] = { DIR_LEFT, DIR_RIGHT ,DIR_RIGHT, DIR_LEFT, DIR_RIGHT, DIR_RIGHT };
int greenPtr[] = {1,1,3,1,1,3};

// 클라이언트
vector<PLAYERINFO> g_vecplayer;
bool g_arrayconnected[MAX_USER]; // connected 배열 (id 부여 위함)

DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval{ 0 };
	SOCKADDR_IN clientaddr;
	int addrlen;
	int datatype;
	MONSTERINFO monsterinfo;
	PLAYERINFO playerinfo;
	PACKETINFO packetinfo;

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	// -------------------------------------------
	// 구조 바꾸기. 고정 길이 + 가변 길이.
	while (true) {
		char buf[BUFSIZE];

		// 고정 길이 수신.
		ZeroMemory(&packetinfo, sizeof(packetinfo));	 /// 재사용 할 거니까. 계속 비워줌.		
		cout << "고정 길이 수신 대기 중" << endl;
		/// recvn으로 계속 받아와야 함. tcp 특성 때문에..!
		retval = recvn(client_sock, buf, BUFSIZE, 0);
		cout << "고정 길이 수신됨" << endl;
		/// 받기 실패 시,
		if (retval == SOCKET_ERROR) {
			err_display("packetinfo recv()");
			break;
		}
		/// 받기 성공 시,
		else {
			/// 고정 길이 데이터에 recv 해온 메모리만큼 복사 한다.
			memcpy(&packetinfo, buf, sizeof(packetinfo));
		}


		// 가변 길이 수신.
		switch (packetinfo.type) {
		case CS_PACKET_PLAYERINFO_INITIALLY: // 초기 플레이어 info.
		{
			char buf2[BUFSIZE];

			// -----------------------------------------
			// 클라이언트가 초기 설정 한 playerinfo를 recvn 한다.
			{
				ZeroMemory(&buf2, sizeof(buf2));	 /// 재사용 할 거니까. 계속 비워줌.		
				retval = recvn(client_sock, buf2, BUFSIZE, 0);
				cout << "가변 길이 수신 - CS_PACKET_PLAYERINFO_INITIALLY" << endl;
				if (retval == SOCKET_ERROR) {
					err_display("intial playerinfo recv()");
					break;
				}
				else {
					/// 고정 길이 데이터에 recv 해온 메모리만큼 복사 한다.
					memcpy(&playerinfo, buf2, sizeof(playerinfo));
				}

#ifdef DEBUGGING
				cout << "[TCP" << inet_ntoa(clientaddr.sin_addr) << " : " << ntohs(clientaddr.sin_port) << "]";
				if (playerinfo.job == JOB_CAPTIN)
					cout << "[RECV]" << "PlayerInfo - 닉네임 : " << playerinfo.nickname << ", 직업 : 캡틴" << endl;
				else
					cout << "[RECV]" << "PlayerInfo - 닉네임 : " << playerinfo.nickname << ", 직업 : 스트라이커" << endl;
#endif
				// 초기 설정 한 playerinfo를 기반으로 vector에 push 한다.
				int id = -1;
				for (int i = 0; i < MAX_USER; ++i) {
					if (false == g_arrayconnected[i]) { /// 순차적으로 접근해서 연결 되지 않은 인덱스 i를 찾고, 
						id = i; /// 아이디를 부여한다.
						break;
					}


					/// 유저 2인 제한.
					if (-1 == id) {
						std::cout << "user가 다 찼습니다." << std::endl;
						closesocket(client_sock);
						break;
					}
					// 실제 playerinfo에 id를 부여한다.
					playerinfo.id = id;
					playerinfo.connected = true;
					playerinfo.pt.x = 100.f; // 초기 좌표는 (100, 500)
					playerinfo.pt.y = 500.f;
					playerinfo.hp = 30000;
					playerinfo.size.cx = 100.f;
					playerinfo.size.cy = 100.f;
				}
				// 정보가 다 채워진 playerinfo를 g_vecplayer에 담는다.
				g_vecplayer.push_back(playerinfo);
			}
			// -------------------------------------------
			// id 를 전송한다.
			/// 고정 길이 데이터 전송,,
			ZeroMemory(&packetinfo, sizeof(packetinfo)); /// packetinfo 재사용.
			packetinfo.type = SC_PACKET_ID_INITIALLY;
			packetinfo.size = sizeof(playerinfo.id);
			ZeroMemory(buf, sizeof(buf)); /// 버퍼 재사용.
			memcpy(buf, &packetinfo, sizeof(packetinfo));
			retval == send(client_sock, buf, BUFSIZE, 0);
			cout << "고정 길이 전송 - SC_PACKET_PLAYERINFO_ID" << endl;
			if (retval == SOCKET_ERROR) {
				err_display("send() - SC_PACKET_PLAYERINFO_ID");
			}
			/// 가변 길이 데이터 전송,,
			ZeroMemory(buf, sizeof(buf));
			memcpy(buf, &(playerinfo.id), sizeof(playerinfo.id));
			retval == send(client_sock, buf, BUFSIZE, 0);
			cout << "가변 길이 전송 - SC_PACKET_PLAYERINFO_ID" << endl;
			if (retval == SOCKET_ERROR)
				err_display("send() - SC_PACKET_PLAYERINFO_ID");
			// -------------------------------------------
			// 새로 접속한 클라이언트에게, 나를 포함한 모든 플레이어의 위치를 동기화한다.
			// me, other playerinfo 전송.
			else {
				for (int i = 0; i < g_vecplayer.size(); ++i) {
					/// 고정 길이 데이터 전송,,
					ZeroMemory(&packetinfo, sizeof(packetinfo)); /// packetinfo 재사용.
					packetinfo.type = SC_PACKET_PLAYERINFO;
					packetinfo.size = sizeof(g_vecplayer[i]);
					packetinfo.id = i;
					ZeroMemory(buf, sizeof(buf)); /// 버퍼 재사용.
					memcpy(buf, &packetinfo, sizeof(packetinfo));
					retval == send(client_sock, buf, BUFSIZE, 0);
					cout << "고정 길이 전송 - SC_PACKET_PLAYERINFO - " << i << "번째 클라이언트" << endl;
					if (retval == SOCKET_ERROR) {
						err_display("send() - SC_PACKET_PLAYERINFO");
					}
					/// 가변 길이 데이터 전송,,
					ZeroMemory(buf, sizeof(buf)); /// 버퍼 재사용.
					memcpy(buf, &(g_vecplayer[i]), sizeof(g_vecplayer[i]));
					retval == send(client_sock, buf, BUFSIZE, 0);
					cout << "가변 길이 전송 - SC_PACKET_PLAYERINFO - " << i << "번째 클라이언트" << endl;
					if (retval == SOCKET_ERROR) {
						err_display("send() - SC_PACKET_PLAYERINFO");
					}
				}
			}

		}
		break;
		}

	}

	// -------------------------------------------
	// 몬스터
	//bCreateMonster_check = true;
	//if (bCreateMonster_check == true)
	//{
	//	// 초록버섯 데이터 정의
	//	for (int i = 0; i < MAX_GREEN - 1; ++i)
	//	{
	//		monsterinfo.id = i;
	//		monsterinfo.hp = 100;
	//		monsterinfo.key = OBJ_GRRENMUSH;
	//		monsterinfo.money = 10;
	//		monsterinfo.pt.x = greenposX[i];
	//		monsterinfo.pt.y = greenposY;
	//		monsterinfo.dir = greenDir[i];
	//		monsterinfo.pattern = greenPtr[i];

	//		g_vecgreen.push_back(monsterinfo);
	//		datatype = OBJ_GRRENMUSH;
	//		cout << "몬스터 입력 정보 전달" << endl;

	//		retval = send(client_sock, (char*)&datatype, sizeof(int), 0);
	//		retval = send(client_sock, (char*)&monsterinfo, sizeof(monsterinfo), 0);

	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//			return 1;
	//		}
	//	}
	//	bCreateMonster_check = false;
	//}





	
	// closesocket()
	closesocket(client_sock);
	cout << "[클라이언트 종료]" << endl;
	cout << "IP 주소 (" << inet_ntoa(clientaddr.sin_addr) << "), 포트 번호 ("
		<< ntohs(clientaddr.sin_port) << ")" << endl;

	return 0;
}
DWORD WINAPI CalculateThread()
{
	return 0;
}

int main()
{
	// 공간 2개 예약.
	g_vecplayer.reserve(MAX_USER);
	g_vecgreen.reserve(MAX_GREEN);
	int retval;

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
	HANDLE hThread;

	while (true) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		cout << "[클라이언트 접속]" << endl;
		cout << "IP 주소 (" << inet_ntoa(clientaddr.sin_addr) << "), 포트 번호 ("
			<< ntohs(clientaddr.sin_port) << ") " << endl;

		// 스레드 생성
		hThread = CreateThread(
			NULL, 0, ClientThread,
			(LPVOID)client_sock, 0, NULL);

		if (hThread == NULL)
			closesocket(client_sock);
		else
			CloseHandle(hThread);
	}

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

void DisconnectClient()
{
}

void SendPacket()
{
}

void ProcessPacket()
{
}

void WorkerThread()
{
}

void AcceptThread()
{
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

void MakeMonster()
{

}

