#include "StdAfx.h"
#include "BossHPBar.h"

BossHPBar::BossHPBar(void)
{
}

BossHPBar::~BossHPBar(void)
{
}

void BossHPBar::Initialize(void)
{
	m_tInfo.size.cx = 597.f;
	m_tInfo.size.cy = 28.f;
	m_tInfo.pt.x = 415.f;
	m_tInfo.pt.y = 40.f;

	m_iBossHp = 70000;
	m_iBossHpMax = 70000;

	m_pImgName = L"BossHP";

	// 뒤에 HP바
	m_tBarInfo.size.cx = 634.f;
	m_tBarInfo.size.cy = 35.f;
	m_tBarInfo.pt.x = 400.f;
	m_tBarInfo.pt.y = 40.f;

	m_tBarRect.left = static_cast<LONG>(m_tBarInfo.pt.x - m_tBarInfo.size.cx / 2);
	m_tBarRect.top = static_cast<LONG>(m_tBarInfo.pt.y - m_tBarInfo.size.cy / 2);
	m_tBarRect.right = static_cast<LONG>(m_tBarInfo.pt.x + m_tBarInfo.size.cx / 2);
	m_tBarRect.bottom = static_cast<LONG>(m_tBarInfo.pt.y + m_tBarInfo.size.cy / 2);
	//

	m_eRenderType = RENDER_OBJ;
}

int BossHPBar::Update(void)
{
	m_iBossHp = m_pBoss->GetState().iHp;

	UpdateRect();
	return 0;
}

void BossHPBar::Render(HDC hDc)
{
	// HPBar 뒤에꺼
	CMyBmp* pBit2 = CBitmapMgr::GetInstance()->FindImage(L"BossHPBar");

	if(NULL == pBit2)  return;

	TransparentBlt(hDc,
		static_cast<int>(m_tBarRect.left ),
		static_cast<int>(m_tBarRect.top ), 
		static_cast<int>(m_tBarInfo.size.cx),
		static_cast<int>(m_tBarInfo.size.cy),
		pBit2->GetMemDC(),
		0,
		0,
		static_cast<int>(m_tBarInfo.size.cx),
		static_cast<int>(m_tBarInfo.size.cy),
		RGB(0, 0, 255));

	// HpBar
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;

	float fHpBarCX = m_tInfo.size.cx * ((float)m_iBossHp / (float)m_iBossHpMax);
	

	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left),
		static_cast<int>(m_tRect.top), 
		static_cast<int>(fHpBarCX),
		static_cast<int>(m_tInfo.size.cy),
		pBit->GetMemDC(),
		0,
		0,
		static_cast<int>(fHpBarCX),
		static_cast<int>(m_tInfo.size.cy),
		RGB(0, 0, 255));

}

void BossHPBar::FrameMove()
{

}

void BossHPBar::Release(void)
{

}
