#include "StdAfx.h"
#include "Line.h"

CLine::CLine(void)
{
}

CLine::CLine(LINEPOINT tLPoint, LINEPOINT tRPoint)
: m_tInfo(tLPoint, tRPoint)
{

}

CLine::~CLine(void)
{
}

void CLine::Render(HDC hDC)
{
	MoveToEx(hDC, static_cast<int>(m_tInfo.tLPoint.fX + g_fScrollX), static_cast<int>(m_tInfo.tLPoint.fY + g_fScrollY), NULL);
	LineTo(hDC, static_cast<int>(m_tInfo.tRPoint.fX + g_fScrollX), static_cast<int>(m_tInfo.tRPoint.fY + g_fScrollY));
}
