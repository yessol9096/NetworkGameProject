#include "StdAfx.h"
#include "Rope.h"

CRope::CRope(void)
{
}

CRope::~CRope(void)
{
}

void CRope::Initialize(void)
{
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;
}

int CRope::Update(void)
{
	UpdateRect();

	return 0;
}

void CRope::Render(HDC hDc)
{
	if(GetAsyncKeyState('1'))
		Rectangle(hDc,static_cast<int>(m_tRect.left + g_fScrollX), 
		static_cast<int>(m_tRect.top+ g_fScrollY), 
		static_cast<int>(m_tRect.right+ g_fScrollX), 
		static_cast<int>(m_tRect.bottom+ g_fScrollY));
}

void CRope::Release(void)
{

}

void CRope::FrameMove()
{

}
