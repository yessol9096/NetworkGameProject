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
vector<MONSTERINFO> g_vecgreen;
int greenposX[] = { HENESISCX * 0.5f, HENESISCX * 0.7f , HENESISCX * 0.6f ,HENESISCX * 0.7f,HENESISCX * 0.5f, HENESISCX * 0.4f };
int greenposY = HENESISCY - 460.f;
OBJECT_DIR greenDir[] = { DIR_LEFT, DIR_RIGHT ,DIR_RIGHT, DIR_LEFT, DIR_RIGHT, DIR_RIGHT };
int greenPtr[] = {1,1,3,1,1,3};

// Ŭ���̾�Ʈ
vector<PLAYERINFO> g_vecplayer;
bool g_arrayconnected[MAX_USER]; // connected �迭 (id �ο� ����)
DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval{ 0 };
	SOCKADDR_IN clientaddr;
	int addrlen;
	int datatype;
	MONSTERINFO monsterinfo;
	PLAYERINFO playerinfo;
	ZeroMemory(&playerinfo, sizeof(playerinfo));


	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	// -------------------------------------------
	// ���� �ٲٱ�. ���� ���� + ���� ����.
	PACKETINFO packetinfo;
	while (true) {
		char buf[BUFSIZE];

		// ���� ����.
		ZeroMemory(&packetinfo, sizeof(packetinfo));	 /// ���� �� �Ŵϱ�. ��� �����.		
		while (true) {
			int totalreadbytes{ 0 }, readbytes{ 0 };

			/// recvn���� ��� �޾ƿ;� ��. tcp Ư�� ������..!
			readbytes = retval = recvn(client_sock, buf, BUFSIZE, 0);
			/// �ޱ� ���� ��,
			if (retval == SOCKET_ERROR) {
				err_display("packetinfo recv()");
				break;
			}
			/// �ޱ� ���� ��,
			else {
				/// ���� ���� �����Ϳ� recv �ؿ� �޸𸮸�ŭ ���� �Ѵ�.
				memcpy(&packetinfo + totalreadbytes, buf, readbytes);
				/// �޾ƿ� ��ŭ �����Ͽ� ���Ѵ�.
				totalreadbytes += readbytes;
			}
			/// �� ������, 
			if (totalreadbytes >= sizeof(PACKETINFO)) {
				break; /// recvn ������ ������.
			}
		}

		// ���� ����.
		switch (packetinfo.type) {
		case CS_PACKET_PLAYERINFO_INITIALLY: // �ʱ� �÷��̾� info.
		{
			char buf[BUFSIZE];

			// Ŭ���̾�Ʈ�κ��� �ʱ� ������ PlayerInfo�� recvn �Ѵ�.
			while (true) {
				int totalreadbytes{ 0 }, readbytes{ 0 };
				readbytes = retval = recvn(client_sock, buf, BUFSIZE, 0);
				/// �ޱ� ���� ��,
				if (retval == SOCKET_ERROR) {
					err_display("intial playerinfo recv()");
					break;
				}
				/// �ޱ� ���� ��,
				else {
					/// ���� ���� �����Ϳ� recv �ؿ� �޸𸮸�ŭ ���� �Ѵ�.
					memcpy(&playerinfo + totalreadbytes, buf, readbytes);
					/// �޾ƿ� ��ŭ �����Ͽ� ���Ѵ�.
					totalreadbytes += readbytes;
				}
				/// �� ������, 
				if (totalreadbytes >= sizeof(packetinfo.size)) {
					break; /// recvn ������ ������.
			}
#ifdef DEBUGGING
				cout << "[TCP" << inet_ntoa(clientaddr.sin_addr) << " : " << ntohs(clientaddr.sin_port) << "]";
				if (playerinfo.job == JOB_CAPTIN)
					cout << "[RECV]" <<  "PlayerInfo - �г��� : " << playerinfo.nickname << ", ���� : ĸƾ" << endl;
				else
					cout << "[RECV]" << "PlayerInfo - �г��� : " << playerinfo.nickname << ", ���� : ��Ʈ����Ŀ" << endl;
#endif
		}

			// playerinfo�� id �ο�.
			{
				int id = -1;
				for (int i = 0; i < MAX_USER; ++i) {
					if (false == g_arrayconnected[i]) { /// ���������� �����ؼ� ���� ���� ���� �ε��� i�� ã��, 
						id = i; /// ���̵� �ο��Ѵ�.
						break;
					}
				}

				/// ���� 2�� ����.
				if (-1 == id) {
					cout << "user�� �� á���ϴ�." << endl;
					closesocket(client_sock);
					break;
				}

				// ���� playerinfo�� id�� �ο��Ѵ�.
				playerinfo.id = id;
			}

			// playerinfo�� �� ä���.
			{
				playerinfo.connected = true;
				playerinfo.pt.x = 100.f; // �ʱ� ��ǥ�� (100, 500)
				playerinfo.pt.y = 500.f;
				playerinfo.hp = 30000;
				playerinfo.size.cx = 100.f;
				playerinfo.size.cy = 100.f;
			}

			// ���Ӱ� ���ŵ� playerinfo�� buf�� copy �ؼ� ������.
			ZeroMemory(buf, sizeof(buf));
			memcpy(&buf, &playerinfo, sizeof(playerinfo));
			retval = send(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			else { // ������ ������ �� ����������
				g_vecplayer.push_back(playerinfo); // ������ �� ä���� playerinfo�� g_vecplayer�� ��´�.
			}

			// -------------------------------------------
			// ���� ������ Ŭ���̾�Ʈ����, �ٸ� �÷��̾��� ��ġ�� ����ȭ�Ѵ�.

			if (g_vecplayer.size() <= 1) /// ���� �÷��̾ ���� ��
				break;
			else {
				/// ���� ���� ������ ����,,
				ZeroMemory(&packetinfo, sizeof(packetinfo)); /// packetinfo ����.
				packetinfo.type = SC_PACKET_PLAYERVECTOR;
				packetinfo.size = sizeof(g_vecplayer);
				ZeroMemory(buf, sizeof(buf)); /// ���� ����.
				memcpy(buf, &packetinfo, sizeof(buf));

				send(client_sock, buf, BUFSIZE, 0);	
			}

		}
		break;
		}

	}

	// -------------------------------------------
	// ����
	//bCreateMonster_check = true;
	//if (bCreateMonster_check == true)
	//{
	//	// �ʷϹ��� ������ ����
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
	//		cout << "���� �Է� ���� ����" << endl;

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
	g_vecgreen.reserve(MAX_GREEN);
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
