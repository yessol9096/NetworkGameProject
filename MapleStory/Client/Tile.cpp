#include "StdAfx.h"
#include "Tile.h"

CTile::CTile(void)
: m_iDrawID(0), m_iOption(0)
{
}

CTile::~CTile(void)
{
	Release();
}

void CTile::Initialize(void)
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;
}

int CTile::Update(void)
{
	return 0;
}

void CTile::Render(HDC hDc)
{
	UpdateRect();

	CMyBmp* pBmp = CBitmapMgr::GetInstance()->FindImage(L"Tile");

	if(NULL == pBmp)
		return;

	TransparentBlt(hDc, static_cast<int>(m_tRect.left + g_fScrollX), 
		static_cast<int>(m_tRect.top + g_fScrollY)
		, static_cast<int>(m_tInfo.fCX), 
		static_cast<int>(m_tInfo.fCY)
		, pBmp->GetMemDC(),static_cast<int>( m_iDrawID * m_tInfo.fCX), 0
		, static_cast<int>(m_tInfo.fCX), 
		static_cast<int>(m_tInfo.fCY), RGB(0, 255, 0));
}

void CTile::Release(void)
{

}

void CTile::FrameMove()
{

}
