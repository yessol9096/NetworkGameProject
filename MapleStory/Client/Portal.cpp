#include "StdAfx.h"
#include "Portal.h"

CPortal::CPortal(void)
{
}

CPortal::~CPortal(void)
{
}

void CPortal::Initialize(void)
{
	m_tInfo.fCX = 89.f;
	m_tInfo.fCY = 257.f;
	
	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_pImgName = L"Portal";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iScene = 0;
	m_tFrame.dwFrameSpd = 150;

	m_eRenderType = RENDER_OBJ;

	UpdateCollRect();
}

int CPortal::Update(void)
{

	UpdateRect();
	FrameMove();
	return 0;

}

void CPortal::Render(HDC hDc)
{
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit) return;

	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left + g_fScrollX),
		static_cast<int>(m_tRect.top + g_fScrollY), 
		static_cast<int>(m_tInfo.fCX),
		static_cast<int>(m_tInfo.fCY),
		pBit->GetMemDC(),
		static_cast<int>(m_tFrame.iFrameStart * m_tInfo.fCX),
		static_cast<int>(m_tFrame.iScene * m_tInfo.fCY),
		static_cast<int>(m_tInfo.fCX),
		static_cast<int>(m_tInfo.fCY),
		RGB(255, 255, 255));

// 	if(GetAsyncKeyState('2'))
// 		Rectangle(hDc, m_tCollRect.left + g_fScrollX, 
// 		m_tCollRect.top + g_fScrollY,
// 		m_tCollRect.right + g_fScrollX, 
// 		m_tCollRect.bottom + g_fScrollY);
			

}

void CPortal::FrameMove()
{

	m_dwFrameCurTime = GetTickCount();

	if(m_dwFrameOldTime + m_tFrame.dwFrameSpd < m_dwFrameCurTime)
	{
		++(m_tFrame.iFrameStart);
		m_dwFrameOldTime = m_dwFrameCurTime;
	}

	if(m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		m_tFrame.iFrameStart = 0;
}

void CPortal::Release(void)
{

}

void CPortal::UpdateCollRect()
{
	m_tRect = m_tCollRect;

	m_tCollRect.top = static_cast<LONG>(m_tRect.top - 140.f);

}
