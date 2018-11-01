#include "StdAfx.h"
#include "MushCouple.h"
#include "Gold.h"

CMushCouple::CMushCouple(void)
{
}

CMushCouple::~CMushCouple(void)
{
}

void CMushCouple::Initialize( void )
{
	m_tInfo.fCX = 170.f;
	m_tInfo.fCY = 170.f;

	m_fSpeed = 4.f;
	//
	m_tState.iAtt = 15;
	m_tState.iHp  = 400;
	m_tState.iLevel = 20;
	m_tState.iExp = 150;
	m_tState.iMaxExp = 120;
	m_tState.iMaxHp = 400;
	m_tState.iGold = 150;
	//
	m_fKnockBackMax = 0.2f;

	m_eCurState = MONSTER_WALK;
	m_ePreState = m_eCurState;

	//
	m_dwCreateCurTime = GetTickCount();
	m_dwCreateOldTime = m_dwCreateCurTime;

	// 데미지 상태 지속 시간
	m_dwDamageCurTime = GetTickCount();
	m_dwDamageOldTime = m_dwDamageCurTime;
	m_dwDamageTime = 1200;


	// 이미지
	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	if(m_eDir == DIR_LEFT)
		m_pImgName = L"CoupleMush_LEFT";
	else if(m_eDir == DIR_RIGHT)
		m_pImgName = L"CoupleMush_RIGHT";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 13;
	m_tFrame.iScene = 1;
	m_tFrame.dwFrameSpd = 150;

	m_eRenderType = RENDER_OBJ;


	UpdateCollRect();

}

int CMushCouple::Update( void )
{
	if(true == m_bIsDead)
	{
		g_iTakedMob2++;	
		CSoundMgr::GetInstance()->PlaySound(L"MonsterDead.MP3", CSoundMgr::CHANNEL_EFFECT);
		CObjMgr::GetInstance()->AddObject(CreateGold<CGold>(), OBJ_ITEM);
		return 1;
	}

	if(m_tState.iHp <= 0)
	{
		//m_bIsDead = true;
		m_eCurState = MONSTER_DEAD;
	}


// 	if(true == m_bIsDead)
// 	{
// 		DWORD dwTime = GetTickCount();
// 
// 		if(dwTime + 3000 < GetTickCount())
// 			return 1;
// 	}
	if(m_eDir == DIR_LEFT)
		m_pImgName = L"CoupleMush_LEFT";
	else if(m_eDir == DIR_RIGHT)
		m_pImgName = L"CoupleMush_RIGHT";


	m_dwDamageCurTime = GetTickCount();

	// 패턴에 따른 움직임
	MoveInPattern();
	//KnockBack();
	UpdateCollRect();
	//LineCollision();
	FrameMove();

// 	if(m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
// 	{
// 		// 죽었을 때는 스프라이트 한번 돌리고 죽을것!
// 		if(MONSTER_DEAD == m_eCurState)
// 		{
// 			return 1;
// 		}
// 	}


	CObj::UpdateRect();

	return 0;
}

void CMushCouple::Render( HDC hDc )
{
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;


	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left + g_fScrollX),
		static_cast<int>(m_tRect.top + g_fScrollY), 
		static_cast<int>(m_tInfo.fCX),
		static_cast<int>(m_tInfo.fCY),
		pBit->GetMemDC(),
		static_cast<int>(m_tFrame.iFrameStart * m_tInfo.fCX),
		static_cast<int>(m_tFrame.iScene * m_tInfo.fCY),
		static_cast<int>(m_tInfo.fCX),
		static_cast<int>(m_tInfo.fCY),
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

void CMushCouple::FrameMove()
{
	if(m_eCurState != m_ePreState)
	{
		switch(m_eCurState)
		{
		case MONSTER_STAND:
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iScene = 0;
			break;
		case MONSTER_DAMAGED:
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iScene = 2;
			break;
		case MONSTER_WALK:
			m_tFrame.iFrameEnd = 13;
			m_tFrame.iScene = 1;
			break;
		case MONSTER_DEAD:
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iScene = 3;
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

void CMushCouple::Release( void )
{

}

void CMushCouple::MoveInPattern()
{
	m_dwCreateCurTime = GetTickCount();

	if(DIR_LEFT == m_eDir)
	{
		switch(m_eCurState)
		{
		case MONSTER_WALK:
			{
				m_tInfo.fX -= m_fSpeed;

				if(m_tInfo.fX < 465.f){
					m_eDir = DIR_RIGHT;
					m_tInfo.fX += m_fSpeed;
				}
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
				m_tInfo.fX += m_fSpeed;

				if(m_tInfo.fX > 1390.f){
					m_eDir = DIR_LEFT;
					m_tInfo.fX += m_fSpeed;
				}
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

void CMushCouple::UpdateCollRect()
{
	float fGapY = 55.f;
	float fGapX = 45.f;

	m_tCollRect.top = static_cast<LONG>(m_tRect.top + fGapY);
	m_tCollRect.bottom = static_cast<LONG>(m_tRect.bottom - fGapY);
	m_tCollRect.left = static_cast<LONG>(m_tRect.left + fGapX);
	m_tCollRect.right = static_cast<LONG>(m_tRect.right - fGapX);
}

void CMushCouple::KnockBack()
{
	if(MONSTER_DAMAGED == m_eCurState)
	{
		if(DIR_RIGHT == this->GetDir())
			m_tInfo.fX -= m_fKnockBack * 0.5f;
		else
			m_tInfo.fX += m_fKnockBack * 0.5f;

		if(m_fKnockBack > m_fKnockBackMax)
		{
			m_fKnockBack = 0;
			m_eCurState = MONSTER_WALK;
		}

	}

}
