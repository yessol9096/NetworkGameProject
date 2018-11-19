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

// monster ���� ���³� �Ⱥ��³� Ȯ��
bool bCreateMonster_check = false;
MONSTERINFO *pGreenMushroom_server = new MONSTERINFO;

// Ŭ���̾�Ʈ
vector<PLAYERINFO> g_vecplayer;
bool g_arrayconnected[MAX_USER]; // connected �迭 (id �ο� ����)

DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	PLAYERINFO playerinfo; 
	ZeroMemory(&playerinfo, sizeof(playerinfo));

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	// -------------------------------------------
	// ����
	bCreateMonster_check = true;
	if (bCreateMonster_check == true)
	{
		// �ʷϹ��� ������ ������
		pGreenMushroom_server->hp = 100;
		pGreenMushroom_server->key = NULL;
		pGreenMushroom_server->money = 10;
		pGreenMushroom_server->pt.x = HENESISCX * 0.5f;
		pGreenMushroom_server->pt.y = HENESISCY - 460.f;


		cout << "���� �Է� ���� ����" << endl;
		retval = send(client_sock, (char*)pGreenMushroom_server, sizeof(MONSTERINFO), 0);
		bCreateMonster_check = false;

		if (retval == SOCKET_ERROR) {
			err_display("send()");
			return 1;
		}
	}
	// -------------------------------------------

	while (true) {
		// g_vecplayer �� ���� ã�Ƽ� id �ο�.
		int id = -1;
		for (int i = 0; i < MAX_USER; ++i) {
			if (false == g_arrayconnected[i]) { // ���� ���� ���� ������ ��, 
				id = i; // ���̵� �ο��Ѵ�.
				break;
			}
		}

		if (-1 == id) {	// ���� 2�� ����.
			cout << "user�� �� á���ϴ�." << endl;
			closesocket(client_sock);
			continue;
		}


		// Ŭ���̾�Ʈ�κ��� �ʱ� ������ PlayerInfo�� �޾ƿ´�. ���� ���� ��..
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// Ŭ���̾�Ʈ�κ��� ���� PlayerInfo �����͸� playerinfo ������ ����. 
		// buf[retval] = '\0';
		cout << "[TCP" << inet_ntoa(clientaddr.sin_addr) << " : " << ntohs(clientaddr.sin_port) << "]" << "�κ��� ������ ����" << endl;
		memcpy(&playerinfo, &buf, sizeof(buf));

		// Player Info�� �� ä���.
		playerinfo.id = id;
		playerinfo.connected = true;
		playerinfo.pt.x = 100.f; // �ʱ� ��ǥ�� (100, 500)
		playerinfo.pt.y = 500.f;
		playerinfo.hp = 30000;
		playerinfo.size.cx = 100.f;
		playerinfo.size.cy = 100.f;

		memcpy(&buf, &playerinfo, sizeof(playerinfo));

		// ������ ������
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		else {// ������ ������ �� ����������
			g_vecplayer.push_back(playerinfo);// ������ �� ä���� playerinfo�� g_vecplayer�� ��´�.
			break;
		}
	}
	// closesocket()
	closesocket(client_sock);
	cout << "[Ŭ���̾�Ʈ ����]" << endl;
	cout << "IP �ּ� (" << inet_ntoa(clientaddr.sin_addr) << "), ��Ʈ ��ȣ ("
		<< ntohs(clientaddr.sin_port) << ")" << endl;

	return 0;
}
DWORD WINAPI CalculateThread()
{
	return 0;
}

int main()
{
	// ���� 2�� ����.
	g_vecplayer.reserve(MAX_USER);

	int retval;

	// ���� �ʱ�ȭ.
	InitializeNetwork();

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	// �ּ� ����ü ����
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

	// ������ ��ſ� ����� ����
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

		// ������ Ŭ���̾�Ʈ ���� ���
		cout << "[Ŭ���̾�Ʈ ����]" << endl;
		cout << "IP �ּ� (" << inet_ntoa(clientaddr.sin_addr) << "), ��Ʈ ��ȣ ("
			<< ntohs(clientaddr.sin_port) << ") " << endl;

		// ������ ����
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
	// ���� �ʱ�ȭ.
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		err_quit("WSAStartup()");
}

void EndNetwork()
{
	// ���� ����
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
