#include "StdAfx.h"
#include "Obj.h"

CObj::CObj(void)
: m_fSpeed(0.f), m_bIsDead(false), m_fAngle(0.f), m_fDist(0.f),
m_pTarget(NULL), m_eRenderType(RENDER_END)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
}

CObj::~CObj(void)
{
}

void CObj::UpdateRect()
{
	m_tRect.left = static_cast<LONG>(m_tInfo.fX - m_tInfo.fCX / 2);
	m_tRect.top = static_cast<LONG>(m_tInfo.fY - m_tInfo.fCY / 2);
	m_tRect.right = static_cast<LONG>(m_tInfo.fX + m_tInfo.fCX / 2);
	m_tRect.bottom = static_cast<LONG>(m_tInfo.fY + m_tInfo.fCY / 2);
}
