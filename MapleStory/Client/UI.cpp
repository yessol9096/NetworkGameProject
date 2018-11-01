#include "StdAfx.h"
#include "UI.h"

CUI::CUI(void)
{
}

CUI::~CUI(void)
{
}

void CUI::Initialize(void)
{
	m_tInfo.fCX = WINCX;
	m_tInfo.fCY = WINCY;
	m_tInfo.fX = WINCX * 0.5f;
	m_tInfo.fY = WINCY * 0.5f;

	m_pImgName = L"UI";

	m_eRenderType = RENDER_OBJ;


	// 레벨 (십의자리)
	m_tTensLvInfo.fX = 42.f;
	m_tTensLvInfo.fY = 575.f;
	m_tTensLvInfo.fCX = 10.f;
	m_tTensLvInfo.fCY = 12.f;

	m_tTensLvRect.top = static_cast<LONG>(m_tTensLvInfo.fY - m_tTensLvInfo.fCY * 0.5f);
	m_tTensLvRect.bottom = static_cast<LONG>(m_tTensLvInfo.fY + m_tTensLvInfo.fCY * 0.5f);
	m_tTensLvRect.left = static_cast<LONG>(m_tTensLvInfo.fX - m_tTensLvInfo.fCX * 0.5f);
	m_tTensLvRect.right = static_cast<LONG>(m_tTensLvInfo.fX + m_tTensLvInfo.fCX * 0.5f);

	m_tTensFrame.iFrameStart = 8;
	m_tTensFrame.iFrameEnd = 8;


	// 레벨 (일의 자리)
	m_tUnitLvInfo.fX = 54.f;
	m_tUnitLvInfo.fY = 575.f;
	m_tUnitLvInfo.fCX = 10.f;
	m_tUnitLvInfo.fCY = 12.f;

	m_tUnitLvRect.top = static_cast<LONG>(m_tUnitLvInfo.fY - m_tUnitLvInfo.fCY * 0.5f);
	m_tUnitLvRect.bottom = static_cast<LONG>(m_tUnitLvInfo.fY + m_tUnitLvInfo.fCY * 0.5f);
	m_tUnitLvRect.left = static_cast<LONG>(m_tUnitLvInfo.fX - m_tUnitLvInfo.fCX * 0.5f);
	m_tUnitLvRect.right = static_cast<LONG>(m_tUnitLvInfo.fX + m_tUnitLvInfo.fCX * 0.5f);

	m_tUnitFrame.iFrameStart = 8;
	m_tUnitFrame.iFrameEnd = 8;

	m_iTensLevel = static_cast<int>(g_iLevel * 0.1);
	int iTensNumber = static_cast<int>((g_iLevel * 0.1));
	m_iUnitLevel = g_iLevel - (iTensNumber) * 10;

}

int CUI::Update(void)
{
//	FrameMove();

	// 레벨
	m_iTensLevel = static_cast<int>(g_iLevel * 0.1);
	int iTensNumber = static_cast<int>((g_iLevel * 0.1));
	m_iUnitLevel = g_iLevel - (iTensNumber) * 10;

	m_tUnitFrame.iFrameStart = m_iUnitLevel;
	m_tTensFrame.iFrameStart = m_iTensLevel;


	UpdateRect();


	return 0;
}

void CUI::Render(HDC hDc)
{
	// 메인 UI
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;

	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left ),
		static_cast<int>(m_tRect.top ), 
		static_cast<int>(m_tInfo.fCX),
		static_cast<int>(m_tInfo.fCY),
		pBit->GetMemDC(),
		0,
		0,
		static_cast<int>(m_tInfo.fCX),
		static_cast<int>(m_tInfo.fCY),
		RGB(0, 0, 255));



	// 레벨
	CMyBmp* pTensBit = CBitmapMgr::GetInstance()->FindImage(L"LvNum");
	if(NULL == pTensBit)  return;
	TransparentBlt(hDc,
		static_cast<int>(m_tTensLvRect.left ),
		static_cast<int>(m_tTensLvRect.top ), 
		static_cast<int>(m_tTensLvInfo.fCX),
		static_cast<int>(m_tTensLvInfo.fCY),
		pTensBit->GetMemDC(),
		static_cast<int>(m_tTensLvInfo.fCX * m_tTensFrame.iFrameStart),
		0,
		static_cast<int>(m_tTensLvInfo.fCX),
		static_cast<int>(m_tTensLvInfo.fCY),
		RGB(0, 0, 0));


	CMyBmp* pUnitBit = CBitmapMgr::GetInstance()->FindImage(L"LvNum");
	if(NULL == pUnitBit)  return;
	TransparentBlt(hDc,
		static_cast<int>(m_tUnitLvRect.left ),
		static_cast<int>(m_tUnitLvRect.top ), 
		static_cast<int>(m_tUnitLvInfo.fCX),
		static_cast<int>(m_tUnitLvInfo.fCY),
		pUnitBit->GetMemDC(),
		static_cast<int>(m_tUnitLvInfo.fCX * m_tUnitFrame.iFrameStart),
		0,
		static_cast<int>(m_tUnitLvInfo.fCX),
		static_cast<int>(m_tUnitLvInfo.fCY),
		RGB(0, 0, 0));

}

void CUI::FrameMove()
{

}

void CUI::Release(void)
{

}
