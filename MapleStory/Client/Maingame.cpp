#include "StdAfx.h"
#include "Maingame.h"


float g_fScrollX = 0.f;
float g_fScrollY = 0.f;
bool  g_bIsSceneChange = false;
SCENE_TYPE g_eScene = SCENE_FIELD;
PLAYERINFO g_myinfo;
int g_retval;

// 서버와 통신 소켓
SOCKET g_sock;
// 플레이어들의 정보를 담는 벡터.
vector<PLAYERINFO> g_vecplayer;

//몬스터 정보 받기 
vector<MONSTERINFO> g_vecgreen(MAX_GREEN);
bool bMonster_check = false;
MONSTERINFO monsterinfo{};

//데이터타입 구분하기
int datatype;

void CMaingame::Initialize(void)
{	
	m_hDC = GetDC(g_hWnd);
	//콘솔창 디버깅용
	AllocConsole();

	freopen("CONOUT$", "wt", stdout);
	//
	// 관련 이미지 로드
	CBitmapMgr::GetInstance()->GetMapBit().insert(make_pair(
		L"Back", (new CMyBmp)->LoadBmp(L"../Image/Back.bmp")));

	CBitmapMgr::GetInstance()->GetMapBit().insert(make_pair(
		L"BackBuffer", (new CMyBmp)->LoadBmp(L"../Image/BackBuffer.bmp")));

	// 씬 바꾸기 (Logo로)
	CSceneMgr::GetInstance()->SetScene(SCENE_LOGO);

	CollisionMgr::InitWELLRNG512((unsigned long)time(NULL));

	//-------------------------------------------------------------------------------
	// 서버 추가.

	// 윈속을 초기화 한다.
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// 소켓 생성.
	g_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_sock == INVALID_SOCKET) // 생성 실패시
		MessageBoxW(g_hWnd, L"socket()", MB_OK, MB_OK);

}

void CMaingame::Update(void)
{
	CSceneMgr::GetInstance()->Update();
	CSoundMgr::GetInstance()->UpdateSound();

	if (g_retval != SOCKET_ERROR)
	{
		g_retval = recv(g_sock, (char*)&datatype, sizeof(int), 0);
		//cout << datatype << endl;
		switch (datatype)
		{
		case OBJ_PLAYER:
			break;
		case OBJ_GRRENMUSH:
			g_retval = recv(g_sock, (char*)&monsterinfo, sizeof(monsterinfo), 0);
			if(g_vecgreen[monsterinfo.id].key == NULL)
				g_vecgreen[monsterinfo.id] = monsterinfo;
			//cout << g_vecgreen[5].pt.x << endl;
			break;
		}
	}
}

void CMaingame::Render(void)
{
	// 더블 버퍼링
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
