#include <winsock2.h>
#include <iostream>
#include <vector>
#include "Server.h"
#include "Include.h"
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

// ������ ����ȭ
CRITICAL_SECTION cs;

// Ŭ���̾�Ʈ
vector<PLAYERINFO> g_vecplayer;
bool g_arrayconnected[MAX_USER]; // connected �迭 (id �ο� ����)
vector<SOCKET> g_vecsocket;

// monster ���� ���³� �Ⱥ��³� Ȯ��
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

	// Ŭ���̾�Ʈ ���� ���
	int addrlen = sizeof(clientaddr);
	EnterCriticalSection(&cs);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);
	LeaveCriticalSection(&cs);

	int retval{ 0 };

	PACKETINFO packetinfo;
	PLAYERINFO playerinfo;


	
	// -------------------------------------------
	// ���� : ���� ���� + ���� ����.
	while (true) {
		char buf[BUFSIZE];

		// ���� ����.
		// ������ ���� ��Ŷ ������ �˾Ƴ���.
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

		// ���� ����.
		switch (packetinfo.type) {
		case CS_PACKET_PLAYERINFO_INITIALLY:
		{
			// Ŭ���̾�Ʈ�� �ʱ� ���� �� playerinfo�� recvn �Ѵ�.
			// 1. �ʱ� ������ playeinfo�� �����Ѵ�.
			// 2. playerinfo�� ������ ��� �������� ä���ش�. (id, size, hp, mp, ..)
			// 3. ������ ��� �������� ä���� playerinfo�� ���Ϳ� push �Ѵ�.
			// 4. Ŭ���̾�Ʈ���� "�� ������ �̰ž�!" ��� send �Ͽ� �˸���.
			// 5. ������ �ٸ� Ŭ���̾�Ʈ���� ���� playerinfo�� �����Ͽ� ���� ������ �˸���. (2�� �����ߴٸ� 1����, 1�� �����ߴٸ� �ƹ����Ե� X)

			// --------------------Process---------------------
			// 1. �ʱ� ������ playeinfo�� �����Ѵ�.
			{
				ZeroMemory(&buf, sizeof(buf));	 /// ���� �� �Ŵϱ�. ��� �����.		
				EnterCriticalSection(&cs);
				retval = recvn(client_sock, buf, BUFSIZE, 0);
				LeaveCriticalSection(&cs);
				std::cout << "���� ���� ���� - CS_PACKET_PLAYERINFO_INITIALLY" << endl;
				if (retval == SOCKET_ERROR) {
					err_display("intial playerinfo recv()");
					break;
				}
				else {
					/// ���� ���� �����Ϳ� recv �ؿ� �޸𸮸�ŭ ���� �Ѵ�.
					ZeroMemory(&playerinfo, sizeof(playerinfo));
					memcpy(&playerinfo, buf, sizeof(playerinfo));
				}
			}

			// 2. playerinfo�� ������ ��� �������� ä���ش�. (id, size, hp, mp, ..)
			{
				int id = -1;
				for (int i = 0; i < MAX_USER; ++i) {
					if (false == g_arrayconnected[i]) { /// ���������� �����ؼ� ���� ���� ���� �ε��� i�� ã��, 
						g_arrayconnected[i] = true;
						id = i; /// ���̵� �ο��Ѵ�.
						break;
					}
				}

				/// ���� 2�� ����.
				if (-1 == id) {
					std::cout << "user�� �� á���ϴ�." << std::endl;
					EnterCriticalSection(&cs);
					closesocket(client_sock);
					LeaveCriticalSection(&cs);
					break;
				}

				// ���� playerinfo�� ���� id�� �ο��Ѵ�.
				playerinfo.id = id;
				playerinfo.connected = true;
				playerinfo.ready = false;
				playerinfo.pt.x = 100.f; // �ʱ� ��ǥ�� (100, 500)
				playerinfo.pt.y = 500.f;
				playerinfo.hp = 30000;
				playerinfo.size.cx = 100.f;
				playerinfo.size.cy = 100.f;
				playerinfo.attackAccValue = 0.f;
				playerinfo.money = 0;
			}

			// 3. ������ ��� �������� ä���� playerinfo�� ���Ϳ� push �Ѵ�.
			g_vecplayer.push_back(playerinfo);

			// 4. Ŭ���̾�Ʈ���� "�� ������ �̰ž�!" ��� send �Ͽ� �˸���.
			{
				// ���� ����. 
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
					err_display("send() - ���� ���� - SC_PACKET_YOUR_PLAYERINFO");

				// ���� ����. 
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &(playerinfo), sizeof(playerinfo));
				EnterCriticalSection(&cs);
				retval = send(client_sock, buf, BUFSIZE, 0);
				LeaveCriticalSection(&cs);
				if (retval == SOCKET_ERROR)
					err_display("send() - ���� ���� - SC_PACKET_YOUR_PLAYERINFO");
			}

			// 5. ������ Ŭ���̾�Ʈ���� ���� playerinfo�� �����Ͽ� ���� ������ �˸���. (2�� �����ߴٸ� 1����, 1�� �����ߴٸ� �ƹ����Ե� X)
			{
				for (int i = 0; i < g_vecplayer.size(); ++i) {
					/// �����Դ� ���� �ʿ� ����.
					if (i == playerinfo.id)
						continue;
					/// ���� ����.
					ZeroMemory(&packetinfo, sizeof(packetinfo));
					packetinfo.type = SC_PACKET_OTHER_PLAYERINFO;
					packetinfo.size = sizeof(playerinfo);
					packetinfo.id = playerinfo.id; /// playerinfo.id�� ������ ���̵���.
					ZeroMemory(buf, sizeof(buf));
					memcpy(buf, &packetinfo, sizeof(packetinfo));
					EnterCriticalSection(&cs);
					retval = send(g_vecsocket[i], buf, BUFSIZE, 0);	// �ٸ� Ŭ���̾�Ʈ���� ������ �Ŵϱ�, g_vecsocket���� �����ؾ߰���.
					LeaveCriticalSection(&cs);
					if (retval == SOCKET_ERROR) {
						err_display("send() - SC_PACKET_NEW_PLAYERINFO");
					}
					/// ���� ����.
					ZeroMemory(buf, sizeof(buf)); /// ���� ����.
					memcpy(buf, &playerinfo, sizeof(g_vecplayer[i]));
					EnterCriticalSection(&cs);
					retval = send(g_vecsocket[i], buf, BUFSIZE, 0);
					LeaveCriticalSection(&cs);
					cout << "���� ���� ���� - SC_PACKET_PLAYERINFO - " << i << "��° Ŭ���̾�Ʈ" << endl;
					if (retval == SOCKET_ERROR) {
						err_display("send() - SC_PACKET_PLAYERINFO");
					}
				}
			}

		}
		break;
		case CS_PACKET_PLAYERINFO_MOVE:
		{
			// �÷��̾ �������� �� ��Ŷ�� ������ ������ �κ�.
			// 2. ���� ���� ��Ŷ�� �޾� playerinfo�� �޴´�.
			// 3. g_vecplayer[���� playerinfo�� id] �� �����Ͽ� ������ �����Ѵ�.
			// 4. �ٸ� Ŭ���̾�Ʈ���Ե� ������. (SC_PACKET_OTHER_PLAYERINFO)

			// -------------------Process---------------------
			// 1. ���� ���� ��Ŷ����, ��� Ŭ�� �ش�Ǵ��� id �˾ƿ�.
			int id = packetinfo.id;
			// 2. ���� ���� ��Ŷ�� �޾� playerinfo�� �޴´�.
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

			// 3. g_vecplayer[���� playerinfo�� id]�� �����Ͽ� ������ �����Ѵ�.
			{
				g_vecplayer[id] = tempplayerinfo;
			}

			// 4. �ٸ� Ŭ���̾�Ʈ���Ե� ������. (SC_PACKET_OTHER_PLAYERINFO)
			if (g_vecplayer.size() == 2) {// 2�� ���� �ÿ���.
				// ���� ����. 
				ZeroMemory(&packetinfo, sizeof(packetinfo));
				packetinfo.id = id;
				packetinfo.size = sizeof(PLAYERINFO);
				packetinfo.type = SC_PACKET_OTHER_PLAYERINFO;
				memcpy(buf, &packetinfo, sizeof(packetinfo));
				if (0 == id) {	// 0 ������ 1���� ������ �Ѵ�.
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
					cout << "failed : send - ���� - SC_PACKET_OTHER_PLAYERINFO" << endl;
					break;
				}

				// ���� ����
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &(g_vecplayer[id]), sizeof(playerinfo));
				if (0 == id) {	// 0 ������ 1���� ������ �Ѵ�.
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
					cout << "failed : send - ���� - SC_PACKET_OTHER_PLAYERINFO" << endl;
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
			cout << "[Ŭ���̾�Ʈ ���� ����] IP �ּ� (" << inet_ntoa(clientaddr.sin_addr) <<
				"), ��Ʈ ��ȣ (" << ntohs(clientaddr.sin_port) << ")" << endl;

			return 0;
		}
		break;
		}
	}

	
	closesocket(client_sock);
	cout << "[Ŭ���̾�Ʈ ���� ����] IP �ּ� (" << inet_ntoa(clientaddr.sin_addr) <<
		"), ��Ʈ ��ȣ (" << ntohs(clientaddr.sin_port) << ")" << endl;

	return 0;
}

DWORD WINAPI MonsterThread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	char buf[BUFSIZE];
	bool monsterthread_start = false;
	// Ŭ���̾�Ʈ ���� ���
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
	// ���� 2�� ����.
	g_vecplayer.reserve(MAX_USER);

	int retval;

	// �Ӱ� ���� �ʱ�ȭ
	InitializeCriticalSection(&cs);

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
	HANDLE hThread[2];

	while (true) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ socket�� �迭�� ��´�. (1201)
		g_vecsocket.push_back(client_sock);

		// ������ Ŭ���̾�Ʈ ���� ���
		cout << "[Ŭ���̾�Ʈ ����] IP �ּ� (" << inet_ntoa(clientaddr.sin_addr) <<
			"), ��Ʈ ��ȣ (" << ntohs(clientaddr.sin_port) << ")" << endl;

		// Ŭ���̾�Ʈ ������ ����
		hThread[0] = CreateThread(NULL, 0, ClientThread, (LPVOID)client_sock, 0, NULL);
		if (hThread[0] == NULL)	closesocket(client_sock);
		else					CloseHandle(hThread[0]);

		//���� ������ ����
		hThread[1] = CreateThread(NULL, 0, MonsterThread, (LPVOID)client_sock, 0, NULL);
	}
	// �� ���� ������ ���� ���
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	// �Ӱ� ���� ����
	DeleteCriticalSection(&cs);

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
//	//		/// ���� ���� ������ ����,,
//	//		ZeroMemory(&packetinfo, sizeof(packetinfo)); /// packetinfo ����.
//	//		packetinfo.type = SC_PACKET_PLAYERINFO;
//	//		packetinfo.size = sizeof(g_vecplayer[i]);
//	//		packetinfo.id = i;
//	//		ZeroMemory(buf, sizeof(buf)); /// ���� ����.
//	//		memcpy(buf, &packetinfo, sizeof(packetinfo));
//	//		retval == send(client_sock, buf, BUFSIZE, 0);
//	//		cout << "���� ���� ���� - SC_PACKET_PLAYERINFO - " << i << "��° Ŭ���̾�Ʈ" << endl;
//	//		if (retval == SOCKET_ERROR) {
//	//			err_display("send() - SC_PACKET_PLAYERINFO");
//	//		}
//
//	//		/// ���� ���� ������ ����,,
//	//		ZeroMemory(buf, sizeof(buf)); /// ���� ����.
//	//		memcpy(buf, &(g_vecplayer[i]), sizeof(g_vecplayer[i]));
//	//		retval == send(client_sock, buf, BUFSIZE, 0);
//	//		cout << "���� ���� ���� - SC_PACKET_PLAYERINFO - " << i << "��° Ŭ���̾�Ʈ" << endl;
//	//		if (retval == SOCKET_ERROR) {
//	//			err_display("send() - SC_PACKET_PLAYERINFO");
//	//		}
//	//	}
//	//}
//}