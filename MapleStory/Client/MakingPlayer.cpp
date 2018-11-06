#include "stdafx.h"
#include "MakingPlayer.h"
#include "Mouse.h"
#include <comdef.h>

CMakingPlayer::CMakingPlayer()
{
}


CMakingPlayer::~CMakingPlayer()
{
	Release();
}

void CMakingPlayer::Initialize()
{
	CBitmapMgr::GetInstance()->LoadImageByScene(SCENE_MAKINGPLAYER);
	
	m_pImgName = L"MakingPlayer1";
	//m_pImgName = L"MakingPlayer1";

		// 마우스
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMouse>::CreateObj(), OBJ_MOUSE);

	m_hDC = (CBitmapMgr::GetInstance()->GetMapBit()[L"BackBuffer"])->GetMemDC();
}

int CMakingPlayer::Update()
{
	CObjMgr::GetInstance()->UpdateObj();

	// 직업 바꾸는 화살표 - 마우스 충돌 검사.
	RECT rcRight, rcLeft;
	POINT pt;
	POINT pt2;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt2.x = static_cast<LONG>(pt.x - g_fScrollX) - 10 + static_cast<LONG>(g_fScrollX);
	pt2.y = static_cast<LONG>(pt.y - g_fScrollY) - 21 + static_cast<LONG>(g_fScrollY);

	rcRight.left = WINCX - 160; rcRight.right = WINCX - 120;
	rcRight.bottom = WINCY - 350; rcRight.top = WINCY - 380;
	rcLeft.left = WINCX - 260; rcLeft.right = WINCX - 230;
	rcLeft.bottom = WINCY - 350; rcLeft.top = WINCY - 380;

	// Debugging
	//Rectangle(m_hDC, rcRight.left, rcRight.top, rcRight.right, rcRight.bottom);
	//Rectangle(m_hDC, rcLeft.left, rcLeft.top, rcLeft.right, rcLeft.bottom);

	if (PtInRect(&rcRight, pt2))
	{
		if (KEYMGR->OnceKeyDown(VK_LBUTTON)) {
			if (sceneNum) // 1
				sceneNum = 0;
			else
				sceneNum = 1;
		}
	}
	if (PtInRect(&rcLeft, pt2))
	{
		if (KEYMGR->OnceKeyDown(VK_LBUTTON)) {
			if (sceneNum) // 1
				sceneNum = 0;
			else
				sceneNum = 1;
		}
	}

	switch (sceneNum) {
	case 0:
		m_pImgName = L"MakingPlayer1";
		m_initInfo.job = JOB_STRIKER;
		break;
	case 1:
		m_pImgName = L"MakingPlayer2";
		m_initInfo.job = JOB_CAPTIN;
		break;
	}
	
	// 생성 버튼 - 마우스 충돌 검사.
	RECT rcMake{ WINCX - 300, WINCY - 180, WINCX - 200, WINCY - 130 };

	// Debugging
	Rectangle(m_hDC, rcMake.left, rcMake.top, rcMake.right, rcMake.bottom);
	
	if (PtInRect(&rcMake, pt2)) {
		if (KEYMGR->OnceKeyDown(VK_LBUTTON)) {

			// 입력 받은 id를 InitInfo.id에 갱신한다
			//m_initInfo.id = (char)g_idbuf; // wcstombs()
			char* pStr; char str;
			int strSize = WideCharToMultiByte(CP_ACP, 0, g_idbuf, -1, NULL, 0, NULL, NULL);
			pStr = new char[strSize];
			WideCharToMultiByte(CP_ACP, 0, g_idbuf, -1, pStr, strSize, 0, 0);
			// InitInfo 구조체를 서버에 send 한다.
			for(int i = 0; i < strSize; ++i)
				strcpy(&(m_initInfo.id), &(pStr[i])); 

			CSceneMgr::GetInstance()->SetScene(SCENE_FIELD);
			CSoundMgr::GetInstance()->StopSoundAll();
			CSoundMgr::GetInstance()->PlaySound(L"Start.MP3", CSoundMgr::CHANNEL_EFFECT);
			CSoundMgr::GetInstance()->PlayBGM(L"BGM_Field.mp3");
		}
	}

	return 0;
}

void CMakingPlayer::Render(HDC hDc)
{
	CMyBmp* pBmp = CBitmapMgr::GetInstance()->FindImage(m_pImgName);


	if (NULL == pBmp)
		return;

	TransparentBlt(hDc, 0, 0, WINCX, WINCY,
		pBmp->GetMemDC(),
		0, 0,
		WINCX, WINCY, RGB(255, 255, 255));

	// 입력하는 id 출력.
	RECT rc = { WINCX - 240, WINCY - 313, WINCX - 100, WINCY - 293 };

	// Rectangle(m_hDC, rc.left, rc.top, rc.right, rc.bottom);

	DrawText(m_hDC, g_idbuf, wcslen(g_idbuf), &rc, DT_SINGLELINE);
}

void CMakingPlayer::Release()
{
}
 