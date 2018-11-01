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
	m_tInfo.fCX = 597.f;
	m_tInfo.fCY = 28.f;
	m_tInfo.fX = 415.f;
	m_tInfo.fY = 40.f;

	m_iBossHp = 70000;
	m_iBossHpMax = 70000;

	m_pImgName = L"BossHP";

	// 뒤에 HP바
	m_tBarInfo.fCX = 634.f;
	m_tBarInfo.fCY = 35.f;
	m_tBarInfo.fX = 400.f;
	m_tBarInfo.fY = 40.f;

	m_tBarRect.left = static_cast<LONG>(m_tBarInfo.fX - m_tBarInfo.fCX / 2);
	m_tBarRect.top = static_cast<LONG>(m_tBarInfo.fY - m_tBarInfo.fCY / 2);
	m_tBarRect.right = static_cast<LONG>(m_tBarInfo.fX + m_tBarInfo.fCX / 2);
	m_tBarRect.bottom = static_cast<LONG>(m_tBarInfo.fY + m_tBarInfo.fCY / 2);
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
		static_cast<int>(m_tBarInfo.fCX),
		static_cast<int>(m_tBarInfo.fCY),
		pBit2->GetMemDC(),
		0,
		0,
		static_cast<int>(m_tBarInfo.fCX),
		static_cast<int>(m_tBarInfo.fCY),
		RGB(0, 0, 255));

	// HpBar
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;

	float fHpBarCX = m_tInfo.fCX * ((float)m_iBossHp / (float)m_iBossHpMax);
	

	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left),
		static_cast<int>(m_tRect.top), 
		static_cast<int>(fHpBarCX),
		static_cast<int>(m_tInfo.fCY),
		pBit->GetMemDC(),
		0,
		0,
		static_cast<int>(fHpBarCX),
		static_cast<int>(m_tInfo.fCY),
		RGB(0, 0, 255));

}

void BossHPBar::FrameMove()
{

}

void BossHPBar::Release(void)
{

}
