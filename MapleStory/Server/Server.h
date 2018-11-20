#pragma once


DWORD WINAPI ClientThread(LPVOID);
DWORD WINAPI CalculateThread();

void InitializeNetwork();
void EndNetwork();
void DisconnectClient();	// 클라이언트가 종료하여 접속이 끊어졌을 때.
void SendPacket();
void ProcessPacket();
void WorkerThread();
void AcceptThread();

int recvn(SOCKET, char *, int, int);

// 초록버섯 이동패턴
// void GreenMushRoom_MoveInPattern();