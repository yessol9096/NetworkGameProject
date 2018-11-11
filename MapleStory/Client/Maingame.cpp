#include "StdAfx.h"
#include "Maingame.h"

float g_fScrollX = 0.f;
float g_fScrollY = 0.f;
bool  g_bIsSceneChange = false;
SCENE_TYPE g_eScene = SCENE_FIELD;
PLAYERINFO g_myinfo;
int g_retval;

// ������ ��� ����
SOCKET g_sock;

void CMaingame::Initialize(void)
{	
	m_hDC = GetDC(g_hWnd);

	// ���� �̹��� �ε�
	CBitmapMgr::GetInstance()->GetMapBit().insert(make_pair(
		L"Back", (new CMyBmp)->LoadBmp(L"../Image/Back.bmp")));

	CBitmapMgr::GetInstance()->GetMapBit().insert(make_pair(
		L"BackBuffer", (new CMyBmp)->LoadBmp(L"../Image/BackBuffer.bmp")));

	// �� �ٲٱ� (Logo��)
	CSceneMgr::GetInstance()->SetScene(SCENE_LOGO);

	CollisionMgr::InitWELLRNG512((unsigned long)time(NULL));

	//-------------------------------------------------------------------------------
	// ���� �߰�.

	// ������ �ʱ�ȭ �Ѵ�.
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// ���� ����.
	g_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_sock == INVALID_SOCKET) // ���� ���н�
		MessageBoxW(g_hWnd, L"socket()", MB_OK, MB_OK);
}

void CMaingame::Update(void)
{
	CSceneMgr::GetInstance()->Update();
	CSoundMgr::GetInstance()->UpdateSound();
}

void CMaingame::Render(void)
{
	// ���� ���۸�
	HDC hBackBuff = (CBitmapMgr::GetInstance()->GetMapBit()[L"BackBuffer"])->GetMemDC();
	HDC hMemDC = (CBitmapMgr::GetInstance()->GetMapBit()[L"Back"])->GetMemDC();

	BitBlt(hBackBuff, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CSceneMgr::GetInstance()->Render(hBackBuff);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackBuff, 0, 0, SRCCOPY);

}

void CMaingame::Release(void)
{
	CKeyMgr::GetInstance()->DestroyInstance();
	CBitmapMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	ReleaseDC(g_hWnd, m_hDC);
}

CMaingame::CMaingame(void)
{
}

CMaingame::~CMaingame(void)
{
	Release();
}
