#include "StdAfx.h"
#include "PlayerHP.h"

CPlayerHP::CPlayerHP(void)
{
}

CPlayerHP::~CPlayerHP(void)
{
}

void CPlayerHP::Initialize(void)
{
	m_tInfo.fCX = 141.f;
	m_tInfo.fCY = 14.f;
	m_tInfo.fX = 320.f;
	m_tInfo.fY = 567.5f;

	m_iHp = 3000;
	m_iHpMax = 3000;

	m_pImgName = L"PlayerHPBar";

	m_eRenderType = RENDER_OBJ;
}

int CPlayerHP::Update(void)
{
	m_iHp = m_pPlayer->GetState().iHp;
	m_iHpMax = m_pPlayer->GetState().iMaxHp;

	UpdateRect();
	return 0;

}

void CPlayerHP::Render(HDC hDc)
{
	// HpBar
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;

	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left),
		static_cast<int>(m_tRect.top), 
		static_cast<int>(m_tInfo.fCX * ( (float)m_iHp / (float)m_iHpMax)),
		static_cast<int>(m_tInfo.fCY),
		pBit->GetMemDC(),
		0,
		0,
		static_cast<int>(m_tInfo.fCX * ( (float)m_iHp / (float)m_iHpMax)),
		static_cast<int>(m_tInfo.fCY),
		RGB(0, 0, 255));

}

void CPlayerHP::FrameMove()
{

}

void CPlayerHP::Release(void)
{

}
