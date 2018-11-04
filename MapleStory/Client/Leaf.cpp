#include "StdAfx.h"
#include "Leaf.h"
#include "Mouse.h"
#include "LeafCommunication.h"

int g_iTakedMob1 = 0;
int g_iTakedMob1Max = 1;
int g_iTakedMob2 = 0;
int g_iTakedMob2Max = 1;
bool g_bIsMissionClear = false;

CLeaf::CLeaf(void)
{
}

CLeaf::~CLeaf(void)
{
}

void CLeaf::Initialize(void)
{
	m_tInfo.size.cx = 69.f;
	m_tInfo.size.cy = 66.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iScene = 0;
	m_tFrame.dwFrameSpd = 250;

	m_pImgName = L"Leaf";

	m_iCommunicationNumber = 0;

	m_bIsActivated = false;
	
	m_bIsQuestSucessed = false;

	m_eRenderType = RENDER_OBJ;
}

int CLeaf::Update(void)
{
	FrameMove();

	CObj::UpdateRect();

	POINT pt;
	POINT pt2;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt2.x = static_cast<LONG>(pt.x - g_fScrollX);
	pt2.y = static_cast<LONG>(pt.y - g_fScrollY);


	if(PtInRect(&m_tRect, pt2) && m_bIsActivated == false)
	{
		if(CKeyMgr::GetInstance()->OnceKeyDown(VK_LBUTTON) && m_bIsActivated == false)
		{
			m_bIsActivated = true;
		}

		if(!CKeyMgr::GetInstance()->OnceKeyDown(VK_LBUTTON) && m_bIsActivated == true)
		{
			CObjMgr::GetInstance()->AddObject(CreateCommunication<CLeafCommunication>(),
				OBJ_COMMUNICATION);
			m_bIsActivated = false;
		}
	}
	//

	if(g_iTakedMob1 >= g_iTakedMob1Max && g_iTakedMob2 >= g_iTakedMob2Max && g_bIsMissionClear == false)
	{
		CSoundMgr::GetInstance()->PlaySound(L"QuestClear.MP3", CSoundMgr::CHANNEL_EFFECT);

		m_bIsQuestSucessed = true;
		g_bIsMissionClear = true;
	}
	//

	return 0;
}

void CLeaf::Render(HDC hDc)
{

	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;

	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left + g_fScrollX),
		static_cast<int>(m_tRect.top + g_fScrollY), 
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		pBit->GetMemDC(),
		static_cast<int>(m_tFrame.iFrameStart * m_tInfo.size.cx),
		static_cast<int>(m_tFrame.iScene * m_tInfo.size.cy),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		RGB(0, 255, 0));


	CMyBmp* pMarkBit = CBitmapMgr::GetInstance()->FindImage(L"QuestMark");

	if(NULL == pMarkBit)  return;

	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left + g_fScrollX),
		static_cast<int>(m_tRect.top + g_fScrollY - 100.f), 
		static_cast<int>(126),
		static_cast<int>(91),
		pMarkBit->GetMemDC(),
		static_cast<int>(0),
		static_cast<int>(0),
		static_cast<int>(126),
		static_cast<int>(91),
		RGB(0, 255, 0));

//	Rectangle(hDc, m_tRect.left + g_fScrollX, m_tRect.top + g_fScrollY, m_tRect.right + g_fScrollX, m_tRect.bottom + g_fScrollY);

// 	POINT pt;
// 	RECT ptRect;
// 	float size.cx = 10.f;
// 	float size.cy = 10.f;
// 	GetCursorPos(&pt);
// 	ScreenToClient(g_hWnd, &pt);
// // 	pt.x += g_fScrollX;
// // 	pt.y += g_fScrollY;
// 
// 	ptRect.left = pt.x - size.cx;
// 	ptRect.right = pt.x + size.cx;
// 	ptRect.top = pt.y - size.cy;
// 	ptRect.bottom = pt.y + size.cy;
// 
// 
// 	Rectangle(hDc, ptRect.left, ptRect.top, ptRect.right, ptRect.bottom );



}

void CLeaf::Release(void)
{

}

void CLeaf::FrameMove()
{

}
