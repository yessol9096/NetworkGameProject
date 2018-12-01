#pragma once


DWORD WINAPI ClientThread(LPVOID);
DWORD WINAPI CalculateThread();

void InitializeNetwork();
void EndNetwork();

int recvn(SOCKET, char *, int, int);

// 초록버섯 이동패턴
// void GreenMushRoom_MoveInPattern();