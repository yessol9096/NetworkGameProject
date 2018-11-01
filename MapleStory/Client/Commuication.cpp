#include "StdAfx.h"
#include "Commuication.h"

CCommuication::CCommuication(void)
{
}

CCommuication::~CCommuication(void)
{
}

void CCommuication::SetNextButtonRect()
{
// 	m_tNextButton.left = m_tRect.right - 67.f;
// 	m_tNextButton.right = m_tRect.right - 28.f;
// 	m_tNextButton.bottom = m_tRect.bottom - 39.f;
// 	m_tNextButton.top = m_tRect.bottom - 55.f;

	m_tNextButton.left = static_cast<LONG>(m_tInfo.fX + 194.f);
	m_tNextButton.right = static_cast<LONG>(m_tInfo.fX + 233.f);
	m_tNextButton.bottom = static_cast<LONG>(m_tInfo.fY + 57.f);
	m_tNextButton.top = static_cast<LONG>(m_tInfo.fY + 43.f);
 
// 	m_tNextButton.left -= g_fScrollX;
// 	m_tNextButton.right -= g_fScrollX;
// 	m_tNextButton.bottom -= g_fScrollY;
// 	m_tNextButton.top -= g_fScrollY;



	
}
