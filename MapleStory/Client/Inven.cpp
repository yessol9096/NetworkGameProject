#include "StdAfx.h"
#include "Inven.h"
#include "Player.h"
#include "Item.h"
#include "HPPortion.h"
#include "MPPortion.h"

CInven::CInven(void)
: m_iMaxInven(24), m_bIsShowed(false)
{
	m_vecInven.reserve(m_iMaxInven);
	memset(m_pEquipment, NULL, sizeof(CObj*) * ITEM_END);
}

CInven::~CInven(void)
{
	Release();
}

void CInven::Initialize(void)
{
	m_tInfo.fX = WINCX * 0.6;
	m_tInfo.fY = WINCY * 0.3;

	m_tInfo.fCX = 172.f;
	m_tInfo.fCY = 335.f;

	UpdateRect();
	ButtonSetting();

	SettingInvenPos();

	// 버튼
	m_bIsEquipButtonOn = true; 
	m_bIsPortionButtonOn = false;
	m_bIsExitButtonOn = false;
	m_bIsMoveButtonOn = false;
	m_bIsHpPortionOn = false;
	m_bIsMpPortionOn = false;


	// 아이템 설명창
	m_tRectHPPortionInfo.top = 0;
	m_tRectHPPortionInfo.bottom = 0;
	m_tRectHPPortionInfo.left = 0;
	m_tRectHPPortionInfo.right = 0;

	m_tHPPortionInfo.fX = 0;
	m_tHPPortionInfo.fY = 0;
	m_tHPPortionInfo.fCX = 287.f;
	m_tHPPortionInfo.fCY = 144.f;
	

	m_tRectMPPortionInfo.top = 0;
	m_tRectMPPortionInfo.bottom = 0;
	m_tRectMPPortionInfo.left = 0;
	m_tRectMPPortionInfo.right = 0;

	m_tMPPortionInfo.fX = 0;
	m_tMPPortionInfo.fY = 0;
	m_tMPPortionInfo.fCX = 287.f;
	m_tMPPortionInfo.fCY = 144.f;


	m_bIsHPPortionInfoOn = false;
	m_bIsMPPortionInfoOn = false;



	// 이미지
	m_pImgName = L"Inventory_Equip";

	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iScene = 1;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.dwFrameSpd = 200;

	// 렌더 타입
	m_eRenderType = RENDER_OBJ;

	// 원래 pt 받아오기 위함
	m_bIsOriginalpt = false;
	m_fGapX = 0.f;
	m_fGapY = 0.f;

	// 아이템 (기본) 배치
	ItemSetting();



}

int CInven::Update(void)
{

	CObj::UpdateRect();

	if(CKeyMgr::GetInstance()->OnceKeyUp('I'))
	{
		if(!m_bIsShowed)
			CSoundMgr::GetInstance()->PlaySound(L"OpenWnd.mp3", CSoundMgr::CHANNEL_EFFECT);
		else
			CSoundMgr::GetInstance()->PlaySound(L"CloseWnd.mp3", CSoundMgr::CHANNEL_EFFECT);

		m_bIsShowed = !m_bIsShowed;
	}
	
	if(m_bIsShowed == true)
	{
		m_tFrame.iScene = 0;
	}
	else
	{
		m_tFrame.iScene = 1;
	}

	// 아이템 설명창 렉트 업데이트..//
	POINT pt;
	POINT pt2;
	float fPtCX = 52.f;
	float fPtCY = 30.f;

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt2.x = static_cast<LONG>(pt.x - g_fScrollX) - 10 + static_cast<LONG>(g_fScrollX);
	pt2.y = static_cast<LONG>(pt.y - g_fScrollY) - 21 + static_cast<LONG>(g_fScrollY);

	m_tHPPortionInfo.fX = 0;
	m_tHPPortionInfo.fY = 0;
	m_tHPPortionInfo.fCX = 287.f;
	m_tHPPortionInfo.fCY = 144.f;

	m_tRectHPPortionInfo.top = static_cast<LONG>(pt2.y + fPtCY * 0.5f);
	m_tRectHPPortionInfo.bottom = static_cast<LONG>(m_tRectHPPortionInfo.top + m_tHPPortionInfo.fCY);
	m_tRectHPPortionInfo.left = static_cast<LONG>(pt2.x + fPtCX * 0.5f);
	m_tRectHPPortionInfo.right = static_cast<LONG>(m_tRectHPPortionInfo.left + m_tHPPortionInfo.fCX);



	m_tRectMPPortionInfo.top = static_cast<LONG>(pt2.y + fPtCY * 0.5f);
	m_tRectMPPortionInfo.bottom = static_cast<LONG>(m_tRectMPPortionInfo.top + m_tMPPortionInfo.fCY);
	m_tRectMPPortionInfo.left = static_cast<LONG>(pt2.x + fPtCX * 0.5f);
	m_tRectMPPortionInfo.right = static_cast<LONG>(m_tRectMPPortionInfo.left + m_tMPPortionInfo.fCX);

	m_tMPPortionInfo.fX = 0;
	m_tMPPortionInfo.fY = 0;
	m_tMPPortionInfo.fCX = 287.f;
	m_tMPPortionInfo.fCY = 144.f;

	//
	SettingInvenPos(); // 각 인벤 칸마다의 pos, rect 지정

	FrameMove();

	ButtonSetting(); // 버튼들 rect 지정

	MouseInButton(); // 마우스가 button 안에 들어왔는지 판정

	MoveToMouse();

	return 0;
}

void CInven::Render(HDC hDc)
{
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;

	// 인벤토리
	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left/* + g_fScrollX*/),
		static_cast<int>(m_tRect.top /*+ g_fScrollY*/), 
		static_cast<int>(m_tInfo.fCX),
		static_cast<int>(m_tInfo.fCY),
		pBit->GetMemDC(),
		static_cast<int>(m_tFrame.iFrameStart * m_tInfo.fCX),
		static_cast<int>(m_tFrame.iScene * m_tInfo.fCY),
		static_cast<int>(m_tInfo.fCX),
		static_cast<int>(m_tInfo.fCY),
		RGB(0, 255, 0));

	// 아이템 설명창 출력
	float fInfoCX = 287.f;
	float fInfoCY = 144.f;

	
	if(m_bIsHPPortionInfoOn && m_bIsPortionButtonOn)// HP포션 설명창
	{

		CMyBmp* pBit2 = CBitmapMgr::GetInstance()->FindImage(L"HPPortion_Info");

		if(NULL == pBit2)  return;

		TransparentBlt(hDc,
			static_cast<int>(m_tRectHPPortionInfo.left/* + g_fScrollX*/),
			static_cast<int>(m_tRectHPPortionInfo.top /*+ g_fScrollY*/), 
			static_cast<int>(fInfoCX),
			static_cast<int>(fInfoCY),
			pBit2->GetMemDC(),
			0,
			0,
			static_cast<int>(fInfoCX),
			static_cast<int>(fInfoCY),
			RGB(255, 255, 255));
	}
	if(m_bIsMPPortionInfoOn && m_bIsPortionButtonOn)// HP포션 설명창
	{

		CMyBmp* pBit3 = CBitmapMgr::GetInstance()->FindImage(L"MPPortion_Info");

		if(NULL == pBit3)  return;

		TransparentBlt(hDc,
			static_cast<int>(m_tRectMPPortionInfo.left/* + g_fScrollX*/),
			static_cast<int>(m_tRectMPPortionInfo.top /*+ g_fScrollY*/), 
			static_cast<int>(fInfoCX),
			static_cast<int>(fInfoCY),
			pBit3->GetMemDC(),
			0,
			0,
			static_cast<int>(fInfoCX),
			static_cast<int>(fInfoCY),
			RGB(255, 255, 255));
	}

	

	// 아이템 출력
	if(m_bIsShowed && m_bIsPortionButtonOn)
	{

		vector<CObj*>::iterator iter_begin = m_vecInven.begin();
		vector<CObj*>::iterator iter_end = m_vecInven.end();

		int i = 0;

		for(; iter_begin != iter_end; ++iter_begin)
		{
			// 주기적으로 포스 업데이트
			(*iter_begin)->SetPos(static_cast<float>(m_ptSpacePos[i].x), 
									static_cast<float>(m_ptSpacePos[i].y));
			(*iter_begin)->Update();
			(*iter_begin)->Render(hDc);
			++i;
		}
	}

	// 플레이어 메소 출력
	if(m_pPlayer != NULL && m_bIsShowed == true) 
	{
		TCHAR	szMesoBuf[128] = __T("");
		swprintf_s(szMesoBuf, __T("%d"), m_pPlayer->GetState().iGold);
		RECT tMesoRect = { static_cast<LONG>(m_tRect.left + 85.f), 
			static_cast<LONG>(m_tRect.top + 266.f), 
			static_cast<LONG>(m_tRect.left + 105.f), 
			static_cast<LONG>(m_tRect.top + 280.f) };
		DrawText(hDc, szMesoBuf, _tcslen(szMesoBuf), &tMesoRect, DT_NOCLIP);
	}

	if(GetAsyncKeyState('3'))
	{
		// Exit 버튼
		Rectangle(hDc, m_tExitButtonRect.left, m_tExitButtonRect.top, 
			m_tExitButtonRect.right, m_tExitButtonRect.bottom);
		// Equip 버튼
		Rectangle(hDc, m_tEquipButtonRect.left, m_tEquipButtonRect.top, 
			m_tEquipButtonRect.right, m_tEquipButtonRect.bottom);
		// Portion 버튼
		Rectangle(hDc, m_tPortionButtonRect.left, m_tPortionButtonRect.top, 
			m_tPortionButtonRect.right, m_tPortionButtonRect.bottom);
		// 메소 자리
		Rectangle(hDc, static_cast<int>(m_tRect.left + 52.f), 
			static_cast<int>(m_tRect.top + 266.f), 
			static_cast<int>(m_tRect.left + 125.f), 
			static_cast<int>(m_tRect.top + 282.f));
		// 인벤 스페이스
		for(int i = 0; i < 6; ++i) // 세로축
		{
			for(int j = 0; j < 4; ++j) // 가로축
			{
				int iIndex = 4 * i + j;
				Rectangle(hDc, m_tSpaceRect[iIndex].left, m_tSpaceRect[iIndex].top, m_tSpaceRect[iIndex].right, m_tSpaceRect[iIndex].bottom);

			}
		}
	}
	

}

void CInven::Release(void)
{

}

void CInven::FrameMove()
{

}

// 버튼


// 기능

bool CInven::GainItem(CObj* pItem)
{
	if(m_vecInven.size() < m_iMaxInven)
	{
// 		CObj* pMyItem = new CItem(*(dynamic_cast<CItem>*(pItem)));
// 		m_vecInven.push_back(pMyItem);
		return true;
	}
	else
	{
		// 인벤 가득참
		return false;
	}
}

void CInven::EquipItem(void)
{
}

void CInven::UnEquipItem(void)
{

}

void CInven::SettingEquip(void)
{

}

STATE CInven::EquipStat(void)
{
	STATE tStat;

	memset(&tStat, 0, sizeof(STATE));

	for(int i = 0; i < ITEM_END; ++i)
	{
		if(NULL != m_pEquipment[i])
		{
			tStat.iAtt += m_pEquipment[i]->GetState().iAtt;
			tStat.iMaxHp += m_pEquipment[i]->GetState().iMaxHp;
		}
	}

	return tStat;
}

void CInven::ButtonSetting()
{
	float fEquipCX = 32.f;
	float fEquipCY = 20.f;

	float fExitCX = 12.f;
	float fExitCY = 13.f;

	m_tEquipButtonRect.top = static_cast<LONG>(m_tRect.top + 19.f);
	m_tEquipButtonRect.bottom = static_cast<LONG>(m_tRect.top +  46.f);
	m_tEquipButtonRect.left = static_cast<LONG>(m_tRect.left + 10.f);
	m_tEquipButtonRect.right = static_cast<LONG>(m_tRect.left + 39.f);

	m_tPortionButtonRect.top = static_cast<LONG>(m_tRect.top + 19.f);
	m_tPortionButtonRect.bottom = static_cast<LONG>(m_tRect.top + 46.f);
	m_tPortionButtonRect.left = static_cast<LONG>(m_tRect.left + 39.f);
	m_tPortionButtonRect.right = static_cast<LONG>(m_tRect.left + 70.f);

	m_tExitButtonRect.top = static_cast<LONG>(m_tRect.top + 4.f);
	m_tExitButtonRect.bottom = static_cast<LONG>(m_tRect.top + 20.f);
	m_tExitButtonRect.left = static_cast<LONG>(m_tRect.left + 140.f);
	m_tExitButtonRect.right = static_cast<LONG>(m_tRect.left + 168.f);

	m_tMoveButtonRect.top = m_tRect.top;
	m_tMoveButtonRect.bottom = static_cast<LONG>(m_tRect.top + 24.f);
	m_tMoveButtonRect.left = m_tRect.left;
	m_tMoveButtonRect.right = static_cast<LONG>(m_tRect.left + 148.f);

}

void CInven::MouseInButton()
{
	POINT pt;
	POINT pt2;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt2.x = static_cast<LONG>(pt.x - g_fScrollX) - 10 + static_cast<LONG>(g_fScrollX);
	pt2.y = static_cast<LONG>(pt.y - g_fScrollY) - 21 + static_cast<LONG>(g_fScrollY);

#ifdef _DEBUG
// 	system("cls");
// 	cout << "g_fScrollX : " << g_fScrollX << endl;
// 	cout << "g_fScrollY : " << g_fScrollY << endl;
// 	cout << "pt2.x , pt2.y : ";
// 	cout << pt2.x << ", " << pt2.y << endl << endl;
// 	cout << "m_tExitButtonRect.left, m_tExitButtonRect.top, m_tExitButtonRect.right, m_tExitButtonRect.bottom :" << endl;
// 	cout << "(" << m_tExitButtonRect.left << ", " << m_tExitButtonRect.top << ") " << "(" << m_tExitButtonRect.right << ", " << m_tExitButtonRect.bottom << ")" << endl << endl;
// 	cout << "m_tEquipButtonRect.left, m_tEquipButtonRect.top, m_tEquipButtonRect.right, m_tEquipButtonRect.bottom :" << endl;
// 	cout << "(" << m_tEquipButtonRect.left << ", " << m_tEquipButtonRect.top << ") " << "(" << m_tEquipButtonRect.right << ", " << m_tEquipButtonRect.bottom << ")" << endl << endl;
// 	cout << "m_tPortionButtonRect.left, m_tPortionButtonRect.top, m_tPortionButtonRect.right, m_tPortionButtonRect.bottom :" << endl;
// 	cout << "(" << m_tPortionButtonRect.left << ", " << m_tPortionButtonRect.top << ") " << "(" << m_tPortionButtonRect.right << ", " << m_tPortionButtonRect.bottom << ")" << endl << endl;
// 
// 	cout << "m_iGold = " << m_pPlayer->GetState().iGold << endl;
#endif
	//

	// Exit 버튼
	if(PtInRect(&m_tExitButtonRect, pt2))
	{
		if(CKeyMgr::GetInstance()->OnceKeyUp(VK_LBUTTON) 
			&& m_bIsExitButtonOn == false)
		{
			m_bIsShowed = false;
			m_bIsExitButtonOn = true;
			CSoundMgr::GetInstance()->PlaySound(L"CloseWnd.mp3", CSoundMgr::CHANNEL_EFFECT);
		}
	}

	// 포션창
	if(PtInRect(&m_tPortionButtonRect, pt2))
	{
		if(CKeyMgr::GetInstance()->OnceKeyUp(VK_LBUTTON) 
			&& m_bIsPortionButtonOn == false)
		{
			CSoundMgr::GetInstance()->PlaySound(L"Button.mp3", CSoundMgr::CHANNEL_EFFECT);
			m_pImgName = L"Inventory_Portion";
			m_bIsPortionButtonOn = true;
			m_bIsEquipButtonOn = false;
		}
	}

	// 장비창
	if(PtInRect(&m_tEquipButtonRect, pt2))
	{
		if(CKeyMgr::GetInstance()->OnceKeyUp(VK_LBUTTON) 
			&& m_bIsEquipButtonOn == false)
		{
			CSoundMgr::GetInstance()->PlaySound(L"Button.mp3", CSoundMgr::CHANNEL_EFFECT);
			m_pImgName = L"Inventory_Equip";
			m_bIsEquipButtonOn = true;
			m_bIsPortionButtonOn = false;
		}
	}

	// 창 움직이기
	if(PtInRect(&m_tMoveButtonRect, pt2))
	{
		if(CKeyMgr::GetInstance()->OnceKeyDown(VK_LBUTTON) 
			&& m_bIsMoveButtonOn == false)
		{
			m_bIsMoveButtonOn = true;
		}
		if(CKeyMgr::GetInstance()->OnceKeyUp(VK_LBUTTON) 
			&& m_bIsMoveButtonOn == true)
		{
			m_bIsMoveButtonOn = false;
		}
	}
// 	else
// 	{
// 		m_bIsMoveButtonOn = false;
// 	}

	// 포션
	if(PtInRect(&m_tSpaceRect[0], pt2))
	{
		// 아이템 설명창
		if(m_bIsHPPortionInfoOn == false)
		{
			m_bIsHPPortionInfoOn = true;
		}

		// 포션 먹음
		if(CKeyMgr::GetInstance()->OnceKeyDown(VK_LBUTTON) 
			&& m_bIsHpPortionOn == false)
		{
			m_bIsHpPortionOn = true;
		}
		if(CKeyMgr::GetInstance()->OnceKeyUp(VK_LBUTTON) 
			&& m_bIsHpPortionOn == true)
		{
			if(m_pPlayer != NULL)
				dynamic_cast<CPlayer*>(m_pPlayer)->SetHp(500);
			CSoundMgr::GetInstance()->PlaySound(L"Portion.mp3", CSoundMgr::CHANNEL_EFFECT);
			m_bIsHpPortionOn = false;
		}
	}
	else
	{
		m_bIsHPPortionInfoOn = false;
	}

	if(PtInRect(&m_tSpaceRect[1], pt2))
	{
		// 아이템 설명창
		if(m_bIsMPPortionInfoOn == false)
		{
			m_bIsMPPortionInfoOn = true;
		}
		// 포션 먹음
		if(CKeyMgr::GetInstance()->OnceKeyDown(VK_LBUTTON) 
			&& m_bIsMpPortionOn == false)
		{
			m_bIsMpPortionOn = true;
		}
		if(CKeyMgr::GetInstance()->OnceKeyUp(VK_LBUTTON) 
			&& m_bIsMpPortionOn == true)
		{
			if(m_pPlayer != NULL)
				dynamic_cast<CPlayer*>(m_pPlayer)->SetMp(500);
			CSoundMgr::GetInstance()->PlaySound(L"Portion.mp3", CSoundMgr::CHANNEL_EFFECT);
			m_bIsMpPortionOn = false;
		}
	}
	else
	{
		m_bIsMPPortionInfoOn = false;
	}
}

void CInven::MoveToMouse()
{
	if(m_bIsMoveButtonOn)
	{
		POINT pt;
		POINT pt2;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt2.x = static_cast<LONG>(pt.x - g_fScrollX) - 10 + static_cast<LONG>(g_fScrollX);
		pt2.y = static_cast<LONG>(pt.y - g_fScrollY) - 21 + static_cast<LONG>(g_fScrollY);

	

		if(m_bIsOriginalpt == false)
		{
			m_ptOriginal.x = pt2.x;
			m_ptOriginal.y = pt2.y;

			m_fGapX = m_tInfo.fX - m_ptOriginal.x;
			m_fGapY = m_tInfo.fY - m_ptOriginal.y;

			m_bIsOriginalpt = true;
		}

		m_tInfo.fX = pt2.x + m_fGapX;
		m_tInfo.fY = pt2.y + m_fGapY;
	}
}

void CInven::SettingInvenPos()
{
	// 각 인벤 칸의 pos 지정
	float fCX = 35.f;
	float fCY = 35.f;
	float fStartX = m_tRect.left + 11.f + (fCX * 0.5f);
	float fStartY = m_tRect.top + 51.f + (fCY * 0.5f);

	for(int i = 0; i < 6; ++i) // 세로축
	{
		for(int j = 0; j < 4; ++j) // 가로축
		{
			int iIndex = 4 * i + j;
			m_ptSpacePos[iIndex].x = static_cast<LONG>(fStartX + j * (fCX));
			m_ptSpacePos[iIndex].y = static_cast<LONG>(fStartY + i * (fCY));
		}
	}

	// 각 인벤 칸의 rect 지정

	for(int i = 0; i < 6; ++i) // 세로축
	{
		for(int j = 0; j < 4; ++j) // 가로축
		{
			int iIndex = 4 * i + j;

			m_tSpaceRect[iIndex].left = static_cast<LONG>(m_ptSpacePos[iIndex].x - fCX * 0.5f);
			m_tSpaceRect[iIndex].right = static_cast<LONG>(m_ptSpacePos[iIndex].x + fCX * 0.5f);
			m_tSpaceRect[iIndex].top = static_cast<LONG>(m_ptSpacePos[iIndex].y - fCY * 0.5f);
			m_tSpaceRect[iIndex].bottom = static_cast<LONG>(m_ptSpacePos[iIndex].y + fCY * 0.5f);
		}
	}

}

void CInven::ItemSetting()
{
	CObj* pHPPortion = CreateItem<CHPPortion>(
		static_cast<float>(m_ptSpacePos[0].x), 	static_cast<float>(m_ptSpacePos[0].y));
	m_vecInven.push_back(pHPPortion);
//	CObjMgr::GetInstance()->AddObject(pHPPortion, OBJ_ITEM);

	CObj* pMPPortion = CreateItem<CMPPortion>(
		static_cast<float>(m_ptSpacePos[1].x), 	static_cast<float>(m_ptSpacePos[1].y));
	m_vecInven.push_back(pMPPortion);
//	CObjMgr::GetInstance()->AddObject(pMPPortion, OBJ_ITEM);

}

