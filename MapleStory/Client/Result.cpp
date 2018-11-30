#include "stdafx.h"
#include "Result.h"

CResult::CResult()
	: m_bIsShowed(false)
{
}

CResult::~CResult()
{
}

void CResult::Initialize(void)
{
	m_tInfo.pt.x = 400.f;
	m_tInfo.pt.y = 200.f;
	m_tInfo.size.cx = 600.f;
	m_tInfo.size.cy = 250.f;

	m_pImgName = L"Result";

	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iScene = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.dwFrameSpd = 200;

	m_eRenderType = RENDER_OBJ;

	UpdateRect();

	m_tExitButton.left = 360.f;
	m_tExitButton.right = 445.f;
	m_tExitButton.bottom = 300.f;
	m_tExitButton.top = 270.f;
}

int CResult::Update(void)
{
	if (CKeyMgr::GetInstance()->OnceKeyUp('P'))	// 임시
		SetShowed(!m_bIsShowed);

	if (m_bIsShowed)
	{
		// 마우스가 들어오는지
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		if (PtInRect(&m_tExitButton, pt) &&
			CKeyMgr::GetInstance()->OnceKeyDown(VK_LBUTTON))
		{
			// 게임 종료
			m_bIsShowed = false;
		}
	}

	return 0;
}

void CResult::Render(HDC hDc)
{
	if (m_bIsShowed)
	{
		CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);
		if (NULL == pBit)  return;

		// 결과창
		TransparentBlt(hDc,
			static_cast<int>(m_tRect.left),
			static_cast<int>(m_tRect.top),
			static_cast<int>(m_tInfo.size.cx),
			static_cast<int>(m_tInfo.size.cy),
			pBit->GetMemDC(),
			static_cast<int>(m_tFrame.iFrameStart * m_tInfo.size.cx),
			static_cast<int>(m_tFrame.iScene * m_tInfo.size.cy),
			static_cast<int>(m_tInfo.size.cx),
			static_cast<int>(m_tInfo.size.cy),
			RGB(0, 255, 0));

		// 파티원 정보, 킬 관여율
		for (int i = 0; i < g_vecplayer.size(); ++i)
		{
			if (g_vecplayer[i].id == -1)	// 플레이어 벡터에 값이 없다면
				continue;

			int y = 0;
			if (g_vecplayer[0].attackAccValue > g_vecplayer[1].attackAccValue)
			{
				if (i == 0)	y = 190;
				else		y = 230;
			}
			else
			{
				if (i == 0)	y = 230;
				else		y = 190;
			}

			// name
			wchar_t name[512];
			mbstowcs(name, g_vecplayer[i].nickname, strlen(g_vecplayer[i].nickname) + 1);
			TextOut(hDc, 250, y, name, strlen(g_vecplayer[i].nickname));

			// 직업
			if (g_vecplayer[i].job == JOB_CAPTIN)	TextOut(hDc, 415, y, L"Captain", 7);
			else									TextOut(hDc, 415, y, L"Striker", 7);

			// 킬 관여율
			char bur[512];
			sprintf(bur, "%.1f", g_vecplayer[i].attackAccValue);

			wchar_t kill[512];
			mbstowcs(kill, bur, strlen(bur) + 1);

			SetTextColor(hDc, RGB(255, 0, 0));
			TextOut(hDc, 575, y, kill, strlen(bur));
			SetTextColor(hDc, RGB(0, 0, 0));
		}
	}
}

void CResult::SetShowed(bool bShow)
{
	m_bIsShowed = bShow;

	if (m_bIsShowed)
		CSoundMgr::GetInstance()->PlaySound(L"LevelUp.wav", CSoundMgr::CHANNEL_EFFECT);
}
