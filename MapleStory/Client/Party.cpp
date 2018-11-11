#include "stdafx.h"
#include "Party.h"

CParty::CParty()
	: m_bIsShowed(false)
{
}

CParty::~CParty()
{
}

void CParty::Initialize(void)
{
	m_tInfo.pt.x = WINCX * 0.8f;
	m_tInfo.pt.y = WINCY * 0.3f;
	m_tInfo.size.cx = 236.f;
	m_tInfo.size.cy = 342.f;

	UpdateRect();

	// 이미지
	m_pImgName = L"Party";

	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iScene = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.dwFrameSpd = 200;

	// 렌더 타입
	m_eRenderType = RENDER_OBJ;
}

int CParty::Update(void)
{
	CObj::UpdateRect();

	if (CKeyMgr::GetInstance()->OnceKeyUp('P'))
	{
		if (!m_bIsShowed)
			CSoundMgr::GetInstance()->PlaySound(L"OpenWnd.mp3", CSoundMgr::CHANNEL_EFFECT);
		else
			CSoundMgr::GetInstance()->PlaySound(L"CloseWnd.mp3", CSoundMgr::CHANNEL_EFFECT);

		m_bIsShowed = !m_bIsShowed;
	}

	return 0;
}

void CParty::Render(HDC hDc)
{
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);
	if (NULL == pBit)  return;

	if (m_bIsShowed)
	{
		// 파티창
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

		// 파티원 출력
	}
}