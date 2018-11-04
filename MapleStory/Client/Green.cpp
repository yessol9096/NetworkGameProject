#include "StdAfx.h"
#include "Green.h"
#include "Gold.h"
#include "Accessory.h"

CGreen::CGreen(void)
{
}

CGreen::~CGreen(void)
{
}

void CGreen::Initialize( void )
{
	m_tInfo.size.cx = 60.f;
	m_tInfo.size.cy = 60.f;

	m_fSpeed = 4.f;
	//
	m_tState.iAtt = 50;
	m_tState.iHp  = 300;
	m_tState.iLevel = 15;
	m_tState.iExp = 100;
	m_tState.iMaxExp = 100;
	m_tState.iMaxHp = 300;
	m_tState.iGold = 50;

	m_fKnockBackMax = 0.2f;
	//

	m_eCurState = MONSTER_WALK;
	m_ePreState = m_eCurState;

	//
	m_dwCreateCurTime = GetTickCount();
	m_dwCreateOldTime = m_dwCreateCurTime;

	// 데미지 상태 지속시간
	m_dwDamageCurTime = GetTickCount();
	m_dwDamageOldTime = m_dwDamageCurTime;
	m_dwDamageTime = 1200;

	// 죽음 상태 지속 시간
	m_dwDeadCurTime = GetTickCount();
	m_dwDeadOldTime = m_dwDeadCurTime;
	m_dwDeadTime = 4000;

	// 이미지
	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	if(m_eDir == DIR_LEFT)
		m_pImgName = L"GreenMush_LEFT";
	else if(m_eDir == DIR_RIGHT)
		m_pImgName = L"GreenMush_RIGHT";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iScene = 1;
	m_tFrame.dwFrameSpd = 250;

	m_eRenderType = RENDER_OBJ;


	UpdateCollRect();
}

int CGreen::Update( void )
{
	if(1 == m_iPattern)
	{
		m_tState.iGold = 50;
	}
	else if(2 == m_iPattern)
	{
		m_tState.iGold = 20;
	}
 	if(true == m_bIsDead)
	{
		g_iTakedMob1++;	
		g_iExp += m_tState.iExp;
// 		CObjMgr::GetInstance()->AddObject(
//  			CAbstractFactory<CGold>::CreateGold(m_tInfo.pt.x, m_tInfo.pt.y, GOLD_0), OBJ_ITEM);	
		CSoundMgr::GetInstance()->PlaySound(L"MonsterDead.MP3", CSoundMgr::CHANNEL_EFFECT);
		
		if(m_iPattern != 3)
			CObjMgr::GetInstance()->AddObject(CreateGold<CGold>(), OBJ_ITEM);
/*		CObjMgr::GetInstance()->AddObject(CreateItem<CAccessory>(), OBJ_ITEM);*/

		return 1;
 	}

	// 방향 설정
	if(m_eDir == DIR_LEFT)
		m_pImgName = L"GreenMush_LEFT";
	else if(m_eDir == DIR_RIGHT)
		m_pImgName = L"GreenMush_RIGHT";

	// 데미지 cur time 설정
	m_dwDamageCurTime = GetTickCount();
	

	// 패턴에 따른 움직임
	MoveInPattern();
	//KnockBack();
	UpdateCollRect();
	//LineCollision();
	FrameMove();

	if(m_tState.iHp <= 0)
	{
		//m_bIsDead = true;
		m_eCurState = MONSTER_DEAD;
	}

// 	if(m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
// 	{
// 		// 죽었을 때는 스프라이트 한번 돌리고 죽을것!
// 		if(m_bIsDead)
// 		{
// 			return 1;
// 		}
// 	}

// 	if(MONSTER_DEAD == m_eCurState)
// 	{
// 		m_dwDeadCurTime = GetTickCount();
// 		if(m_dwDeadOldTime + m_dwDeadTime < m_dwDamageCurTime)
// 		{
// 			//m_dwDeadOldTime = m_dwDeadCurTime;
// 			return 1;
// 		}
// 	}


	CObj::UpdateRect();

	return 0;
}

void CGreen::Render( HDC hDc )
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
		RGB(255, 0, 255));

	// 히트박스
	if(GetAsyncKeyState('2'))
	{
		Rectangle(hDc, 
			static_cast<int>(m_tCollRect.left + g_fScrollX),
			static_cast<int>(m_tCollRect.top + g_fScrollY), 
			static_cast<int>(m_tCollRect.right + g_fScrollX),
			static_cast<int>(m_tCollRect.bottom + g_fScrollY));
	}
}

void CGreen::FrameMove()
{
	if(m_eCurState != m_ePreState)
	{
		switch(m_eCurState)
		{
		case MONSTER_STAND:
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iScene = 3;
			break;
		case MONSTER_DAMAGED:
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iScene = 2;
			break;
		case MONSTER_WALK:
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iScene = 1;
			break;
		case MONSTER_DEAD:
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iScene = 0;
			break;
		}

		m_ePreState = m_eCurState;
	}

	m_dwFrameCurTime = GetTickCount();

	if(m_dwFrameOldTime + m_tFrame.dwFrameSpd < m_dwFrameCurTime)
	{
		++(m_tFrame.iFrameStart);
		m_dwFrameOldTime = m_dwFrameCurTime;
	}

	if(m_eCurState == MONSTER_DEAD)
	{
		if(m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			m_bIsDead = true;
	}

	if(m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
	{
		m_tFrame.iFrameStart = 0;
	}

}

void CGreen::MoveInPattern()
{
	m_dwCreateCurTime = GetTickCount();

	if(DIR_LEFT == m_eDir)
	{
		switch(m_eCurState)
		{
		case MONSTER_WALK:
			{
				m_tInfo.pt.x -= m_fSpeed;

				if(m_tInfo.pt.x < 465.f)
					m_eDir = DIR_RIGHT;
			}
			break;
		case MONSTER_STAND:
			break;
		}
		
	}
	else
	{
		switch(m_eCurState)
		{
		case MONSTER_WALK:
			{
				m_tInfo.pt.x += m_fSpeed;
				if(m_tInfo.pt.x > 1390.f)
					m_eDir = DIR_LEFT;
			}
			break;
		case MONSTER_STAND:
			break;
		}

	}

	switch(m_iPattern)
	{
	case 1:
		{
			if(MONSTER_DAMAGED == m_eCurState)
			{
				if(m_dwDamageOldTime + m_dwDamageTime < m_dwDamageCurTime)
				{

					m_eCurState = MONSTER_WALK;
					m_dwDamageOldTime = m_dwDamageCurTime;
				}
				return;
			}

			if(MONSTER_DEAD == m_eCurState)
			{
				return;
			}

			else
			{
				if(m_dwCreateOldTime + 3000 < m_dwCreateCurTime)
				{
					m_eCurState = MONSTER_STAND;
				}
				if(m_dwCreateOldTime + 6000 < m_dwCreateCurTime)
				{
					m_eCurState = MONSTER_WALK;
					m_dwCreateOldTime = m_dwCreateCurTime;
					m_bIsFloorBoxColl = false;
				}

			}
			
		}
		break;
	case 2:
		{
			if(MONSTER_DAMAGED == m_eCurState)
			{
				if(m_dwDamageOldTime + m_dwDamageTime < m_dwDamageCurTime)
				{
					m_eCurState = MONSTER_WALK;
					m_dwDamageOldTime = m_dwDamageCurTime;
				}
				return;
			}
			if(MONSTER_DEAD == m_eCurState)
			{
				return;
			}

			else
			{
				if(m_dwCreateOldTime + 7000 < m_dwCreateCurTime)
				{
					m_eCurState = MONSTER_STAND;
				}
				if(m_dwCreateOldTime + 11000 < m_dwCreateCurTime)
				{
					m_eCurState = MONSTER_WALK;
					m_dwCreateOldTime = m_dwCreateCurTime;
					m_bIsFloorBoxColl = false;
				}

			}
			
		}
		break;
	case 3:
		{
			if(MONSTER_DAMAGED == m_eCurState)
			{
				if(m_dwDamageOldTime + m_dwDamageTime < m_dwDamageCurTime)
				{
					m_dwDamageOldTime = m_dwDamageCurTime;
					m_eCurState = MONSTER_WALK;
				}
				return;
			}
			if(MONSTER_DEAD == m_eCurState)
			{
				return;
			}


			else
			{
				if(m_dwCreateOldTime + 10000 < m_dwCreateCurTime)
				{
					m_eCurState = MONSTER_STAND;
				}
				if(m_dwCreateOldTime + 15000 < m_dwCreateCurTime)
				{
					m_eCurState = MONSTER_WALK;
					m_dwCreateOldTime = m_dwCreateCurTime;
					m_bIsFloorBoxColl = false;
				}

			}
			
		}
		break;
	}

}


void CGreen::Release( void )
{

}

void CGreen::LineCollision()
{
}

void CGreen::UpdateCollRect()
{
	m_tCollRect = m_tRect;


}

void CGreen::KnockBack()
{
	if(MONSTER_DAMAGED == m_eCurState)
	{
		if(DIR_RIGHT == this->GetDir())
			m_tInfo.pt.x -= m_fKnockBack * 0.5f;
		else
			m_tInfo.pt.x += m_fKnockBack * 0.5f;

		if(m_fKnockBack > m_fKnockBackMax)
		{
			m_fKnockBack = 0;
			m_eCurState = MONSTER_WALK;
		}

	}
	
}
