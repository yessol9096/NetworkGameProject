#pragma once


DWORD WINAPI ClientThread(LPVOID);
DWORD WINAPI CalculateThread();

void InitializeNetwork();
void EndNetwork();

int recvn(SOCKET, char *, int, int);

// �ʷϹ��� �̵�����
// void GreenMushRoom_MoveInPattern();