#pragma once

extern HWND g_hWnd;
extern float g_fScrollX;
extern float g_fScrollY;
extern SCENE_TYPE g_eScene;
extern bool g_bIsSceneChange;

extern bool g_bIsSend;
extern bool g_bIsProgramEnd;

extern int g_iTakedMob1;
extern int g_iTakedMob2;
extern int g_iTakedMob1Max;
extern int g_iTakedMob2Max;

extern int g_iLevel;
extern int g_iExp;

// ���� �߰�
extern SOCKET g_sock;
extern wchar_t g_ipbuf[50]; // �ΰ��� �Է��ϴ� ip �ּ� ���� ����.
extern wchar_t g_nicknamebuf[50]; // ĳ���� ���� ������ �Է��ϴ� ip �ּ� ���� ����.
extern int g_retval;

extern PLAYERINFO g_myinfo;
extern int g_myid;
extern vector<PLAYERINFO> g_vecplayer;

// monster ���� �ܰ� ����
extern vector<MONSTERINFO> g_vecgreen;