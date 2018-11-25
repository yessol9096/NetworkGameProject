#include "StdAfx.h"
#include "Font.h"

CFont::CFont(void)
{
}

CFont::~CFont(void)
{
}

void CFont::Initialize(void)
{
	Calculating();

	SettingPos();

	m_tInfo.size.cx = 149.f;
	m_tInfo.size.cy = 94.f;

	m_eRenderType = RENDER_OBJ;

	m_dwLifeCurTime = GetTickCount();
	m_dwLifeOldTime = GetTickCount();
}

int CFont::Update(void)
{
	m_dwLifeCurTime = GetTickCount();

	// 2�ʰ� ������ �������.
	if (m_dwLifeOldTime + 1000 < m_dwLifeCurTime)
		return 1;

	m_tInfo.size.cx = 149.f;
	m_tInfo.size.cy = 94.f;

	// ���� ���ڵ��� ����Ͽ� ���� ��ġ ����
	Calculating();

	// ���� ��ġ ������ ����
	SettingPos();

	// ��Ʈ Ÿ�Կ� ���� ���� Info�� Rect�� ������ ������ ��ġ �ֱ�
	if (FONTTYPE_TENS == m_eType) // ���ڸ�
	{
		m_tUnitInfo.pt.x = m_tPosArr[0].pt.x;
		m_tUnitInfo.pt.y = m_tPosArr[0].pt.y;
		m_tUnitRect.left = static_cast<LONG>(m_tPosArr[0].pt.x - m_tInfo.size.cx * 0.5f);
		m_tUnitRect.right = static_cast<LONG>(m_tPosArr[0].pt.x + m_tInfo.size.cx * 0.5f);
		m_tUnitRect.top = static_cast<LONG>(m_tPosArr[0].pt.y - m_tInfo.size.cy * 0.5f);
		m_tUnitRect.bottom = static_cast<LONG>(m_tPosArr[0].pt.y + m_tInfo.size.cy * 0.5f);

		m_tTensInfo.pt.x = m_tPosArr[1].pt.x;
		m_tTensInfo.pt.y = m_tPosArr[1].pt.y;
		m_tTensRect.left = static_cast<LONG>(m_tPosArr[1].pt.x - m_tInfo.size.cx * 0.5f);
		m_tTensRect.right = static_cast<LONG>(m_tPosArr[1].pt.x + m_tInfo.size.cx * 0.5f);
		m_tTensRect.top = static_cast<LONG>(m_tPosArr[1].pt.y - m_tInfo.size.cy * 0.5f);
		m_tTensRect.bottom = static_cast<LONG>(m_tPosArr[1].pt.y + m_tInfo.size.cy * 0.5f);
	}
	else if (FONTTYPE_HUND == m_eType) // ���ڸ�
	{
		m_tUnitInfo.pt.x = m_tPosArr[0].pt.x;
		m_tUnitInfo.pt.y = m_tPosArr[0].pt.y;
		m_tUnitRect.left = static_cast<LONG>(m_tPosArr[0].pt.x - m_tInfo.size.cx * 0.5f);
		m_tUnitRect.right = static_cast<LONG>(m_tPosArr[0].pt.x + m_tInfo.size.cx * 0.5f);
		m_tUnitRect.top = static_cast<LONG>(m_tPosArr[0].pt.y - m_tInfo.size.cy * 0.5f);
		m_tUnitRect.bottom = static_cast<LONG>(m_tPosArr[0].pt.y + m_tInfo.size.cy * 0.5f);

		m_tTensInfo.pt.x = m_tPosArr[1].pt.x;
		m_tTensInfo.pt.y = m_tPosArr[1].pt.y;
		m_tTensRect.left = static_cast<LONG>(m_tPosArr[1].pt.x - m_tInfo.size.cx * 0.5f);
		m_tTensRect.right = static_cast<LONG>(m_tPosArr[1].pt.x + m_tInfo.size.cx * 0.5f);
		m_tTensRect.top = static_cast<LONG>(m_tPosArr[1].pt.y - m_tInfo.size.cy * 0.5f);
		m_tTensRect.bottom = static_cast<LONG>(m_tPosArr[1].pt.y + m_tInfo.size.cy * 0.5f);

		m_tHundInfo.pt.x = m_tPosArr[2].pt.x;
		m_tHundInfo.pt.y = m_tPosArr[2].pt.y;
		m_tHundRect.left = static_cast<LONG>(m_tPosArr[2].pt.x - m_tInfo.size.cx * 0.5f);
		m_tHundRect.right = static_cast<LONG>(m_tPosArr[2].pt.x + m_tInfo.size.cx * 0.5f);
		m_tHundRect.top = static_cast<LONG>(m_tPosArr[2].pt.y - m_tInfo.size.cy * 0.5f);
		m_tHundRect.bottom = static_cast<LONG>(m_tPosArr[2].pt.y + m_tInfo.size.cy * 0.5f);
	}
	else if (FONTTYPE_THOU == m_eType) // ���ڸ�
	{
		m_tUnitInfo.pt.x = m_tPosArr[0].pt.x;
		m_tUnitInfo.pt.y = m_tPosArr[0].pt.y;
		m_tUnitRect.left = static_cast<LONG>(m_tPosArr[0].pt.x - m_tInfo.size.cx * 0.5f);
		m_tUnitRect.right = static_cast<LONG>(m_tPosArr[0].pt.x + m_tInfo.size.cx * 0.5f);
		m_tUnitRect.top = static_cast<LONG>(m_tPosArr[0].pt.y - m_tInfo.size.cy * 0.5f);
		m_tUnitRect.bottom = static_cast<LONG>(m_tPosArr[0].pt.y + m_tInfo.size.cy * 0.5f);

		m_tTensInfo.pt.x = m_tPosArr[1].pt.x;
		m_tTensInfo.pt.y = m_tPosArr[1].pt.y;
		m_tTensRect.left = static_cast<LONG>(m_tPosArr[1].pt.x - m_tInfo.size.cx * 0.5f);
		m_tTensRect.right = static_cast<LONG>(m_tPosArr[1].pt.x + m_tInfo.size.cx * 0.5f);
		m_tTensRect.top = static_cast<LONG>(m_tPosArr[1].pt.y - m_tInfo.size.cy * 0.5f);
		m_tTensRect.bottom = static_cast<LONG>(m_tPosArr[1].pt.y + m_tInfo.size.cy * 0.5f);

		m_tHundInfo.pt.x = m_tPosArr[2].pt.x;
		m_tHundInfo.pt.y = m_tPosArr[2].pt.y;
		m_tHundRect.left = static_cast<LONG>(m_tPosArr[2].pt.x - m_tInfo.size.cx * 0.5f);
		m_tHundRect.right = static_cast<LONG>(m_tPosArr[2].pt.x + m_tInfo.size.cx * 0.5f);
		m_tHundRect.top = static_cast<LONG>(m_tPosArr[2].pt.y - m_tInfo.size.cy * 0.5f);
		m_tHundRect.bottom = static_cast<LONG>(m_tPosArr[2].pt.y + m_tInfo.size.cy * 0.5f);

		m_tThouInfo.pt.x = m_tPosArr[3].pt.x;
		m_tThouInfo.pt.y = m_tPosArr[3].pt.y;
		m_tThouRect.left = static_cast<LONG>(m_tPosArr[3].pt.x - m_tInfo.size.cx * 0.5f);
		m_tThouRect.right = static_cast<LONG>(m_tPosArr[3].pt.x + m_tInfo.size.cx * 0.5f);
		m_tThouRect.top = static_cast<LONG>(m_tPosArr[3].pt.y - m_tInfo.size.cy * 0.5f);
		m_tThouRect.bottom = static_cast<LONG>(m_tPosArr[3].pt.y + m_tInfo.size.cy * 0.5f);
	}
	else if (FONTTYPE_TENTH == m_eType) // �ټ��ڸ�
	{
		m_tUnitInfo.pt.x = m_tPosArr[0].pt.x;
		m_tUnitInfo.pt.y = m_tPosArr[0].pt.y;
		m_tUnitRect.left = static_cast<LONG>(m_tPosArr[0].pt.x - m_tInfo.size.cx * 0.5f);
		m_tUnitRect.right = static_cast<LONG>(m_tPosArr[0].pt.x + m_tInfo.size.cx * 0.5f);
		m_tUnitRect.top = static_cast<LONG>(m_tPosArr[0].pt.y - m_tInfo.size.cy * 0.5f);
		m_tUnitRect.bottom = static_cast<LONG>(m_tPosArr[0].pt.y + m_tInfo.size.cy * 0.5f);

		m_tTensInfo.pt.x = m_tPosArr[1].pt.x;
		m_tTensInfo.pt.y = m_tPosArr[1].pt.y;
		m_tTensRect.left = static_cast<LONG>(m_tPosArr[1].pt.x - m_tInfo.size.cx * 0.5f);
		m_tTensRect.right = static_cast<LONG>(m_tPosArr[1].pt.x + m_tInfo.size.cx * 0.5f);
		m_tTensRect.top = static_cast<LONG>(m_tPosArr[1].pt.y - m_tInfo.size.cy * 0.5f);
		m_tTensRect.bottom = static_cast<LONG>(m_tPosArr[1].pt.y + m_tInfo.size.cy * 0.5f);

		m_tHundInfo.pt.x = m_tPosArr[2].pt.x;
		m_tHundInfo.pt.y = m_tPosArr[2].pt.y;
		m_tHundRect.left = static_cast<LONG>(m_tPosArr[2].pt.x - m_tInfo.size.cx * 0.5f);
		m_tHundRect.right = static_cast<LONG>(m_tPosArr[2].pt.x + m_tInfo.size.cx * 0.5f);
		m_tHundRect.top = static_cast<LONG>(m_tPosArr[2].pt.y - m_tInfo.size.cy * 0.5f);
		m_tHundRect.bottom = static_cast<LONG>(m_tPosArr[2].pt.y + m_tInfo.size.cy * 0.5f);

		m_tThouInfo.pt.x = m_tPosArr[3].pt.x;
		m_tThouInfo.pt.y = m_tPosArr[3].pt.y;
		m_tThouRect.left = static_cast<LONG>(m_tPosArr[3].pt.x - m_tInfo.size.cx * 0.5f);
		m_tThouRect.right = static_cast<LONG>(m_tPosArr[3].pt.x + m_tInfo.size.cx * 0.5f);
		m_tThouRect.top = static_cast<LONG>(m_tPosArr[3].pt.y - m_tInfo.size.cy * 0.5f);
		m_tThouRect.bottom = static_cast<LONG>(m_tPosArr[3].pt.y + m_tInfo.size.cy * 0.5f);

		m_tTenthInfo.pt.x = m_tPosArr[4].pt.x;
		m_tTenthInfo.pt.y = m_tPosArr[4].pt.y;
		m_tTenthRect.left = static_cast<LONG>(m_tPosArr[4].pt.x - m_tInfo.size.cx * 0.5f);
		m_tTenthRect.right = static_cast<LONG>(m_tPosArr[4].pt.x + m_tInfo.size.cx * 0.5f);
		m_tTenthRect.top = static_cast<LONG>(m_tPosArr[4].pt.y - m_tInfo.size.cy * 0.5f);
		m_tTenthRect.bottom = static_cast<LONG>(m_tPosArr[4].pt.y + m_tInfo.size.cy * 0.5f);
	}

	return 0;
}

void CFont::Render(HDC hDc)
{
	CMyBmp* pTensBit = CBitmapMgr::GetInstance()->FindImage(L"Font");
	if (NULL == pTensBit)  return;

	// 1���ڸ�
	TransparentBlt(hDc,
		static_cast<int>(m_tUnitRect.left + g_fScrollX),
		static_cast<int>(m_tUnitRect.top + g_fScrollY),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		pTensBit->GetMemDC(),
		static_cast<int>(m_tInfo.size.cx * m_tUnitFrame.iFrameStart),
		static_cast<int>(m_tInfo.size.cy * m_tUnitFrame.iScene),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		RGB(0, 255, 0));
	if (FONTTYPE_UNIT == m_eType)	return;

	// 10���ڸ�
	TransparentBlt(hDc,
		static_cast<int>(m_tTensRect.left + g_fScrollX),
		static_cast<int>(m_tTensRect.top + g_fScrollY),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		pTensBit->GetMemDC(),
		static_cast<int>(m_tInfo.size.cx * m_tTensFrame.iFrameStart),
		static_cast<int>(m_tInfo.size.cy * m_tUnitFrame.iScene),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		RGB(0, 255, 0));
	if (FONTTYPE_TENS == m_eType)	return;

	// 100���ڸ�
	TransparentBlt(hDc,
		static_cast<int>(m_tHundRect.left + g_fScrollX),
		static_cast<int>(m_tHundRect.top + g_fScrollY),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		pTensBit->GetMemDC(),
		static_cast<int>(m_tInfo.size.cx * m_tHundFrame.iFrameStart),
		static_cast<int>(m_tInfo.size.cy * m_tUnitFrame.iScene),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		RGB(0, 255, 0));
	if (FONTTYPE_HUND == m_eType)	return;

	// 1000���ڸ�
	TransparentBlt(hDc,
		static_cast<int>(m_tThouRect.left + g_fScrollX),
		static_cast<int>(m_tThouRect.top + g_fScrollY),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		pTensBit->GetMemDC(),
		static_cast<int>(m_tInfo.size.cx * m_tThouFrame.iFrameStart),
		static_cast<int>(m_tInfo.size.cy * m_tUnitFrame.iScene),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		RGB(0, 255, 0));
	if (FONTTYPE_THOU == m_eType)	return;

	// 10000���ڸ�
	TransparentBlt(hDc,
		static_cast<int>(m_tTenthRect.left + g_fScrollX),
		static_cast<int>(m_tTenthRect.top + g_fScrollY),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		pTensBit->GetMemDC(),
		static_cast<int>(m_tInfo.size.cx * m_tTenthFrame.iFrameStart),
		static_cast<int>(m_tInfo.size.cy * m_tUnitFrame.iScene),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		RGB(0, 255, 0));
	if (FONTTYPE_TENTH == m_eType)	return;
}

void CFont::FrameMove()
{

}

void CFont::SettingPos()
{
	int iIntervalX = static_cast<int>(12.f);
	int iIntervalX_HUND = static_cast<int>(23.f);
	int iIntervalY = static_cast<int>(25.f);

	float fTargetX = static_cast<float>(m_pObj->GetInfo().pt.x);
	float fTargetY = static_cast<float>(m_pObj->GetInfo().size.cy);
	float fTargetTop = static_cast<float>(m_pObj->GetCollRect().top);

	// 1�� ��Ʈ�϶�
	if (FONT_FIRSTFLOOR == m_eFloor)
	{
		if (FONTTYPE_UNIT == m_eType)
		{
			m_tPosArr[0].pt.x = (fTargetX);
			m_tPosArr[0].pt.y = (fTargetTop);
		}
		else if (FONTTYPE_TENS == m_eType)
		{
			m_tPosArr[0].pt.x = (fTargetX)+iIntervalX;
			m_tPosArr[0].pt.y = (fTargetTop);
			m_tPosArr[1].pt.x = (fTargetX)-iIntervalX;
			m_tPosArr[1].pt.y = (fTargetTop);
		}
		else if (FONTTYPE_HUND == m_eType)
		{
			m_tPosArr[1].pt.x = (fTargetX);
			m_tPosArr[1].pt.y = fTargetTop;
			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = (fTargetTop);
			m_tPosArr[2].pt.x = m_tPosArr[1].pt.x - iIntervalX_HUND;
			m_tPosArr[2].pt.y = (fTargetTop);
		}
		else if (FONTTYPE_THOU == m_eType)
		{
			m_tPosArr[1].pt.x = (fTargetX)+iIntervalX;
			m_tPosArr[1].pt.y = fTargetTop;
			m_tPosArr[2].pt.x = (fTargetX)-iIntervalX;
			m_tPosArr[2].pt.y = fTargetTop;
			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = fTargetTop;
			m_tPosArr[3].pt.x = m_tPosArr[2].pt.x - iIntervalX_HUND;
			m_tPosArr[3].pt.y = fTargetTop;
		}
		else if (FONTTYPE_TENTH == m_eType)
		{
			m_tPosArr[2].pt.x = fTargetX;
			m_tPosArr[2].pt.y = fTargetTop;

			m_tPosArr[1].pt.x = fTargetX + iIntervalX_HUND;
			m_tPosArr[1].pt.y = fTargetTop;
			m_tPosArr[3].pt.x = fTargetX - iIntervalX_HUND;
			m_tPosArr[3].pt.y = fTargetTop;

			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = fTargetTop;

			m_tPosArr[4].pt.x = m_tPosArr[3].pt.x - iIntervalX_HUND;
			m_tPosArr[4].pt.y = fTargetTop;
		}
	}

	// 2�� ��Ʈ�϶�
	else if (FONT_SECONDFLOOR == m_eFloor)
	{
		if (FONTTYPE_UNIT == m_eType)
		{
			m_tPosArr[0].pt.x = (fTargetX);
			m_tPosArr[0].pt.y = (fTargetTop - iIntervalY);
		}
		else if (FONTTYPE_TENS == m_eType)
		{
			m_tPosArr[0].pt.x = (fTargetX)+iIntervalX;
			m_tPosArr[0].pt.y = (fTargetTop - iIntervalY);
			m_tPosArr[1].pt.x = (fTargetX)-iIntervalX;
			m_tPosArr[1].pt.y = (fTargetTop - iIntervalY);
		}
		else if (FONTTYPE_HUND == m_eType)
		{
			m_tPosArr[1].pt.x = (fTargetX);
			m_tPosArr[1].pt.y = fTargetTop - iIntervalY;
			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = (fTargetTop - iIntervalY);
			m_tPosArr[2].pt.x = m_tPosArr[1].pt.x - iIntervalX_HUND;
			m_tPosArr[2].pt.y = (fTargetTop - iIntervalY);
		}
		else if (FONTTYPE_THOU == m_eType)
		{
			m_tPosArr[1].pt.x = (fTargetX)+iIntervalX;
			m_tPosArr[1].pt.y = fTargetTop - iIntervalY;
			m_tPosArr[2].pt.x = (fTargetX)-iIntervalX;
			m_tPosArr[2].pt.y = fTargetTop - iIntervalY;
			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = fTargetTop - iIntervalY;
			m_tPosArr[3].pt.x = m_tPosArr[2].pt.x - iIntervalX_HUND;
			m_tPosArr[3].pt.y = fTargetTop - iIntervalY;
		}
		else if (FONTTYPE_TENTH == m_eType)
		{
			m_tPosArr[2].pt.x = fTargetX;
			m_tPosArr[2].pt.y = fTargetTop - iIntervalY;

			m_tPosArr[1].pt.x = fTargetX + iIntervalX_HUND;
			m_tPosArr[1].pt.y = fTargetTop - iIntervalY;
			m_tPosArr[3].pt.x = fTargetX - iIntervalX_HUND;
			m_tPosArr[3].pt.y = fTargetTop - iIntervalY;

			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = fTargetTop - iIntervalY;

			m_tPosArr[4].pt.x = m_tPosArr[3].pt.x - iIntervalX_HUND;
			m_tPosArr[4].pt.y = fTargetTop - iIntervalY;
		}
	}

	// 3�� ��Ʈ�϶�
	else if (FONT_THIRDFLOOR == m_eFloor)
	{
		if (FONTTYPE_UNIT == m_eType)
		{
			m_tPosArr[0].pt.x = (fTargetX);
			m_tPosArr[0].pt.y = (fTargetTop - iIntervalY * 2);
		}
		else if (FONTTYPE_TENS == m_eType)
		{
			m_tPosArr[0].pt.x = (fTargetX)+iIntervalX;
			m_tPosArr[0].pt.y = (fTargetTop - iIntervalY * 2);
			m_tPosArr[1].pt.x = (fTargetX)-iIntervalX;
			m_tPosArr[1].pt.y = (fTargetTop - iIntervalY * 2);
		}
		else if (FONTTYPE_HUND == m_eType)
		{
			m_tPosArr[1].pt.x = (fTargetX);
			m_tPosArr[1].pt.y = fTargetTop - iIntervalY * 2;
			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = (fTargetTop - iIntervalY * 2);
			m_tPosArr[2].pt.x = m_tPosArr[1].pt.x - iIntervalX_HUND;
			m_tPosArr[2].pt.y = (fTargetTop - iIntervalY * 2);
		}
		else if (FONTTYPE_THOU == m_eType)
		{
			m_tPosArr[1].pt.x = (fTargetX)+iIntervalX;
			m_tPosArr[1].pt.y = fTargetTop - iIntervalY * 2;
			m_tPosArr[2].pt.x = (fTargetX)-iIntervalX;
			m_tPosArr[2].pt.y = fTargetTop - iIntervalY * 2;
			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = fTargetTop - iIntervalY * 2;
			m_tPosArr[3].pt.x = m_tPosArr[2].pt.x - iIntervalX_HUND;
			m_tPosArr[3].pt.y = fTargetTop - iIntervalY * 2;
		}
		else if (FONTTYPE_TENTH == m_eType)
		{
			m_tPosArr[2].pt.x = fTargetX;
			m_tPosArr[2].pt.y = fTargetTop - iIntervalY * 2;

			m_tPosArr[1].pt.x = fTargetX + iIntervalX_HUND;
			m_tPosArr[1].pt.y = fTargetTop - iIntervalY * 2;
			m_tPosArr[3].pt.x = fTargetX - iIntervalX_HUND;
			m_tPosArr[3].pt.y = fTargetTop - iIntervalY * 2;

			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = fTargetTop - iIntervalY * 2;

			m_tPosArr[4].pt.x = m_tPosArr[3].pt.x - iIntervalX_HUND;
			m_tPosArr[4].pt.y = fTargetTop - iIntervalY * 2;
		}
	}

	// 4�� ��Ʈ�϶�
	else if (FONT_FOURTHFLOOR == m_eFloor)
	{
		if (FONTTYPE_UNIT == m_eType)
		{
			m_tPosArr[0].pt.x = (fTargetX);
			m_tPosArr[0].pt.y = (fTargetTop - iIntervalY * 3);
		}
		else if (FONTTYPE_TENS == m_eType)
		{
			m_tPosArr[0].pt.x = (fTargetX)+iIntervalX;
			m_tPosArr[0].pt.y = (fTargetTop - iIntervalY * 3);
			m_tPosArr[1].pt.x = (fTargetX)-iIntervalX;
			m_tPosArr[1].pt.y = (fTargetTop - iIntervalY * 3);
		}
		else if (FONTTYPE_HUND == m_eType)
		{
			m_tPosArr[1].pt.x = (fTargetX);
			m_tPosArr[1].pt.y = fTargetTop - iIntervalY * 3;
			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = (fTargetTop - iIntervalY * 3);
			m_tPosArr[2].pt.x = m_tPosArr[1].pt.x - iIntervalX_HUND;
			m_tPosArr[2].pt.y = (fTargetTop - iIntervalY * 3);
		}
		else if (FONTTYPE_THOU == m_eType)
		{
			m_tPosArr[1].pt.x = (fTargetX)+iIntervalX;
			m_tPosArr[1].pt.y = fTargetTop - iIntervalY * 3;
			m_tPosArr[2].pt.x = (fTargetX)-iIntervalX;
			m_tPosArr[2].pt.y = fTargetTop - iIntervalY * 3;
			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = fTargetTop - iIntervalY * 3;
			m_tPosArr[3].pt.x = m_tPosArr[2].pt.x - iIntervalX_HUND;
			m_tPosArr[3].pt.y = fTargetTop - iIntervalY * 3;
		}
		else if (FONTTYPE_TENTH == m_eType)
		{
			m_tPosArr[2].pt.x = fTargetX;
			m_tPosArr[2].pt.y = fTargetTop - iIntervalY * 3;

			m_tPosArr[1].pt.x = fTargetX + iIntervalX_HUND;
			m_tPosArr[1].pt.y = fTargetTop - iIntervalY * 3;
			m_tPosArr[3].pt.x = fTargetX - iIntervalX_HUND;
			m_tPosArr[3].pt.y = fTargetTop - iIntervalY * 3;

			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = fTargetTop - iIntervalY * 3;

			m_tPosArr[4].pt.x = m_tPosArr[3].pt.x - iIntervalX_HUND;
			m_tPosArr[4].pt.y = fTargetTop - iIntervalY * 3;
		}
	}

	// 5�� ��Ʈ�϶�
	else if (FONT_FIFTHFLOOR == m_eFloor)
	{
		if (FONTTYPE_UNIT == m_eType)
		{
			m_tPosArr[0].pt.x = (fTargetX);
			m_tPosArr[0].pt.y = (fTargetTop - iIntervalY * 4);
		}
		else if (FONTTYPE_TENS == m_eType)
		{
			m_tPosArr[0].pt.x = (fTargetX)+iIntervalX;
			m_tPosArr[0].pt.y = (fTargetTop - iIntervalY * 4);
			m_tPosArr[1].pt.x = (fTargetX)-iIntervalX;
			m_tPosArr[1].pt.y = (fTargetTop - iIntervalY * 4);
		}
		else if (FONTTYPE_HUND == m_eType)
		{
			m_tPosArr[1].pt.x = (fTargetX);
			m_tPosArr[1].pt.y = fTargetTop - iIntervalY * 4;
			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = (fTargetTop - iIntervalY * 4);
			m_tPosArr[2].pt.x = m_tPosArr[1].pt.x - iIntervalX_HUND;
			m_tPosArr[2].pt.y = (fTargetTop - iIntervalY * 4);
		}
		else if (FONTTYPE_THOU == m_eType)
		{
			m_tPosArr[1].pt.x = static_cast<float>((fTargetX)+iIntervalX * 0.5);
			m_tPosArr[1].pt.y = fTargetTop - iIntervalY * 4;
			m_tPosArr[2].pt.x = static_cast<float>((fTargetX)-iIntervalX * 0.5);
			m_tPosArr[2].pt.y = fTargetTop - iIntervalY * 4;
			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = fTargetTop - iIntervalY * 4;
			m_tPosArr[3].pt.x = m_tPosArr[2].pt.x - iIntervalX_HUND;
			m_tPosArr[3].pt.y = fTargetTop - iIntervalY * 4;
		}

		else if (FONTTYPE_TENTH == m_eType)
		{
			m_tPosArr[2].pt.x = fTargetX;
			m_tPosArr[2].pt.y = fTargetTop - iIntervalY * 4;

			m_tPosArr[1].pt.x = fTargetX + iIntervalX_HUND;
			m_tPosArr[1].pt.y = fTargetTop - iIntervalY * 4;

			m_tPosArr[3].pt.x = fTargetX - iIntervalX_HUND;
			m_tPosArr[3].pt.y = fTargetTop - iIntervalY * 4;

			m_tPosArr[0].pt.x = m_tPosArr[1].pt.x + iIntervalX_HUND;
			m_tPosArr[0].pt.y = fTargetTop - iIntervalY * 4;

			m_tPosArr[4].pt.x = m_tPosArr[3].pt.x - iIntervalX_HUND;
			m_tPosArr[4].pt.y = fTargetTop - iIntervalY * 4;
		}
	}
}

void CFont::Calculating()
{
	int iTenth = m_iNumber / 10000;
	int iThou = (m_iNumber - (iTenth * 10000)) / 1000;
	int iHund = (m_iNumber - (iTenth * 10000) - (iThou * 1000)) / 100;
	int iTens = (m_iNumber - (iTenth * 10000) - (iThou * 1000) - (iHund * 100)) / 10;
	int iUnit = (m_iNumber - (iTenth * 10000) - (iThou * 1000) - (iHund * 100) - (iTens * 10));


	m_tUnitFrame.iFrameStart = iUnit;
	m_tUnitFrame.iFrameEnd = iUnit;
	
	m_tTensFrame.iFrameStart = iTens;
	m_tTensFrame.iFrameEnd = iTens;

	m_tHundFrame.iFrameStart = iHund;
	m_tHundFrame.iFrameEnd = iHund;

	m_tThouFrame.iFrameStart = iThou;
	m_tThouFrame.iFrameEnd = iThou;

	m_tTenthFrame.iFrameStart = iTenth;
	m_tTenthFrame.iFrameEnd = iTenth;


	switch(m_eID)
	{
	case FONTID_ATTACK:
		m_tUnitFrame.iScene = 0;
		m_tTensFrame.iScene = 0;
		m_tHundFrame.iScene = 0;
		m_tThouFrame.iScene = 0;
		m_tTenthFrame.iScene = 0;
		break;
	case FONTID_CRITICAL:
		m_tUnitFrame.iScene = 1;
		m_tTensFrame.iScene = 1;
		m_tHundFrame.iScene = 1;
		m_tThouFrame.iScene = 1;
		m_tTenthFrame.iScene = 1;
		break;
	case FONTID_DAMAGED:
		m_tUnitFrame.iScene = 2;
		m_tTensFrame.iScene = 2;
		m_tHundFrame.iScene = 2;
		m_tThouFrame.iScene = 2;
		m_tTenthFrame.iScene = 2;
		break;
	}
}

void CFont::Release(void)
{

}

int CFont::ReturningRandomNumber(int iNumber)
{
	// 	srand((unsigned)time(NULL));
	// 
	// 	// ** 30000~ 39999 ������ ���� ����.
	// 
	// 	// �ڸ��� ���Ѵ�����
	// 	int iCipher = CalculatingCipher(iNumber);
	// 
	// 	// �ְ� �ڸ��� (���� 31231�̸� 30000�� ���� ��)
	// 	int iHigherNumber = (iNumber / (pow(10.0, iCipher)) * pow(10.0, iCipher));
	// 
	// 	// �� ���� �ְ� �ڸ��� (���� 31231�̸� 1000�� ���� ��)
	// 	//	int iSecondHigherNumber = (iNumber - iHigherNumber) / pow(10.0, (iCipher-1)) * pow(10.0, (iCipher - 1));
	// 
	// 	// �ڸ����� ���� (������ ����) // �ִ� 5�ڸ��� ��
	// 	int iSecondCipher = iCipher - 1;
	// 	int iThirdCipher = iCipher - 2;
	// 	int iFourthCipher = iCipher - 3;
	// 	int iFifthCipher = iCipher - 4;
	// 
	// 	// �տ��� �ι�°, ����°, �׹�°.. �ڸ����� ������ ������ ����
	// 	// iNumber�� 5�ڸ����� secondcipher�� 4, 1�ڸ����� secondcipher�� 0�̹Ƿ� �̶� if�� ��Ž.
	// 
	// 	/*int iRandom = (rand() % 10) * (static_cast<int>(pow(10.0, iCipher - 1)));*/
	// 	int iRandom = 0;
	// 	int iSecondRandom = 0;
	// 	int iThirdRandom = 0;
	// 	int iFourthRandom = 0;
	// 	int iFifthRandom = 0;
	// 
	// 	if(iSecondCipher >= 1)
	// 	{
	// 		iSecondRandom = (rand() % 10) * (static_cast<int>(pow(10.0, iSecondCipher - 1)));
	// 
	// 		if(iThirdCipher >= 1)
	// 		{
	// 			iThirdRandom = (rand() % 10) * (static_cast<int>(pow(10.0, iThirdCipher - 1)));
	// 
	// 			if(iFourthCipher >= 1)
	// 			{
	// 				iFourthRandom = (rand() % 10) * (static_cast<int>(pow(10.0, iFourthCipher - 1)));
	// 
	// 				if(iFifthCipher >= 1)
	// 				{
	// 					iFifthRandom = (rand() % 10) * (static_cast<int>(pow(10.0, iFifthCipher - 1)));
	// 				}
	// 			}
	// 		}
	// 	}
	// 
	// 	iRandom = iHigherNumber + iSecondRandom + iThirdRandom + iFourthRandom + iFifthRandom;
	// 
	// 
	// 
	// 	return iRandom;
	return 0;
}

