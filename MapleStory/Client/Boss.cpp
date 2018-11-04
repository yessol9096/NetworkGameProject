#include "StdAfx.h"
#include "Boss.h"
#include "BossHPBar.h"

CBoss::CBoss(void)
{
}

CBoss::~CBoss(void)
{
}

void CBoss::Initialize( void )
{
	m_tInfo.size.cx = 1260.f;
	m_tInfo.size.cy = 577.f;

	m_fSpeed = 4.f;
	
	// 무브 패턴을 위해서 만들어진 시간 받기
	m_dwCreateCurTime = GetTickCount();
	m_dwCreateOldTime = m_dwCreateCurTime;

	// 스테이터스
	m_tState.iAtt = 100;
	m_tState.iHp  = 70000;
	m_tState.iLevel = 70;
	m_tState.iExp = 7000;
	m_tState.iMaxExp = 7000;
	m_tState.iMaxHp = m_tState.iHp;
	m_tState.iGold = 5000;

	// 상태
	m_eCurState = MONSTER_WALK;
	m_ePreState = m_eCurState;

	m_eDir = DIR_LEFT;

	m_ePattern = BOSS_IDLE;


	// 데미지 상태 지속시간
	m_dwDamageCurTime = GetTickCount();
	m_dwDamageOldTime = m_dwDamageCurTime;
	m_dwDamageTime = 1400;

	// 이미지
	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_pImgName = L"Stand_LEFT";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.iScene = 0;
	m_tFrame.dwFrameSpd = 150;

	// 렌더 타입
	m_eRenderType = RENDER_OBJ;

	// 충돌박스 설정
	UpdateCollRect();

	// HPBar 생성
	CObjMgr::GetInstance()->AddObject(CreateHpBar<BossHPBar>(), OBJ_HPBAR);
}

int CBoss::Update( void )
{
// #ifdef _DEBUG
// 	 	system("cls");
// 	 	cout << "m_ePattern : " << m_ePattern << endl;
// 	 	cout << "m_eCurState : " << m_eCurState << endl;
// 	 	cout << "m_tState.iHp : " << m_tState.iHp << endl;
// 		cout << "m_tFrame.iFrameStart: " << m_tFrame.iFrameStart << endl;
// 		cout << "m_tSkillRect.top: " << m_tSkillRect.top << endl; 
// 		cout << "m_tSkillRect.bottom: " <<	m_tSkillRect.bottom << endl;
// 		cout << "m_tSkillRect.right:  " << m_tSkillRect.right << endl;
// 		cout << "m_tSkillRect.left: " << m_tSkillRect.left  << endl;
// 
// #endif			
	// 방향 설정m_tSkillRect.right 

	// 데미지 cur time 설정
	m_dwDamageCurTime = GetTickCount();

	// 패턴에 따른 움직임
	MoveInPattern();
	// 충돌박스 세팅
	UpdateCollRect();
	// 프레임 돌리기
	FrameMove();

	if(m_bIsDead)
	{
		float fSpeed = 3.f;


//		CObjMgr::GetInstance()->AddObject(CreateGoldBoss<CGold>(fSpeed, 100.f, true), OBJ_ITEM);
		for(int i = 1; i < 10; ++i)
		{
			CObjMgr::GetInstance()->AddObject(CreateGoldBoss<CGold>
				(i * fSpeed, 90.f - i * fSpeed, true, false), OBJ_ITEM);
		}
		for(int i = 1; i < 10; ++i)
		{
			CObjMgr::GetInstance()->AddObject(CreateGoldBoss<CGold>
				(i * (-fSpeed), 90.f + i * fSpeed, true, true), OBJ_ITEM);
		}
		return 1;
	}


	// 컴뱃 패턴 나누기
	if(m_tState.iHp >= 70000)
		m_ePattern = BOSS_IDLE;
	else if(m_tState.iHp <= 69999 && m_tState.iHp >= 50000)
	{
		m_ePattern = BOSS_COMBAT1;
	}
	else if(m_tState.iHp <= 49999 && m_tState.iHp >= 30000)
	{
		m_ePattern = BOSS_COMBAT2;
	}
	else if(m_tState.iHp <= 29999 && m_tState.iHp > 1)
	{
		m_ePattern = BOSS_COMBAT3;
	}


	// 체력 0이 되면 죽음
	if(m_tState.iHp <= 0)
	{
		m_eCurState = MONSTER_DEAD;
	}

	CObj::UpdateRect();

	return 0;
}

void CBoss::Render( HDC hDc )
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
		RGB(255, 250, 255));

	// 히트박스
	if(GetAsyncKeyState('2'))
	{
		Rectangle(hDc, 
			static_cast<int>(m_tCollRect.left + g_fScrollX),
			static_cast<int>(m_tCollRect.top + g_fScrollY), 
			static_cast<int>(m_tCollRect.right + g_fScrollX),
			static_cast<int>(m_tCollRect.bottom + g_fScrollY));

		Rectangle(hDc, 
			static_cast<int>(m_tSkillRect.left + g_fScrollX),
			static_cast<int>(m_tSkillRect.top + g_fScrollY), 
			static_cast<int>(m_tSkillRect.right + g_fScrollX),
			static_cast<int>(m_tSkillRect.bottom + g_fScrollY));
	}
}

void CBoss::FrameMove()
{
	if(m_eCurState != m_ePreState)
	{
		if(m_eDir == DIR_LEFT)
		{
			switch(m_eCurState)
			{
			case MONSTER_STAND:
				m_pImgName = L"Stand_LEFT";
				m_tFrame.iFrameEnd = 11;
				m_tFrame.iScene = 0;
				break;
			case MONSTER_WALK:
				m_pImgName = L"Walk_LEFT";
				m_tFrame.iFrameEnd = 6;
				break;
			case MONSTER_ATTACK1:
				m_pImgName = L"Attack1_LEFT";
				m_tFrame.iFrameEnd = 22;
				break;
			case MONSTER_ATTACK2:
				m_pImgName = L"Attack2_LEFT";
				m_tFrame.iFrameEnd = 22;
				break;
			case MONSTER_SKILL1:
				m_pImgName = L"Skill1_LEFT";
				m_tFrame.iFrameEnd = 22;
				break;
			case MONSTER_SKILL2:
				m_pImgName = L"Skill2_LEFT";
				m_tFrame.iFrameEnd = 17;
				break;
			case MONSTER_SKILL3:
				m_pImgName = L"Skill3_LEFT";
				m_tFrame.iFrameEnd = 22;
				break;
			case MONSTER_SKILL4:
				m_pImgName = L"Skill4_LEFT";
				m_tFrame.iFrameEnd = 22;
				break;
			case MONSTER_SKILL5:
				m_pImgName = L"Skill5_LEFT";
				m_tFrame.iFrameEnd = 20;
				break;
			case MONSTER_DAMAGED:
				m_pImgName = L"Damaged_LEFT";
				m_tFrame.iFrameEnd = 0;
				break;
			case MONSTER_DEAD:
				m_pImgName = L"Dead_LEFT";
				m_tFrame.iFrameEnd = 0;
				break;

			}
		}

		else
		{
			switch(m_eCurState)
			{
			case MONSTER_STAND:
				m_pImgName = L"Stand_RIGHT";
				m_tFrame.iFrameEnd = 11;
				m_tFrame.iScene = 0;
				break;
			case MONSTER_WALK:
				m_pImgName = L"Walk_RIGHT";
				m_tFrame.iFrameEnd = 6;
				break;
			case MONSTER_ATTACK1:
				m_pImgName = L"Attack1_RIGHT";
				m_tFrame.iFrameEnd = 22;
				break;
			case MONSTER_ATTACK2:
				m_pImgName = L"Attack2_RIGHT";
				m_tFrame.iFrameEnd = 22;
				break;
			case MONSTER_SKILL1:
				m_pImgName = L"Skill1_RIGHT";
				m_tFrame.iFrameEnd = 22;
				break;
			case MONSTER_SKILL2:
				m_pImgName = L"Skill2_RIGHT";
				m_tFrame.iFrameEnd = 17;
				break;
			case MONSTER_SKILL3:
				m_pImgName = L"Skill3_RIGHT";
				m_tFrame.iFrameEnd = 22;
				break;
			case MONSTER_SKILL4:
				m_pImgName = L"Skill4_RIGHT";
				m_tFrame.iFrameEnd = 22;
				break;
			case MONSTER_SKILL5:
				m_pImgName = L"Skill5_RIGHT";
				m_tFrame.iFrameEnd = 20;
				break;
			case MONSTER_DAMAGED:
				m_pImgName = L"Damaged_RIGHT";
				m_tFrame.iFrameEnd = 0;
				break;
			case MONSTER_DEAD:
				m_pImgName = L"Dead_RIGHT";
				m_tFrame.iFrameEnd = 0;
				break;
			}

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

	// 보스 프레임 스타트 부분에 따라서 효과음 달리 하기
	if(m_tFrame.iFrameStart == 10)
	{
		if(m_eCurState == MONSTER_ATTACK1
			|| m_eCurState == MONSTER_SKILL5)
		{
			CSoundMgr::GetInstance()->PlaySound(L"Boss_Skill.wav", CSoundMgr::CHANNEL_EFFECT);
		}

		else if(m_eCurState == MONSTER_ATTACK2)
		{
			CSoundMgr::GetInstance()->PlaySound(L"Boss_Skill2.wav", CSoundMgr::CHANNEL_EFFECT);

		}
	}

}

void CBoss::Release( void )
{

}

void CBoss::MoveInPattern()
{
	// 죽으면 패턴이고 뭐고 없음
	if(MONSTER_DEAD == m_eCurState)
	{
		return;
	}
// 	if(MONSTER_DAMAGED == m_eCurState)
// 	{
// 		return;
// 	}

	m_dwCreateCurTime = GetTickCount();

	//////////////////////////////////////////////////////////////////////
	/////////////////////////// Move 패턴
	// 왼쪽 방향일 때
	if(DIR_LEFT == m_eDir)
	{
		switch(m_eCurState)
		{
		case MONSTER_WALK:
			{
				m_pImgName = L"Walk_LEFT";
				if(m_tInfo.pt.x - 100.f < 100.f)
				{
					m_tInfo.pt.x = m_tInfo.pt.x + m_tInfo.size.cx * 0.6f;
					m_eDir = DIR_RIGHT;
					m_pImgName = L"Walk_RIGHT";
				}
				else
					m_tInfo.pt.x -= m_fSpeed;
				m_tFrame.iFrameEnd = 6;
			}
			break;
		case MONSTER_STAND:
			break;
		}

	}
	// 오른쪽 방향일 때
	else
	{
		switch(m_eCurState)
		{
		case MONSTER_WALK:
			{
				m_pImgName = L"Walk_RIGHT";

				if(m_tInfo.pt.x + 100.f > BOSSMAPCX/* - 100.f*/)
				{
					m_tInfo.pt.x = m_tInfo.pt.x - m_tInfo.size.cx * 0.6f;
					m_eDir = DIR_LEFT;
					m_pImgName = L"Walk_LEFT";
				}
				else
					m_tInfo.pt.x += m_fSpeed;

				m_tFrame.iFrameEnd = 6;
			}
			break;
		case MONSTER_STAND:
			break;
		}
	}

	///////////////////////////////////////////////////////////////////
	// 데미지 입었을 땐 일정 시간 뒤에 다시 WALk 상태로
	if(MONSTER_DAMAGED == m_eCurState)
	{
		if(m_eDir == DIR_LEFT)
			m_pImgName = L"Damaged_LEFT";
		else
			m_pImgName = L"Damaged_RIGHT";
		if(m_dwDamageOldTime + m_dwDamageTime < m_dwDamageCurTime)
		{

			m_eCurState = MONSTER_WALK;
			m_dwDamageOldTime = m_dwDamageCurTime;
		}
		return;
	}

	// 데미지 상태가 아니라면
	else
	{
		////////////////////////////////////////////////////////////////
		// 보스 전투패턴
		if(m_ePattern == BOSS_IDLE)
		{
			if(m_dwCreateOldTime + 3000 < m_dwCreateCurTime)
			{
				m_eCurState = MONSTER_STAND;
			}
			if(m_dwCreateOldTime + 6000 < m_dwCreateCurTime)
			{
				m_eCurState = MONSTER_WALK;
				m_dwCreateOldTime = m_dwCreateCurTime;
			}

		}
		else if(m_ePattern == BOSS_COMBAT1)
		{
			if(m_dwCreateOldTime + 4000 < m_dwCreateCurTime)
			{
				m_eCurState = MONSTER_WALK;		
			}
			if(m_dwCreateOldTime + 8000 < m_dwCreateCurTime)
			{
				m_eCurState = MONSTER_SKILL1;
				if(m_eDir == DIR_LEFT)
					m_pImgName = L"Skill1_LEFT";
				else
					m_pImgName = L"Skill1_RIGHT";
			}
			if(m_dwCreateOldTime + 12000 < m_dwCreateCurTime)
			{
				m_eCurState = MONSTER_ATTACK2;
				if(m_eDir == DIR_LEFT)
					m_pImgName = L"Attack2_LEFT";
				else
					m_pImgName = L"Attack2_RIGHT";
				m_dwCreateOldTime = m_dwCreateCurTime;
			}
		}

		else if(m_ePattern == BOSS_COMBAT2)
		{
			if(m_dwCreateOldTime + 3000 < m_dwCreateCurTime)
			{
				m_eCurState = MONSTER_WALK;
			}
			if(m_dwCreateOldTime + 8000 < m_dwCreateCurTime)
			{
				m_eCurState = MONSTER_SKILL2;
				if(m_eDir == DIR_LEFT)
					m_pImgName = L"Skill2_LEFT";
				else
					m_pImgName = L"Skill2_RIGHT";
			}
			if(m_dwCreateOldTime + 12000 < m_dwCreateCurTime)
			{
				m_eCurState = MONSTER_ATTACK1;
				if(m_eDir == DIR_LEFT)
					m_pImgName = L"Attack1_LEFT";
				else
					m_pImgName = L"Attack1_RIGHT";

				m_dwCreateOldTime = m_dwCreateCurTime;
			}
		}

		else if(m_ePattern == BOSS_COMBAT3)
		{
			if(m_dwCreateOldTime + 4000 < m_dwCreateCurTime)
			{
				m_eCurState = MONSTER_WALK;
			}
			if(m_dwCreateOldTime + 8000 < m_dwCreateCurTime)
			{
				m_eCurState = MONSTER_SKILL3;
				if(m_eDir == DIR_LEFT)
					m_pImgName = L"Skill3_LEFT";
				else
					m_pImgName = L"Skill3_RIGHT";
			}
			if(m_dwCreateOldTime + 12000 < m_dwCreateCurTime)
			{
				m_eCurState = MONSTER_SKILL5;
				if(m_eDir == DIR_LEFT)
					m_pImgName = L"Skill5_LEFT";
				else
					m_pImgName = L"Skill5_RIGHT";
				m_dwCreateOldTime = m_dwCreateCurTime;
			}
		}
	}

}

void CBoss::UpdateCollRect()
{
	if(DIR_LEFT == m_eDir)
	{
		m_tCollRect.top =  static_cast<LONG>(m_tInfo.pt.y + 107.f);
		m_tCollRect.bottom = static_cast<LONG>(m_tInfo.pt.y + 260.f);
		m_tCollRect.left = static_cast<LONG>(m_tInfo.pt.x + 352.f);
		m_tCollRect.right = static_cast<LONG>(m_tInfo.pt.x + 500.f);
	}
	else
	{
		m_tCollRect.top = static_cast<LONG>(m_tInfo.pt.y + 107.f);
		m_tCollRect.bottom = static_cast<LONG>(m_tInfo.pt.y + 260.f);
		m_tCollRect.left = static_cast<LONG>(m_tInfo.pt.x - 490.f);
		m_tCollRect.right = static_cast<LONG>(m_tInfo.pt.x - 340.f);
	}

	// 스킬 히트 박스
	if(m_eCurState == MONSTER_ATTACK1)
	{
		if(m_tFrame.iFrameStart >= 13 && m_tFrame.iFrameStart <= 16)
		{
			m_tSkillRect.top = static_cast<LONG>(m_tInfo.pt.y);
			m_tSkillRect.bottom = static_cast<LONG>(m_tInfo.pt.y + 280.f);

			if(m_eDir == DIR_LEFT)
			{
				m_tSkillRect.left = static_cast<LONG>(m_tInfo.pt.x - 300.f);
				m_tSkillRect.right = static_cast<LONG>(m_tInfo.pt.x);
			}
			else if(m_eDir == DIR_RIGHT)
			{
				m_tSkillRect.left = static_cast<LONG>(m_tInfo.pt.x);
				m_tSkillRect.right = static_cast<LONG>(m_tInfo.pt.x + 300.f);
			}
		}

		else
		{
			m_tSkillRect.top = 0;
			m_tSkillRect.bottom = 0;
			m_tSkillRect.left = 0;
			m_tSkillRect.right = 0;
		}
	}

	else if(m_eCurState == MONSTER_ATTACK2)
	{
		if(m_tFrame.iFrameStart >= 15 && m_tFrame.iFrameStart <= 16)
		{
			m_tSkillRect.top = static_cast<LONG>(m_tInfo.pt.y + 95.f);
			m_tSkillRect.bottom = static_cast<LONG>(m_tInfo.pt.y + 275.f);

			m_tSkillRect.left = static_cast<LONG>(m_tInfo.pt.x - 150.f);
			m_tSkillRect.right = static_cast<LONG>(m_tInfo.pt.x + 150.f);
		}
		else
		{
			m_tSkillRect.top = 0;
			m_tSkillRect.bottom = 0;
			m_tSkillRect.left = 0;
			m_tSkillRect.right = 0;
		}
	}


	else if(m_eCurState == MONSTER_SKILL5)
	{
		if(m_tFrame.iFrameStart >= 13 && m_tFrame.iFrameStart <= 16)
		{
			m_tSkillRect.top = static_cast<LONG>(m_tInfo.pt.y);
			m_tSkillRect.bottom = static_cast<LONG>(m_tInfo.pt.y + 290.f);

			if(m_eDir == DIR_LEFT)
			{
				m_tSkillRect.left = static_cast<LONG>(m_tInfo.pt.x - 350.f);
				m_tSkillRect.right = static_cast<LONG>(m_tInfo.pt.x);
			}
			else if(m_eDir == DIR_RIGHT)
			{
				m_tSkillRect.left = static_cast<LONG>(m_tInfo.pt.x);
				m_tSkillRect.right = static_cast<LONG>(m_tInfo.pt.x + 350.f);
			}
			
		}
		else
		{
			m_tSkillRect.top = 0;
			m_tSkillRect.bottom = 0;
			m_tSkillRect.left = 0;
			m_tSkillRect.right = 0;
		}
	}

// 	else
// 	{
// 		m_tSkillRect.top = 0;
// 		m_tSkillRect.bottom = 0;
// 		m_tSkillRect.left = 0;
// 		m_tSkillRect.right = 0;
// 	}
}






void CBoss::KnockBack()
{

}
