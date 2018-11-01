#include "StdAfx.h"
#include "LeafCommunication.h"

CLeafCommunication::CLeafCommunication(void)
{
}

CLeafCommunication::~CLeafCommunication(void)
{
}

void CLeafCommunication::Initialize(void)
{
	m_tInfo.fCX = 517.f;
	m_tInfo.fCY = 189.f;	

	// 버튼 렉트
	m_bIsNextButtonActivated = false;


	// 이미지
	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_pImgName = L"Communication_Leaf";

	m_iRenderNumber= 0;
	m_iRenderNumberOld = m_iRenderNumber - 1;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iScene = 0;
	m_tFrame.dwFrameSpd = 250;

	m_tInfo.fX = WINCX * 0.5f;
	m_tInfo.fY = WINCY * 0.5f;

	SetNextButtonRect();

	m_tInfo.fX -= g_fScrollX;
	m_tInfo.fY -= g_fScrollY;

	

	UpdateRect();

	m_eRenderType = RENDER_OBJ;
}

int CLeafCommunication::Update(void)
{
// 	float fX = m_tInfo.fX - g_fScrollX;
// 	float fY = m_tInfo.fY - g_fScrollY;
// 
// 	m_tInfo.fX = fX;
// 	m_tInfo.fY = fY;
	if(m_bIsDead)
		return 1;

	FrameMove();
	CObj::UpdateRect();

	// 퀘스트 완료 유무에 따른 렌더 넘버 구분
	if(m_bIsSucessed)
	{
		m_iRenderNumber = 3;
	}

	// 마우스가 들어오는지
	POINT pt;
	POINT pt2;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt2.x = static_cast<LONG>(pt.x - g_fScrollX);
	pt2.y = static_cast<LONG>(pt.y - g_fScrollY);


	// **0718
	if(PtInRect(&m_tNextButton, pt) && m_bIsNextButtonActivated == false )
	{
		if(CKeyMgr::GetInstance()->OnceKeyDown(VK_LBUTTON) 
			&& m_bIsNextButtonActivated == false)
		{
			if(!m_bIsSucessed)
			{
				if(m_iRenderNumber >= 2)
				{
					m_iRenderNumber = -1;
				}
				else
					m_iRenderNumber++;
				m_iRenderNumberOld = m_iRenderNumber - 1;		
			}
			if(m_bIsSucessed)
			{
				if(m_iRenderNumber != 3)
					m_iRenderNumber = 3;
				else if(m_iRenderNumber == 3)
				{
										m_iRenderNumber = -1;
				}
			}
			m_bIsNextButtonActivated = true;
		}
		
		if(!CKeyMgr::GetInstance()->OnceKeyDown(VK_LBUTTON)
			&& m_bIsNextButtonActivated == true)
		{
			m_bIsNextButtonActivated = false;
		}
	}


	return 0;
}

void CLeafCommunication::Render(HDC hDc)
{

	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;


	// 퀘스트창 스크롤 왜 먹여;;
	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left /*+ g_fScrollX*/),
		static_cast<int>(m_tRect.top /*+ g_fScrollY*/), 
		static_cast<int>(m_tInfo.fCX),
		static_cast<int>(m_tInfo.fCY),
		pBit->GetMemDC(),
		static_cast<int>(m_tFrame.iFrameStart * m_tInfo.fCX),
		static_cast<int>(m_tFrame.iScene * m_tInfo.fCY),
		static_cast<int>(m_tInfo.fCX),
		static_cast<int>(m_tInfo.fCY),
		RGB(255, 0, 255));

	//Rectangle(hDc,m_tNextButton.left, m_tNextButton.top, m_tNextButton.right, m_tNextButton.bottom);
}

void CLeafCommunication::Release(void)
{

}

void CLeafCommunication::FrameMove()
{
	switch(m_iRenderNumber)
	{
	case 0:
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 0;
		break;
	case 1:
		m_tFrame.iFrameStart = 1;
		m_tFrame.iFrameEnd = 1;
		break;
	case 2:
		m_tFrame.iFrameStart = 2;
		m_tFrame.iFrameEnd = 2;
		break;
	case 3:
		m_tFrame.iFrameStart = 3;
		m_tFrame.iFrameEnd = 3;
		break;
	case -1:
		m_bIsDead = 1;
		break;
	case 4:
		m_bIsDead = 1;
		break;
	}

}
