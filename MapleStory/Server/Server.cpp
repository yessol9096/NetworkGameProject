#include <winsock2.h>
#include <iostream>
#include <vector>
#include "Server.h"
#include "Include.h"
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

#define DEBUG

// ������ ����ȭ
CRITICAL_SECTION cs;

// Ŭ���̾�Ʈ
vector<PLAYERINFO> g_vecplayer;
bool g_arrayconnected[MAX_USER]; // connected �迭 (id �ο� ����)
vector<SOCKET> g_vecsocket;
vector<SKILLINFO> g_vecskill;

// monster ���� ���³� �Ⱥ��³� Ȯ��
bool bCreateMonster_check = false;
vector<MONSTERINFO> g_vecgreen(MAX_GREEN);
int greenposX[] = { HENESISCX * 0.5f,HENESISCX * 0.7f , HENESISCX * 0.6f ,HENESISCX * 0.7f,HENESISCX * 0.5f, HENESISCX * 0.4f };
int greenposY = HENESISCY - 460.f;
OBJECT_DIR greenDir[] = { DIR_LEFT, DIR_RIGHT ,DIR_RIGHT, DIR_LEFT, DIR_RIGHT, DIR_RIGHT };
int greenPtr[] = { 1,1,3,1,1,3 };

//���� Ŭ���̾�Ʈ üũ 

bool test = false;
bool test2 = false;
DWORD WINAPI ClientThread(LPVOID arg)
{
	int retval{ 0 };

	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;

	// Ŭ���̾�Ʈ ���� ���
	int addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);


	PACKETINFO packetinfo;
	PLAYERINFO playerinfo;

	
	// -------------------------------------------
	// ���� : ���� ���� + ���� ����.
	while (true) {
		cout << "--Player Thread--" << endl;
		if (g_vecplayer.size() >= 1)
			test = true;
		if (g_vecplayer.size() >= 2 && g_vecsocket.size() >= 2)
			test2 = true;
		cout << "�÷��̾��ũ��:" << g_vecplayer.size() << endl;
		char buf[BUFSIZE];

		// ���� ����.
		// ������ ���� ��Ŷ ������ �˾Ƴ���.
		{
			ZeroMemory(&packetinfo, sizeof(packetinfo));
			retval = recvn(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("packetinfo recv()");
				break;
			}
			else
				memcpy(&packetinfo, buf, sizeof(packetinfo));
#ifdef DEBUG
			cout << "���� ���� ��Ŷ�� �޾ƿԾ��!" << endl;
#endif
		}

		// ���� ����.
		switch (packetinfo.type) {
		case CS_PACKET_PLAYERINFO_INITIALLY:
		{
			EnterCriticalSection(&cs);
			// Ŭ���̾�Ʈ�� �ʱ� ���� �� playerinfo�� recvn �Ѵ�.
			// 1. �ʱ� ������ playeinfo�� �����Ѵ�.
			// 2. playerinfo�� ������ ��� �������� ä���ش�. (id, size, hp, mp, ..)
			// 3. ������ ��� �������� ä���� playerinfo�� ���Ϳ� push �Ѵ�.
			// 4. Ŭ���̾�Ʈ���� "�� ������ �̰ž�!" ��� send �Ͽ� �˸���.
			// 5. ������ �ٸ� Ŭ���̾�Ʈ���� ���� playerinfo�� �����Ͽ� ���� ������ �˸���. (2�� �����ߴٸ� 1����, 1�� �����ߴٸ� �ƹ����Ե� X)
			// 6. �� Ŭ���̾�Ʈ���� �̹� �����ϴ� Ŭ���̾�Ʈ�� ������ send �Ѵ�.

			// --------------------Process---------------------
			// 1. �ʱ� ������ playeinfo�� �����Ѵ�.
			{
				ZeroMemory(buf, sizeof(buf));	 /// ���� �� �Ŵϱ�. ��� �����.		
				retval = recvn(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("intial playerinfo recv()");
					break;
				}
				else {
					/// ���� ���� �����Ϳ� recv �ؿ� �޸𸮸�ŭ ���� �Ѵ�.
					ZeroMemory(&playerinfo, sizeof(playerinfo));
					memcpy(&playerinfo, buf, sizeof(playerinfo));
#ifdef DEBUG
					cout << "���� ���� - �ʱ� ������ playerinfo�� �����߾��!" << endl;
#endif
				}
			}

			// 2. playerinfo�� ������ ��� �������� ä���ش�. (id, size, hp, mp, ..)
			{
				int id = -1;
				for (int i = 0; i < MAX_USER; ++i) {
					if (false == g_arrayconnected[i]) { /// ���������� �����ؼ� ���� ���� ���� �ε��� i�� ã��, 
						g_arrayconnected[i] = true;
						id = i; /// ���̵� �ο��Ѵ�.
#ifdef DEBUG		
						cout << "������ �÷��̾�� id�� �ο��߽��ϴ�!" << endl;
#endif
						break;
					}
				}

				/// ���� 2�� ����.
				if (-1 == id) {
					cout << "user�� �� á���ϴ�." << std::endl;
					closesocket(client_sock);
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
			{
#ifdef DEBUG
				cout << "������ ä���� playerinfo�� g_vecplayer�� push �߾��!" << endl;
#endif
				g_vecplayer.push_back(playerinfo);
				test = true;
			}

			// 4. ���� ������ Ŭ���̾�Ʈ���� "�� ������ �̰ž�!" ��� send �Ͽ� �˸���.
			{
				// ���� ����. 
				ZeroMemory(&packetinfo, sizeof(packetinfo));
				packetinfo.id = playerinfo.id;
				packetinfo.size = sizeof(playerinfo);
				packetinfo.type = SC_PACKET_YOUR_PLAYERINFO;
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &packetinfo, sizeof(packetinfo));
				retval = send(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					cout << ("send() ���� - ���� ���� - SC_PACKET_YOUR_PLAYERINFO") << endl;
					break;
				}
#ifdef DEBUG
				cout << "Ŭ���̾�Ʈ���� YOUR_PLAYERINFO�� ���� ���� �����߾��!" << endl;
#endif

				// ���� ����. 
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &(playerinfo), sizeof(playerinfo));
				retval = send(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() ���� - ���� ���� - SC_PACKET_YOUR_PLAYERINFO");
					break;
				}
#ifdef DEBUG
				cout << "Ŭ���̾�Ʈ���� YOUR_PLAYERINFO�� ���� ���� �����߾��!" << endl;
#endif

			}

			// 5. ������ ������ �ִ� Ŭ���̾�Ʈ���� �� Ŭ���̾�Ʈ�� playerinfo�� �����Ѵ�.
			if (g_vecplayer.size() >= 2) {
				/// ���� ����.
				ZeroMemory(&packetinfo, sizeof(packetinfo));
				packetinfo.type = SC_PACKET_NEW_OTHER_PLAYERINFO;
				packetinfo.size = sizeof(playerinfo);
				packetinfo.id = playerinfo.id; /// playerinfo.id�� ������ ���̵���.
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &packetinfo, sizeof(packetinfo));

				int recvid;  // ���� Ŭ���̾�Ʈ�� id��?
				if (playerinfo.id == 0) recvid = 1; else recvid = 0;
				retval = send(g_vecsocket[recvid], buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() - SC_PACKET_NEW_OTHER_PLAYERINFO");
					break;
				}
#ifdef DEBUG
				cout << recvid << "��° Ŭ���̾�Ʈ���� " << playerinfo.id << "�� OTHER_PLAYERINFO�� ���� ���� �����߾��!" << endl;
#endif
				/// ���� ����.
				ZeroMemory(buf, sizeof(buf)); /// ���� ����.
				memcpy(buf, &playerinfo, sizeof(g_vecplayer[playerinfo.id]));
				retval = send(g_vecsocket[recvid], buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() - SC_PACKET_PLAYERINFO");
					break;
				}
#ifdef DEBUG
				cout << recvid << "��° Ŭ���̾�Ʈ���� " << playerinfo.id << "�� OTHER_PLAYERINFO�� ���� ���� �����߾��!" << endl;
#endif
			}

			// 6. �� Ŭ���̾�Ʈ���� �̹� �����ϴ� Ŭ���̾�Ʈ�� ������ send �Ѵ�.
			if (g_vecplayer.size() >= 2) {
				// ���� �÷��̾ 2�� ���� �ش��Ѵ�. �� 1��° Ŭ���̾�Ʈ�� �ش��.
				// ���� ����.
				PACKETINFO temppacketinfo = {};
				temppacketinfo.id = 0;// 1��° Ŭ���̾�Ʈ�� 0��° Ŭ���̾�Ʈ�� ������ �޾ƾ� �Ѵ�.
				temppacketinfo.size = sizeof(g_vecplayer[temppacketinfo.id]);
				temppacketinfo.type = SC_PACKET_NEW_OTHER_PLAYERINFO;
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &temppacketinfo, sizeof(temppacketinfo));
				retval = send(g_vecsocket[1], buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() - SC_PACKET_NEW_OTHER_PLAYERINFO");
					break;
				}
				else {
#ifdef DEBUG
					cout << "���� ������ [" << 1 << "] ���� �̹� ������ [" << temppacketinfo.id << "] ��° Ŭ���̾�Ʈ�� ���� ���̸� ���¾��!" << endl;
#endif 
				}

				// ���� ����.
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &(g_vecplayer[temppacketinfo.id]), sizeof(g_vecplayer[temppacketinfo.id]));
				retval = send(g_vecsocket[1], buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() - SC_PACKET_NEW_OTHER_PLAYERINFO");
					break;
				}
				else {
#ifdef DEBUG
					cout << "���� ������ [" << packetinfo.id << "] ���� �̹� ������ [" << temppacketinfo.id << "] ��° Ŭ���̾�Ʈ�� ���� ���̸� ���¾��!" << endl;
#endif 
				}
			}
			LeaveCriticalSection(&cs);
		}
		break;
		case CS_PACKET_PLAYERINFO_MOVE:
		{
			EnterCriticalSection(&cs);

			// �÷��̾ �������� �� ��Ŷ�� ������ ������ �κ�.
			// 1. ���� ���� ��Ŷ����, ��� Ŭ�� �ش�Ǵ��� id �˾ƿ�.
			// 2. g_vecplayer[���� playerinfo�� id] �� �����Ͽ� ������ �����Ѵ�.

			//
			// 2. ���� ���� ��Ŷ�� �޾� playerinfo�� �޴´�.
			// 3. g_vecplayer[���� playerinfo�� id] �� �����Ͽ� ������ �����Ѵ�.
			// 4. �ٸ� Ŭ���̾�Ʈ���Ե� ������. (SC_PACKET_OTHER_PLAYERINFO)
			// 5. ������ Ŭ���̾�Ʈ���Ե� ������. (SC_PACKET_YOUR_PLAYERINFO)

			// -------------------Process---------------------
			// 1. ���� ���� ��Ŷ����, ��� Ŭ�� �ش�Ǵ��� id �˾ƿ�.
			int id = packetinfo.id;
			// 2. ���� ���� ��Ŷ�� �޾� playerinfo�� �޴´�.
			PLAYERINFO tempplayerinfo = {};
			{
				ZeroMemory(buf, sizeof(buf));
				retval = recvn(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					cout << "���� - recvn - CS_PACKET_PLAYERINFO_MOVE" << endl;
					break;
				}
				else
					memcpy(&tempplayerinfo, buf, sizeof(tempplayerinfo));
#ifdef DEBUG
				cout << id << "��° Ŭ���̾�Ʈ�� ���������Ƿ� ���� ���� ��Ŷ�� �޾ƿԾ��!" << endl;
#endif
			}

			// 3. g_vecplayer[���� playerinfo�� id]�� �����Ͽ� ������ �����Ѵ�.
			{
				g_vecplayer[id] = tempplayerinfo;
			}

			// 4. �ٸ� Ŭ���̾�Ʈ���Ե� ������. (SC_PACKET_OTHER_PLAYERINFO)
			if (g_vecplayer.size() == 2) { // 2�� ���� �ÿ���.
				// ���� ����. 
				ZeroMemory(&packetinfo, sizeof(packetinfo));
				packetinfo.id = id;
				packetinfo.size = sizeof(PLAYERINFO);
				packetinfo.type = SC_PACKET_OTHER_PLAYERINFO;
				memcpy(buf, &packetinfo, sizeof(packetinfo));
				if (0 == id) {	// 0 ������ 1���� ������ �Ѵ�.
					retval = send(g_vecsocket[id + 1], buf, BUFSIZE, 0);
				}
				else if (1 == id) {
					retval = send(g_vecsocket[id - 1], buf, BUFSIZE, 0);
				}
				if (retval == SOCKET_ERROR) {
					cout << "failed : send - ���� - SC_PACKET_OTHER_PLAYERINFO" << endl;
					break;
				}
				else {
#ifdef DEBUG
					cout << id << "��° Ŭ���̾�Ʈ�� ���������Ƿ� " << "�ٸ� Ŭ���̾�Ʈ���� ���� ���� ��Ŷ�� �����մϴ�!" << endl;
#endif
				}

				// ���� ����
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &(g_vecplayer[id]), sizeof(playerinfo));
				if (0 == id) {	// 0 ������ 1���� ������ �Ѵ�.
					retval = send(g_vecsocket[id + 1], buf, BUFSIZE, 0);
				}
				else if (1 == id) {
					retval = send(g_vecsocket[id - 1], buf, BUFSIZE, 0);
				}
				if (retval == SOCKET_ERROR) {
					cout << "failed : send - ���� - SC_PACKET_OTHER_PLAYERINFO" << endl;
					break;
				}
				else {
#ifdef DEBUG
					cout << id << "��° Ŭ���̾�Ʈ�� ���������Ƿ� " << "�ٸ� Ŭ���̾�Ʈ���� ���� ���� ��Ŷ�� �����մϴ�!" << endl;
#endif
				}

			}

			// 5. ������ Ŭ���̾�Ʈ���Ե� ������. (SC_PACKET_YOUR_PLAYERINFO)
//			 {  
//				// ���� ����. 
//				ZeroMemory(&packetinfo, sizeof(packetinfo));
//				packetinfo.id = id;
//				packetinfo.size = sizeof(PLAYERINFO);
//				packetinfo.type = SC_PACKET_YOUR_PLAYERINFO;
//				memcpy(buf, &packetinfo, sizeof(packetinfo));
//					retval = send(g_vecsocket[id], buf, BUFSIZE, 0);
//				if (retval == SOCKET_ERROR) {
//					cout << "failed : send - ���� - SC_PACKET_OTHER_PLAYERINFO" << endl;
//					break;
//				}
//				else {
//#ifdef DEBUG
//					cout << id << "��° Ŭ���̾�Ʈ�� ���������Ƿ� " << "�ش� Ŭ���̾�Ʈ���� ���� ���� ��Ŷ�� �����մϴ�!" << endl;
//#endif
//				}
//
//				// ���� ����
//				ZeroMemory(buf, sizeof(buf));
//				memcpy(buf, &(g_vecplayer[id]), sizeof(playerinfo));
//				retval = send(g_vecsocket[id], buf, BUFSIZE, 0);
//				if (retval == SOCKET_ERROR) {
//					cout << "failed : send - ���� - SC_PACKET_OTHER_PLAYERINFO" << endl;
//					break;
//				}
//				else {
//#ifdef DEBUG
//					cout << id << "��° Ŭ���̾�Ʈ�� ���������Ƿ� " << "�ش� Ŭ���̾�Ʈ���� ���� ���� ��Ŷ�� �����մϴ�!" << endl;
//#endif
//				}
//
//			}

			LeaveCriticalSection(&cs);
		}
		break;
		case CS_PACKET_PLAYERINFO_INCHANGINGSCENE:
		{
			EnterCriticalSection(&cs);
			// ���� �ٲ���� ��, Ŭ���̾�Ʈ�� �� ��Ŷ�� �ѹ� ������.
			// 1. ���� ���� ��Ŷ����, ��� Ŭ�� �ش�Ǵ��� id �˾ƿ´�.
			// 2. ���� ���� ��Ŷ�� �ް�, �޾ƿ� ������� playerinfo�� �����Ѵ�.
			// 3. g_vecplayer[���� playerinfo�� id]�� �����Ͽ� ������ �����Ѵ�.
			// 4. ��� Ŭ���̾�Ʈ���� g_vecplayer[0], g_vecplayer[1] ������ ������.

			// ----------------------- Process -------------------------
			// 1. ���� ���� ��Ŷ����, ��� Ŭ�� �ش�Ǵ��� id �˾ƿ´�.
			int id = packetinfo.id;
			// 2. ���� ���� ��Ŷ�� �ް�, �޾ƿ� ������� playerinfo�� �����Ѵ�.
			PLAYERINFO tempplayerinfo = {};
			{
				ZeroMemory(buf, sizeof(buf));
				retval = recvn(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					cout << "���� - recvn - CS_PACKET_PLAYERINFO_INCHANGINGSCENE" << endl;
					break;
				}
				else
					memcpy(&tempplayerinfo, buf, sizeof(tempplayerinfo));
#ifdef DEBUG
				cout << id << "��° Ŭ���̾�Ʈ�� ���� �ٲ�����Ƿ� ���� ���� ��Ŷ�� �޾ƿԾ��!" << endl;
#endif
			}
			// 3. g_vecplayer[���� playerinfo�� id]�� �����Ͽ� ������ �����Ѵ�.
			{
				g_vecplayer[id] = tempplayerinfo;
			}
			// 4. �ٸ� Ŭ���̾�Ʈ���� g_vecplayer[0], g_vecplayer[1] ������ ������.
			{
				if (g_vecplayer.size() == 2) { // 2�� ���� �ÿ���.
					// ���� ����. 
					ZeroMemory(&packetinfo, sizeof(packetinfo));
					packetinfo.id = id;
					packetinfo.size = sizeof(PLAYERINFO);
					packetinfo.type = SC_PACKET_OTHER_PLAYERINFO;
					memcpy(buf, &packetinfo, sizeof(packetinfo));
					if (0 == id) {	// 0 ������ 1���� ������ �Ѵ�.
						retval = send(g_vecsocket[id + 1], buf, BUFSIZE, 0);
					}
					else if (1 == id) { // 1 ������ 0���� ������.
						retval = send(g_vecsocket[id - 1], buf, BUFSIZE, 0);
					}
					if (retval == SOCKET_ERROR) {
						cout << "failed : send - ���� - SC_PACKET_OTHER_PLAYERINFO" << endl;
						break;
					}
					else {
#ifdef DEBUG
						cout << id << "��° Ŭ���̾�Ʈ�� ���� �ٲ�����Ƿ� " << "�ٸ� Ŭ���̾�Ʈ���� ���� ���� ��Ŷ�� �����մϴ�!" << endl;
#endif
					}

					// ���� ����
					ZeroMemory(buf, sizeof(buf));
					memcpy(buf, &(g_vecplayer[id]), sizeof(playerinfo));
					if (0 == id) {	// 0 ������ 1���� ������ �Ѵ�.
						retval = send(g_vecsocket[id + 1], buf, BUFSIZE, 0);
					}
					else if (1 == id) {
						retval = send(g_vecsocket[id - 1], buf, BUFSIZE, 0);
					}
					if (retval == SOCKET_ERROR) {
						cout << "failed : send - ���� - SC_PACKET_OTHER_PLAYERINFO" << endl;
						break;
					}
					else {
#ifdef DEBUG
						cout << id << "��° Ŭ���̾�Ʈ�� ���� �ٲ�����Ƿ� " << "�ٸ� Ŭ���̾�Ʈ���� ���� ���� ��Ŷ�� �����մϴ�!" << endl;
#endif
					}

				}
			}
			LeaveCriticalSection(&cs);
		}
		break;
		case SC_PACKET_CLIENT_END:
		{
			// closesocket()
			closesocket(client_sock);
			cout << "[Ŭ���̾�Ʈ ���� ����] IP �ּ� (" << inet_ntoa(clientaddr.sin_addr) <<
				"), ��Ʈ ��ȣ (" << ntohs(clientaddr.sin_port) << ")" << endl;
			//CloseHandle(hThread[0]);
			//CloseHandle(hThread[1]);
			//SetEvent(hPlayerEvent);
			return 0;
		}
		break;
		case CS_PACKET_GRRENMUSH:
		{
			EnterCriticalSection(&cs);
			// -------------------Process---------------------
			int id = packetinfo.id;
			MONSTERINFO tempmonsterinfo = {};
			{
				ZeroMemory(buf, sizeof(buf));
				retval = recvn(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					cout << "���� - recvn - CS_PACKET_GRRENMUSH" << endl;
					break;
				}
				else
					memcpy(&tempmonsterinfo, buf, sizeof(tempmonsterinfo));
#ifdef DEBUG
				cout << id << "��° ���� Ŭ�󿡼� �޾ƿ�" << endl;
#endif
				{
					MONSTERINFO monsterinfo{};
					monsterinfo.id = id;
					monsterinfo.hp = g_vecgreen[id].hp;
					monsterinfo.money = g_vecgreen[id].money;
					monsterinfo.pt.x = g_vecgreen[id].pt.x;
					monsterinfo.pt.y = g_vecgreen[id].pt.y;
					monsterinfo.dir = g_vecgreen[id].dir;
					monsterinfo.pattern = g_vecgreen[id].pattern;
					monsterinfo.state = g_vecgreen[id].state;


					ZeroMemory(&packetinfo, sizeof(packetinfo));
					packetinfo.id = id;
					packetinfo.size = sizeof(MONSTERINFO);
					packetinfo.type = SC_PACKET_GRRENMUSH;

					ZeroMemory(buf, sizeof(buf));
					memcpy(buf, &packetinfo, sizeof(packetinfo));
					retval = send(client_sock, buf, BUFSIZE, 0);
					if (retval == SOCKET_ERROR) {
						err_display("send() - SC_PACKET_GRRENMUSH_INITIALLY");
						break;
					}
					retval = send(client_sock, (char*)&monsterinfo, sizeof(MONSTERINFO), 0);

					if (retval == SOCKET_ERROR) {
						err_display("send()");
						break;
					}
				}
			}

			LeaveCriticalSection(&cs);
		}
		break;
		case CS_PACKET_SKILL_CREATE:
		{
			EnterCriticalSection(&cs);
			// Ŭ���̾�Ʈ�κ��� ��ų�� �����Ǿ��ٴ� �޽����� �޾Ҵ�.
			// 1. ���� ������ ��ų�� skillinfo�� recv �� �´�.
			// 2. g_vecskill�� skillinfo�� push �Ѵ�.
			// 3. ���� ������ ��ų���� �ο��� id�� ���Ѵ�.
			// 4. �ٸ� Ŭ���̾�Ʈ���Ե� ��ų ���� �޽����� send �Ѵ�.

			// ---------------- Process -------------------
			// 0. �ʿ��� ������
			SKILLINFO skillinfo;
			int skillid;
			// 1. ���� ������ ��ų�� skillinfo�� recv �� �´�.
			{
				ZeroMemory(buf, sizeof(buf));
				retval = recvn(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("recvn() - CS_PACKET_SKILL_CREATE");
					break;
				}
				memcpy(&skillinfo, buf, sizeof(skillinfo));
			}
			// 2. g_vecskill�� skillinfo�� push �Ѵ�.
			{
				g_vecskill.push_back(skillinfo);
			}
			// 3. ���� ������ ��ų���� �ο��� id�� ���Ѵ�.
			{
				skillid = g_vecskill.size() - 1;
			}
			// 4. �ٸ� Ŭ���̾�Ʈ���Ե� ��ų ���� �޽����� send �Ѵ�.
			if(g_vecplayer.size() >= 2) {
				// ���� �ٸ� Ŭ���̾�Ʈ�� id��?
				int recvid{ -1 };
				if (packetinfo.id == 0) recvid = 1;
				else		recvid = 0;

				// ���� ����.
				ZeroMemory(&packetinfo, sizeof(PACKETINFO));
				packetinfo.id = skillid;
				packetinfo.size = sizeof(packetinfo);
				packetinfo.type = SC_PACKET_SKILL_CREATE;
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &packetinfo, sizeof(packetinfo));

				retval = send(g_vecsocket[recvid], buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() - SC_PACKET_SKILL_CREATE");
					break;
				}

				// ���� ����.
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &skillinfo, sizeof(skillinfo));
				retval = send(g_vecsocket[recvid], buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() - SC_PACKET_SKILL_CREATE");
					break;
				}
			}

			LeaveCriticalSection(&cs);
		}
		break;
		}
	}

	closesocket(client_sock);
	cout << "[Ŭ���̾�Ʈ ���� ����] IP �ּ� (" << inet_ntoa(clientaddr.sin_addr) <<
		"), ��Ʈ ��ȣ (" << ntohs(clientaddr.sin_port) << ")" << endl;

	return 0;
}

DWORD WINAPI CalculateThread(LPVOID arg)
{
	while (1)
	{
		GreenMushRoom_MoveInPattern();
	}
	return 0;
}
DWORD test_old_time = GetTickCount();

DWORD WINAPI MonsterThread(LPVOID)
{
	int retval{ 0 };


	PACKETINFO packetinfo;
	char buf[BUFSIZE];
	DWORD test_cur_time;
	while (1)
	{
		test_cur_time = GetTickCount();
		if (test_cur_time - test_old_time > 50)
		{
			EnterCriticalSection(&cs);
			if (test == true)
			{
				cout << "g_Vec������:" << g_vecsocket.size() << endl;
				for (int id = 0; id < MAX_GREEN; ++id)
				{
					MONSTERINFO monsterinfo{};
					monsterinfo.id = id;
					monsterinfo.hp = g_vecgreen[id].hp;
					monsterinfo.money = g_vecgreen[id].money;
					monsterinfo.pt.x = g_vecgreen[id].pt.x;
					monsterinfo.pt.y = g_vecgreen[id].pt.y;
					monsterinfo.dir = g_vecgreen[id].dir;
					monsterinfo.pattern = g_vecgreen[id].pattern;
					monsterinfo.state = g_vecgreen[id].state;

					ZeroMemory(&packetinfo, sizeof(packetinfo));
					packetinfo.id = id;
					packetinfo.size = sizeof(MONSTERINFO);
					packetinfo.type = SC_PACKET_GRRENMUSH;
					//���� ������ ������ 
					ZeroMemory(buf, sizeof(buf));
					memcpy(buf, &packetinfo, sizeof(packetinfo));
				
					retval = send(g_vecsocket[0], buf, BUFSIZE, 0);
					
					
					cout << "���� ���� �� ������" << endl;
					if (retval == SOCKET_ERROR) {
						err_display("send() - SC_PACKET_GRRENMUSH");
						break;
					}
					// ���������� ������ 
					
					{
						retval = send(g_vecsocket[0], (char*)&monsterinfo, sizeof(MONSTERINFO), 0);
					
					}
					if (g_vecplayer.size() >= 2 && g_vecsocket.size() >= 2)
					{
						retval = send(g_vecsocket[1], buf, BUFSIZE, 0);
						retval = send(g_vecsocket[1], (char*)&monsterinfo, sizeof(MONSTERINFO), 0);
					}
					if (retval == SOCKET_ERROR) {
						err_display("send()");
						break;
					}
				}
			}
			LeaveCriticalSection(&cs);
			test_old_time = GetTickCount();
		}
	}
	closesocket(g_vecsocket[0]);
	if(g_vecplayer.size() >= 2 && g_vecsocket.size() >= 2)
		closesocket(g_vecsocket[1]);
	return 0;
}

int main()
{	
	// �Ӱ� ���� �ʱ�ȭ
	InitializeCriticalSection(&cs);
	
	// ���� 2�� ����.
	g_vecplayer.reserve(MAX_USER);

	int retval;

	HANDLE hThread[3];

	// ���� ��ǥ �ʱ�ȭ
	InitializeMonsterInfo();
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

	int test_num{};
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
		cout << "����!:" << g_vecsocket.size() << endl;
		// ������ Ŭ���̾�Ʈ ���� ���
		cout << "[Ŭ���̾�Ʈ ����] IP �ּ� (" << inet_ntoa(clientaddr.sin_addr) <<
			"), ��Ʈ ��ȣ (" << ntohs(clientaddr.sin_port) << ")" << endl;

		// Ŭ���̾�Ʈ ������ ����
		hThread[0] = CreateThread(NULL, 0, ClientThread, (LPVOID)client_sock, 0, NULL);
		 if (hThread[0] == NULL)	
			 closesocket(client_sock);
		else					
			 CloseHandle(hThread[0]);
		 
		 //��� ������ ����
		 if (g_vecsocket.size() < 2)
		 {
			 cout << "������ ���� test" << endl;
			 hThread[1] = CreateThread(NULL, 0, CalculateThread, NULL, 0, NULL);
			 if (hThread[1] == NULL)
				 closesocket(client_sock);
			 else
				 CloseHandle(hThread[1]);

			// //���� ������ ����

			 hThread[2] = CreateThread(NULL, 0, MonsterThread, NULL, 0, NULL);
			 if (hThread[2] == NULL)
			 {
				 closesocket(client_sock);
			 }
			 else
				 CloseHandle(hThread[2]);
		 }

	}
	// �� ���� ������ ���� ���
	WaitForMultipleObjects(1, hThread, TRUE, INFINITE);

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

void GreenMushRoom_MoveInPattern()
{
	float m_fSpeed = 0.0001f;

	for (int i = 0; i < MAX_GREEN; ++i)
	{

		if (DIR_LEFT == g_vecgreen[i].dir)
		{
			switch (g_vecgreen[i].state)
			{
			case MONSTER_WALK:
			{
				g_vecgreen[i].pt.x -= m_fSpeed;

				if (g_vecgreen[i].pt.x < 465.f)
					g_vecgreen[i].dir = DIR_RIGHT;
			}
			break;
			case MONSTER_STAND:
				break;
			}

		}
		else
		{
			switch (g_vecgreen[i].state)
			{
			case MONSTER_WALK:
			{
				g_vecgreen[i].pt.x += m_fSpeed;
				if (g_vecgreen[i].pt.x > 1390.f)
					g_vecgreen[i].dir = DIR_LEFT;
			}
			break;
			case MONSTER_STAND:
				break;
			}

		}
	}
}

void InitializeMonsterInfo()
{
	// �ʷϺ��� �ʱ�ȭ
	for (int i = 0; i < MAX_GREEN; ++i)
	{
		MONSTERINFO monsterinfo{};
		monsterinfo.id = i;
		monsterinfo.hp = 100;
		monsterinfo.money = 10;
		monsterinfo.pt.x = greenposX[i];
		monsterinfo.pt.y = greenposY;
		monsterinfo.dir = greenDir[i];
		monsterinfo.pattern = greenPtr[i];
		monsterinfo.state = MONSTER_WALK;

		g_vecgreen[i] = monsterinfo;
	}

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