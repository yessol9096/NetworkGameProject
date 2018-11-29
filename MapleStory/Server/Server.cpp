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
int greenposX[] = {  HENESISCX * 0.5f,HENESISCX * 0.7f , HENESISCX * 0.6f ,HENESISCX * 0.7f,HENESISCX * 0.5f, HENESISCX * 0.4f };
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
	PACKETINFO packetinfo;

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	// -------------------------------------------
	// ���� �ٲٱ�. ���� ���� + ���� ����.
	while (true) {
		char buf[BUFSIZE];

		// ���� ���� ����.
		ZeroMemory(&packetinfo, sizeof(packetinfo));	 /// ���� �� �Ŵϱ�. ��� �����.		
		cout << "���� ���� ���� ��� ��" << endl;
		/// recvn���� ��� �޾ƿ;� ��. tcp Ư�� ������..!
		retval = recvn(client_sock, buf, BUFSIZE, 0);
		cout << "���� ���� ���ŵ�" << endl;
		/// �ޱ� ���� ��,
		if (retval == SOCKET_ERROR) {
			err_display("packetinfo recv()");
			break;
		}
		/// �ޱ� ���� ��,
		else {
			/// ���� ���� �����Ϳ� recv �ؿ� �޸𸮸�ŭ ���� �Ѵ�.
			memcpy(&packetinfo, buf, sizeof(packetinfo));
		}


		// ���� ���� ����.
		switch (packetinfo.type) {
		case CS_PACKET_PLAYERINFO_INITIALLY: // �ʱ� �÷��̾� info.
		{
			char buf2[BUFSIZE];

			// -----------------------------------------
			// Ŭ���̾�Ʈ�� �ʱ� ���� �� playerinfo�� recvn �Ѵ�.
			{
				ZeroMemory(&buf2, sizeof(buf2));	 /// ���� �� �Ŵϱ�. ��� �����.		
				retval = recvn(client_sock, buf2, BUFSIZE, 0);
				cout << "���� ���� ���� - CS_PACKET_PLAYERINFO_INITIALLY" << endl;
				if (retval == SOCKET_ERROR) {
					err_display("intial playerinfo recv()");
					break;
				}
				else {
					/// ���� ���� �����Ϳ� recv �ؿ� �޸𸮸�ŭ ���� �Ѵ�.
					memcpy(&playerinfo, buf2, sizeof(playerinfo));
				}

#ifdef DEBUGGING
				cout << "[TCP" << inet_ntoa(clientaddr.sin_addr) << " : " << ntohs(clientaddr.sin_port) << "]";
				if (playerinfo.job == JOB_CAPTIN)
					cout << "[RECV]" << "PlayerInfo - �г��� : " << playerinfo.nickname << ", ���� : ĸƾ" << endl;
				else
					cout << "[RECV]" << "PlayerInfo - �г��� : " << playerinfo.nickname << ", ���� : ��Ʈ����Ŀ" << endl;
#endif
				// �ʱ� ���� �� playerinfo�� ������� vector�� push �Ѵ�.
				int id = -1;
				for (int i = 0; i < MAX_USER; ++i) {
					if (false == g_arrayconnected[i]) { /// ���������� �����ؼ� ���� ���� ���� �ε��� i�� ã��, 
						id = i; /// ���̵� �ο��Ѵ�.
						break;
					}


					/// ���� 2�� ����.
					if (-1 == id) {
						std::cout << "user�� �� á���ϴ�." << std::endl;
						closesocket(client_sock);
						break;
					}
					// ���� playerinfo�� id�� �ο��Ѵ�.
					playerinfo.id = id;
					playerinfo.connected = true;
					playerinfo.pt.x = 100.f; // �ʱ� ��ǥ�� (100, 500)
					playerinfo.pt.y = 500.f;
					playerinfo.hp = 30000;
					playerinfo.size.cx = 100.f;
					playerinfo.size.cy = 100.f;
				}
				// ������ �� ä���� playerinfo�� g_vecplayer�� ��´�.
				g_vecplayer.push_back(playerinfo);
			}
			// -------------------------------------------
			// id �� �����Ѵ�.
			/// ���� ���� ������ ����,,
			ZeroMemory(&packetinfo, sizeof(packetinfo)); /// packetinfo ����.
			packetinfo.type = SC_PACKET_ID_INITIALLY;
			packetinfo.size = sizeof(playerinfo.id);
			ZeroMemory(buf, sizeof(buf)); /// ���� ����.
			memcpy(buf, &packetinfo, sizeof(packetinfo));
			retval == send(client_sock, buf, BUFSIZE, 0);
			cout << "���� ���� ���� - SC_PACKET_PLAYERINFO_ID" << endl;
			if (retval == SOCKET_ERROR) {
				err_display("send() - SC_PACKET_PLAYERINFO_ID");
			}
			/// ���� ���� ������ ����,,
			ZeroMemory(buf, sizeof(buf));
			memcpy(buf, &(playerinfo.id), sizeof(playerinfo.id));
			retval == send(client_sock, buf, BUFSIZE, 0);
			cout << "���� ���� ���� - SC_PACKET_PLAYERINFO_ID" << endl;
			if (retval == SOCKET_ERROR)
				err_display("send() - SC_PACKET_PLAYERINFO_ID");
			// -------------------------------------------
			// ���� ������ Ŭ���̾�Ʈ����, ���� ������ ��� �÷��̾��� ��ġ�� ����ȭ�Ѵ�.
			// me, other playerinfo ����.
			else {
				for (int i = 0; i < g_vecplayer.size(); ++i) {
					/// ���� ���� ������ ����,,
					ZeroMemory(&packetinfo, sizeof(packetinfo)); /// packetinfo ����.
					packetinfo.type = SC_PACKET_PLAYERINFO;
					packetinfo.size = sizeof(g_vecplayer[i]);
					packetinfo.id = i;
					ZeroMemory(buf, sizeof(buf)); /// ���� ����.
					memcpy(buf, &packetinfo, sizeof(packetinfo));
					retval == send(client_sock, buf, BUFSIZE, 0);
					cout << "���� ���� ���� - SC_PACKET_PLAYERINFO - " << i << "��° Ŭ���̾�Ʈ" << endl;
					if (retval == SOCKET_ERROR) {
						err_display("send() - SC_PACKET_PLAYERINFO");
					}
					/// ���� ���� ������ ����,,
					ZeroMemory(buf, sizeof(buf)); /// ���� ����.
					memcpy(buf, &(g_vecplayer[i]), sizeof(g_vecplayer[i]));
					retval == send(client_sock, buf, BUFSIZE, 0);
					cout << "���� ���� ���� - SC_PACKET_PLAYERINFO - " << i << "��° Ŭ���̾�Ʈ" << endl;
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

