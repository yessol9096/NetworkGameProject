#include "stdafx.h"
#include "MakingPlayer.h"
#include "Mouse.h"


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

		// ¸¶¿ì½º
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMouse>::CreateObj(), OBJ_MOUSE);

	m_hDC = GetDC(g_hWnd);
}

int CMakingPlayer::Update()
{
	CObjMgr::GetInstance()->UpdateObj();

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
		break;
	case 1:
		m_pImgName = L"MakingPlayer2";
		break;
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

}

void CMakingPlayer::Release()
{
}
 