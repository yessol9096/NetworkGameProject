#pragma once

void err_quit(const char*);
void err_display(const char*);

DWORD WINAPI ClientThread(LPVOID);
DWORD WINAPI CalculateThread();

void InitializeNetwork();
void EndNetwork();
void DisconnectClient();	// Ŭ���̾�Ʈ�� �����Ͽ� ������ �������� ��.
void SendPacket();
void ProcessPacket();
void WorkerThread();
void AcceptThread();