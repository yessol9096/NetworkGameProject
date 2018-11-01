#include "StdAfx.h"
#include "PlayerExp.h"

CPlayerExp::CPlayerExp(void)
{
}

CPlayerExp::~CPlayerExp(void)
{
}

void CPlayerExp::Initialize(void)
{
	m_tInfo.fCX = 310.f;
	m_tInfo.fCY = 14.f;
	m_tInfo.fX = 404.f;
	m_tInfo.fY = 585.f;

	m_iExp = 0;
	m_iExpMax = 3000;

	m_pImgName = L"PlayerEXPBar";

	m_eRenderType = RENDER_OBJ;

}

int CPlayerExp::Update(void)
{
	m_iExp = m_pPlayer->GetState().iExp;
	m_iExpMax = m_pPlayer->GetState().iMaxExp;

	UpdateRect();

	return 0;
}

void CPlayerExp::Render(HDC hDc)
{
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;

	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left),
		static_cast<int>(m_tRect.top), 
		static_cast<int>(m_tInfo.fCX * ((float)g_iExp / (float)m_iExpMax)),
		static_cast<int>(m_tInfo.fCY),
		pBit->GetMemDC(),
		0,
		0,
		static_cast<int>(m_tInfo.fCX * ((float)g_iExp / (float)m_iExpMax)),
		static_cast<int>(m_tInfo.fCY),
		RGB(0, 0, 255));

}

void CPlayerExp::FrameMove()
{

}

void CPlayerExp::Release(void)
{

}
