#pragma once


DWORD WINAPI ClientThread(LPVOID);
DWORD WINAPI CalculateThread(LPVOID);
//���� ������
DWORD WINAPI MonsterThread(LPVOID);

void InitializeNetwork();
void EndNetwork();

int recvn(SOCKET, char *, int, int);

// �ʷϹ��� �ʱ�ȭ
void InitializeMonsterInfo();
// �ʷϹ��� �̵�����
void GreenMushRoom_MoveInPattern();

DWORD Monster_Old_Movetime;
DWORD Monster_Cur_Movetime;