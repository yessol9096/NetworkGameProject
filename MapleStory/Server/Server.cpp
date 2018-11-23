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

// 클라이언트
vector<PLAYERINFO> g_vecplayer;
bool g_arrayconnected[MAX_USER]; // connected 배열 (id 부여 위함)

DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval{ 0 };
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	PLAYERINFO playerinfo;
	MONSTERINFO monsterinfo;
	ZeroMemory(&playerinfo, sizeof(playerinfo));

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	// -------------------------------------------
	// 몬스터
	bCreateMonster_check = true;
	if (bCreateMonster_check == true)
	{
		// 초록버섯 데이터 보내기
		monsterinfo.hp = 100;
		monsterinfo.key = OBJ_GRRENMUSH;
		monsterinfo.money = 10;
		monsterinfo.pt.x = HENESISCX * 0.5f;
		monsterinfo.pt.y = HENESISCY - 460.f;

		g_vecgreen.push_back(monsterinfo);

		cout << "몬스터 입력 정보 전달" << endl;
		retval = send(client_sock, (char*)g_vecgreen[0].key, sizeof(int), 0);
		retval = send(client_sock, (char*)&g_vecgreen , sizeof(g_vecgreen), 0);
		bCreateMonster_check = false;

		if (retval == SOCKET_ERROR) {
			err_display("send()");
			return 1;
		}
	}

	// -------------------------------------------
	// 플레이어가 accept 해 올 때 처리하는 while 문.
	/// 1. id 부여
	/// 2. 클라이언트가 send 해온 PlayerInfo 정보를 recv하고, g_vecplayer에 갱신
	//while (true) {

	//	// g_vecplayer 빈 공간 찾아서 id 부여.
	//	int id = -1;
	//	for (int i = 0; i < MAX_USER; ++i) {
	//		if (false == g_arrayconnected[i]) { // 연결 되지 않은 상태일 때, 
	//			id = i; // 아이디를 부여한다.
	//			break;
	//		}
	//	}

	//	// 유저 2인 제한.
	//	if (-1 == id) {
	//		cout << "user가 다 찼습니다." << endl;
	//		closesocket(client_sock);
	//		continue;
	//	}

	//	// 클라이언트로부터 초기 설정한 PlayerInfo를 recv 한다. 직업 정보 등..
	//	retval = recv(client_sock, buf, BUFSIZE, 0);
	//	if (retval == SOCKET_ERROR) {
	//		err_display("recv()");
	//		break;
	//	}
	//	else if (retval == 0)
	//		break;

	//	// 클라이언트로부터 받은 PlayerInfo 데이터를 playerinfo 변수에 복사. 
	//	// buf[retval] = '\0';
	//	cout << "[TCP" << inet_ntoa(clientaddr.sin_addr) << " : " << ntohs(clientaddr.sin_port) << "]";
	//	memcpy(&playerinfo, &buf, sizeof(buf));

	//	// (debugging)
	//	{
	//		if (playerinfo.job == JOB_CAPTIN)
	//			cout << "PlayerInfo - 닉네임 : " << playerinfo.nickname << ", 직업 : 캡틴 / 정보 수신" << endl;
	//		else
	//			cout << "PlayerInfo - 닉네임 : " << playerinfo.nickname << ", 직업 : 스트라이커 / 정보 수신" << endl;
	//	}

	//	// Player Info에 값 채우기.
	//	playerinfo.id = id;
	//	playerinfo.connected = true;
	//	playerinfo.pt.x = 100.f; // 초기 좌표는 (100, 500)
	//	playerinfo.pt.y = 500.f;
	//	playerinfo.hp = 30000;
	//	playerinfo.size.cx = 100.f;
	//	playerinfo.size.cy = 100.f;

	//	while (true) {

	//	}
	//	// 여기부터!!

	//	//// buf에 다시 player info를 copy 할 필요가 있나..?
	//	//memcpy(&buf, &playerinfo, sizeof(playerinfo));

	//	//// 데이터 보내기
	//	//retval = send(client_sock, buf, BUFSIZE, 0);
	//	//if (retval == SOCKET_ERROR) {
	//	//	err_display("send()");
	//	//	break;
	//	//}
	//	//else {// 데이터 보내는 데 성공했으면
	//	//	g_vecplayer.push_back(playerinfo);// 정보가 다 채워진 playerinfo를 g_vecplayer에 담는다.
	//	//	break;
	//	//}
	//}

	//// -------------------------------------------
	//// 접속한 클라이언트에게, 다른 플레이어의 위치를 동기화한다.
	//ZeroMemory(buf, sizeof(buf)); // 버퍼 재사용.
	//memcpy(buf, &g_vecplayer, sizeof(g_vecplayer)); // 벡터 자체를 보내는건?

	//while (true) {
	//	if (g_vecplayer.size() <= 1) // 기존 플레이어가 없을 때
	//		break;
	//	else {
	//		// 고정 길이 데이터.
	//		int buflength = sizeof(g_vecplayer);
	//		send(client_sock, (char*)buflength, sizeof(int), 0);	// 고정 길이를 보낸다.
	//	}
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

void MakeMonster()
{

}


//void GreenMushRoom_MoveInPattern(MonsterInfo m)
//{
//	m_dwCreateCurTime = GetTickCount();
//
//	if (DIR_LEFT == m.dir)
//	{
//		switch (m_eCurState)
//		{
//		case MONSTER_WALK:
//		{
//			m.pt.x -= m_fSpeed;
//
//			if (m.pt.x < 465.f)
//				m.dir = DIR_RIGHT;
//		}
//		break;
//		case MONSTER_STAND:
//			break;
//		}
//
//	}
//	else
//	{
//		switch (m_eCurState)
//		{
//		case MONSTER_WALK:
//		{
//			m.pt.x += m_fSpeed;
//			if (m.pt.x > 1390.f)
//				m.dir = DIR_LEFT;
//		}
//		break;
//		case MONSTER_STAND:
//			break;
//		}
//
//	}
//
//	switch (m_iPattern)
//	{
//	case 1:
//	{
//		if (MONSTER_DAMAGED == m_eCurState)
//		{
//			if (m_dwDamageOldTime + m_dwDamageTime < m_dwDamageCurTime)
//			{
//				m_eCurState = MONSTER_WALK;
//				m_dwDamageOldTime = m_dwDamageCurTime;
//			}
//			return;
//		}
//
//		if (MONSTER_DEAD == m_eCurState)
//		{
//			return;
//		}
//
//		else
//		{
//			if (m_dwCreateOldTime + 3000 < m_dwCreateCurTime)
//			{
//				m_eCurState = MONSTER_STAND;
//			}
//			if (m_dwCreateOldTime + 6000 < m_dwCreateCurTime)
//			{
//				m_eCurState = MONSTER_WALK;
//				m_dwCreateOldTime = m_dwCreateCurTime;
//				m_bIsFloorBoxColl = false;
//			}
//
//		}
//
//	}
//	break;
//	case 2:
//	{
//		if (MONSTER_DAMAGED == m_eCurState)
//		{
//			if (m_dwDamageOldTime + m_dwDamageTime < m_dwDamageCurTime)
//			{
//				m_eCurState = MONSTER_WALK;
//				m_dwDamageOldTime = m_dwDamageCurTime;
//			}
//			return;
//		}
//		if (MONSTER_DEAD == m_eCurState)
//		{
//			return;
//		}
//
//		else
//		{
//			if (m_dwCreateOldTime + 7000 < m_dwCreateCurTime)
//			{
//				m_eCurState = MONSTER_STAND;
//			}
//			if (m_dwCreateOldTime + 11000 < m_dwCreateCurTime)
//			{
//				m_eCurState = MONSTER_WALK;
//				m_dwCreateOldTime = m_dwCreateCurTime;
//				m_bIsFloorBoxColl = false;
//			}
//
//		}
//
//	}
//	break;
//	case 3:
//	{
//		if (MONSTER_DAMAGED == m_eCurState)
//		{
//			if (m_dwDamageOldTime + m_dwDamageTime < m_dwDamageCurTime)
//			{
//				m_dwDamageOldTime = m_dwDamageCurTime;
//				m_eCurState = MONSTER_WALK;
//			}
//			return;
//		}
//		if (MONSTER_DEAD == m_eCurState)
//		{
//			return;
//		}
//
//
//		else
//		{
//			if (m_dwCreateOldTime + 10000 < m_dwCreateCurTime)
//			{
//				m_eCurState = MONSTER_STAND;
//			}
//			if (m_dwCreateOldTime + 15000 < m_dwCreateCurTime)
//			{
//				m_eCurState = MONSTER_WALK;
//				m_dwCreateOldTime = m_dwCreateCurTime;
//				m_bIsFloorBoxColl = false;
//			}
//
//		}
//
//	}
//	break;
//	}
//
//}
