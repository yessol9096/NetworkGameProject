#pragma once


DWORD WINAPI ClientThread(LPVOID);
DWORD WINAPI CalculateThread();

void InitializeNetwork();
void EndNetwork();
void DisconnectClient();	// Ŭ���̾�Ʈ�� �����Ͽ� ������ �������� ��.
void SendPacket();
void ProcessPacket();
void WorkerThread();
void AcceptThread();