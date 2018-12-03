#include "StdAfx.h"
#include "Player.h"
#include "Shoot.h"
#include "Swing.h"
#include "SwingEffect.h"
#include "Arrow.h"
#include "Dragon.h"
#include "Fire.h"
#include "Wing.h"

bool g_bIsSend = false;

int g_iLevel = 65;
int g_iExp = 0;

CPlayer::CPlayer(void)
	: m_bIsRopeColl(false), m_bIsJumpUp(true), m_bLineColl(true),
	m_fLineY(0.f), m_bIsSkillPressed(false), m_iPlayerFloor(1), m_fOffSetGapY(0.f), m_bIsInvincible(false)
{
}

CPlayer::~CPlayer(void)
{
}

void CPlayer::Initialize(void)
{
	// 서버 추가.
	{
		// if (내가 조종 중인 클라이언트라면) {
	//		MakingPlayer에서 g_vecplayer[g_myid]에 playerinfo를 받았다.
	//		g_vecplayer[g_myid]로부터 CPlayer::m_playerinfo를 최초 한번 받아 와야 한다.
	// }
	// else if (내가 조종할 수 없는, 다른 클라이언트라면) { 
	//		MakingPlayer에서 g_vecplayer[다른 클라이언트 id]에 playerinfo를 받았다.
	//		g_vecplayer[다른 클라이언트 id]로부터 CPlayer::m_playerinfo를 최초 한번 받아 와야 한다.

	// --------------- Process -------------------
		if (!m_IsMaster) {
			if (g_myid == 0)
				g_vecplayer[g_myid + 1].job = m_playerinfo.job;
			else if (g_myid == 1)
				g_vecplayer[g_myid - 1].job = m_playerinfo.job;
		}
	}


	//// 이 부분은 이제 안 해도 될듯..?
	//m_tInfo.pt.x = 100.f;
	//m_tInfo.pt.y = 500.f;

	//m_tInfo.size.cx = 100.f;
	//m_tInfo.size.cy = 100.f;

	// 스테이터스 초기설정
	m_tState.iAtt = 300;
	m_tState.iHp  = 30000;
	m_tState.iMp  = 8000;
	m_tState.iLevel = 88;
	m_tState.iExp = 0;
	m_tState.iMaxExp = 1000;
	m_tState.iMaxHp = 30000;
	m_tState.iMaxMp = 8000;
	m_tState.iGold = 0;

	m_bIsLeveling = false;

	// 방향
	m_eDir = DIR_RIGHT;

	// 스피드
	m_fSpeed = 7.f;
	m_fSpeedY = 4.f;

	// 점프값 조절
	m_fAngle = 90.f;
	m_fJumpAcc = 0.f;
	m_fJumpSpeed = 15.f;

	m_bIsJump = false;
	m_bIsPressed = false;

	// 오프셋
	m_fOffSet = WINCX / 2.f;
	m_fOffSetY = WINCY / 2.f;

	// 상태 설정
	m_eCurState = PLAYER_STAND;
	m_ePreState = m_eCurState;

	if (g_vecplayer[g_myid].job == JOB_STRIKER)
		m_pImgName = L"Player_LEFT";
	else
		m_pImgName = L"Captin_LEFT";

	m_dwDamageTime = GetTickCount();

	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iScene = 0;
	m_tFrame.dwFrameSpd = 50;

	m_eRenderType = RENDER_OBJ;

	// 레벨업 이펙트
	m_tLevelUpFrame.iFrameStart = 0;
	m_tLevelUpFrame.iFrameEnd = 20;
	m_tLevelUpFrame.iScene = 0;
	m_tLevelUpFrame.dwFrameSpd = 150;

	m_dwLevelUpCurTime = 0;
	m_dwLevelUpOldTime = GetTickCount();

	UpdateCollRect();

	// HP, Exp, MP Bar 생성
	CObjMgr::GetInstance()->AddObject(CreateHpBar<CPlayerHP>(), OBJ_HPBAR);
	CObjMgr::GetInstance()->AddObject(CreateExpBar<CPlayerExp>(), OBJ_HPBAR);
	CObjMgr::GetInstance()->AddObject(CreateMpBar<CPlayerMP>(), OBJ_HPBAR);
}

int CPlayer::Update(void)
{
	// 죽으면 오브젝트 삭제
	if (true == m_bIsDead)	return 1;


	// 레벨업
	if (!m_IsMaster) {
		if (g_iExp >= m_tState.iMaxExp)
		{
			m_bIsLeveling = true; // 레벨업 판정
			// 레벨업 이펙트 만들때 추후 수정..
			g_iLevel++;
			g_iExp = 0;
			m_dwLevelUpOldTime = GetTickCount();
		}
	}

	// 화면 밖으로 못나가게끔
	if (m_tInfo.pt.x <= 30)
		m_tInfo.pt.x = 30;

	switch (g_eScene)
	{
	case SCENE_FIELD:
	{
		if (m_tInfo.pt.x >= FIELDCX - 30)
			m_tInfo.pt.x = FIELDCX - 30;
	}
	break;
	case SCENE_STAGE1:
	{
		if (m_tInfo.pt.x >= HENESISCX - 30)
			m_tInfo.pt.x = HENESISCX - 30;
	}
	break;
	}

	// 씬 바뀔 때
	if (true == g_bIsSceneChange)
	{
		// 플레이어 좌표 설정
		m_tInfo.pt.x = 100.f;
		m_tInfo.pt.y = 100.f;
		// 오프셋 값 원위치
		m_fOffSet = WINCX / 2.f;

		if (g_eScene == SCENE_STAGE1)
		{
			g_fScrollX = 0.f;
			g_fScrollY = (WINCY - HENESISCY);
			m_fOffSetY = WINCY / 2.f + 100.f;
		}

		g_bIsSceneChange = false;
	}

	if (false == m_bIsInvincible)
		m_dwDamageTime = GetTickCount();

	if (true == m_bIsInvincible)
	{
		if (m_dwDamageTime + 3000 < GetTickCount())
		{
			m_dwDamageTime = GetTickCount();
			m_eCurState = m_ePreState;
			m_bIsInvincible = false;
			m_bCollMode = true;
		}

	}
	UpdateCollRect();

	if (m_IsMaster)
		Jump();

	if (m_IsMaster)
		LineCollision();

	if (m_IsMaster)
		KeyCheck();

	if (m_IsMaster)
		FrameMove();

	if (m_IsMaster) 
		Scroll();

	CObj::UpdateRect();

	// move 할 때마다 서버에게 MOVE_PACKET을 보낸다.
	if (m_IsMaster)
		SendMovePacket(); 
	// m_tInfo를 계속 쓰되, 서버에서 계속 갱신될 playerinfo를 기준으로 업데이트 해 줘야 한다.
	UpdateINFOinPLAYERINFO(); 
	
	return 0;
}

void CPlayer::Render(HDC hDc)
{
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);
	if (NULL == pBit)  return;

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

	// 히트박스
	if (GetAsyncKeyState('2'))
	{
		Rectangle(hDc,
			static_cast<int>(m_tCollRect.left + g_fScrollX),
			static_cast<int>(m_tCollRect.top + g_fScrollY),
			static_cast<int>(m_tCollRect.right + g_fScrollX),
			static_cast<int>(m_tCollRect.bottom + g_fScrollY));
	}

	// 레벨업 이펙트
	CMyBmp* pLevelUpBit = CBitmapMgr::GetInstance()->FindImage(L"LevelUp");

	int iLevelUpCX = 301;
	int iLevelUpCY = 365;
	if (NULL == pLevelUpBit) return;

	if (m_bIsLeveling)
	{
		if (m_tLevelUpFrame.iFrameStart == 100)
			m_tLevelUpFrame.iFrameStart = 0;

		TransparentBlt(hDc,
			static_cast<int>(m_tRect.left + g_fScrollX),
			static_cast<int>(m_tRect.top + g_fScrollY - 100.f),
			static_cast<int>(iLevelUpCX),
			static_cast<int>(iLevelUpCY),
			pLevelUpBit->GetMemDC(),
			static_cast<int>(m_tLevelUpFrame.iFrameStart * iLevelUpCX),
			static_cast<int>(m_tLevelUpFrame.iScene * iLevelUpCY),
			static_cast<int>(iLevelUpCX),
			static_cast<int>(iLevelUpCY),
			RGB(0, 0, 0));
	}
}


void CPlayer::Jump()
{
	if(true == m_bIsJump)
	{
		m_fJumpAcc += 0.25f;
		float fY =  m_fJumpSpeed * m_fJumpAcc * sinf(m_fAngle * PI / 180.f) 
			- 9.8f * m_fJumpAcc * m_fJumpAcc * 0.5f;

		if(fY <= 0)
			m_bIsJumpUp = false;
		else
			m_bIsJumpUp = true;

		m_tInfo.pt.y -= fY;
	}

}

void CPlayer::Scroll()
{
	switch (g_eScene)
	{
	case SCENE_FIELD:
		m_fOffSetGapY = 200.f;
		break;
	case SCENE_STAGE1:
		m_fOffSetGapY = 50.f;
		break;
	}
	// Player 좌표가 Offset에서 200만큼 이동할 경우 스크롤을 진행한다.
	if (m_tInfo.pt.x > m_fOffSet + 200.f)
	{
		g_fScrollX -= m_fSpeed;
		m_fOffSet += m_fSpeed;
	}
	else if (m_tInfo.pt.x < m_fOffSet - 200.f)
	{
		g_fScrollX += m_fSpeed;
		m_fOffSet -= m_fSpeed;
	}


	if (m_tInfo.pt.y > m_fOffSetY + m_fOffSetGapY)
	{
		g_fScrollY -= m_fSpeedY;
		m_fOffSetY += m_fSpeedY;
	}
	else if (m_tInfo.pt.y < m_fOffSetY - m_fOffSetGapY)
	{
		g_fScrollY += m_fSpeedY;
		m_fOffSetY -= m_fSpeedY;
	}

	if (g_fScrollX > 0)
		g_fScrollX = 0;
	if (g_fScrollY > 0)
		g_fScrollY = 0.f;

	// 필드일 때 스크롤 막기
	if (SCENE_FIELD == g_eScene)
	{
		if (g_fScrollX < WINCX - FIELDCX)
			g_fScrollX = WINCX - FIELDCX;
		if (g_fScrollY < WINCY - FIELDCY)
			g_fScrollY = WINCY - FIELDCY;
	}

	// 헤네시스일때 스크롤 막기
	if (SCENE_STAGE1 == g_eScene)
	{
		if (g_fScrollY < WINCY - HENESISCY)
			g_fScrollY = WINCY - HENESISCY;

		if (g_fScrollX < WINCX - HENESISCX)
			g_fScrollX = WINCX - HENESISCX;
	}
}

void CPlayer::KeyCheck()
{
	if (m_IsMaster == false)
		return;

	// 플레이어 기본 동작
	if(CKeyMgr::GetInstance()->StayKeyDown(VK_LEFT))
	{
		m_tInfo.pt.x -= m_fSpeed;
		m_eDir = DIR_LEFT;
		if(PLAYER_DAMAGED != m_eCurState)
			m_eCurState = PLAYER_WALK;
		m_bIsRopeColl = false;
		if (g_vecplayer[g_myid].job == JOB_STRIKER)
			m_pImgName = L"Player_LEFT";
		else
			m_pImgName = L"Captin_LEFT";

		g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->StayKeyDown(VK_RIGHT))
	{
		m_tInfo.pt.x += m_fSpeed;
		m_eDir = DIR_RIGHT;
		if(PLAYER_DAMAGED != m_eCurState)
			m_eCurState = PLAYER_WALK;
		m_bIsRopeColl = false;
		if (g_vecplayer[g_myid].job == JOB_STRIKER)
			m_pImgName = L"Player_RIGHT";
		else
			m_pImgName = L"Captin_RIGHT";

		g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->StayKeyDown(VK_DOWN) && !m_bIsRopeColl)
	{
		m_eCurState = PLAYER_CRAWL;

		if(CKeyMgr::GetInstance()->StayKeyDown(VK_CONTROL))
		{
			m_eCurState = PLAYER_CRAWLATTACK;
			m_tFrame.dwFrameSpd = 50;
		}

		g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->StayKeyDown(VK_UP) && m_bIsRopeColl)
	{
		m_eCurState = PLAYER_ROPE;
		if (g_vecplayer[g_myid].job == JOB_STRIKER)
			m_pImgName = L"Player_ROPE";
		else
			m_pImgName = L"Captin_ROPE";
		m_tInfo.pt.y -= m_fSpeedY;

		if(CKeyMgr::GetInstance()->OnceKeyDown(VK_SPACE))
		{
			m_eCurState = PLAYER_JUMP;
			if (g_vecplayer[g_myid].job == JOB_STRIKER)
				m_pImgName = L"Player_LEFT";
			else
				m_pImgName = L"Captin_LEFT";
			m_bIsRopeColl = false;
		}

		g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->StayKeyDown(VK_DOWN) && m_bIsRopeColl)
	{
		m_eCurState = PLAYER_ROPE;
		if (g_vecplayer[g_myid].job == JOB_STRIKER)
			m_pImgName = L"Player_ROPE";
		else
			m_pImgName = L"Captin_ROPE";
		m_tInfo.pt.y += m_fSpeedY;

		g_bIsSend = true;
	}

	// 스킬
	else if(CKeyMgr::GetInstance()->OnceKeyUp('Q'))
	{
		m_eCurState = PLAYER_SWING;
		CObjMgr::GetInstance()->AddObject(CreateSkill<CSwing>(), OBJ_SKILL_SWING);
		CObjMgr::GetInstance()->AddObject(CreateEffect<CSwingEffect>(), OBJ_EFFECT);

		g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->OnceKeyUp('W'))
	{
		m_eCurState = PLAYER_SHOOT;
		CObjMgr::GetInstance()->AddObject(CreateSkill<CShoot>(), OBJ_SKILL_SHOOT);
		CObjMgr::GetInstance()->AddObject(CreateArrow<CArrow>(0.f, 0.f, ARROW_BASIC, 0), OBJ_ARROW);
		CSoundMgr::GetInstance()->PlaySound(L"Skill_Shoot.MP3", CSoundMgr::CHANNEL_SKILL);

		g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->OnceKeyUp('E'))
	{
		m_eCurState = PLAYER_SHOOT;
		CObjMgr::GetInstance()->AddObject(CreateDragon<CDragon>(), OBJ_SKILL_DRAGON);

		CSoundMgr::GetInstance()->PlaySound(L"Skill_Dragon.MP3", CSoundMgr::CHANNEL_SKILL);
		for(int i = 0; i < 5; ++i)
		{
			float fX = 15.f;
			float fY = 10.f;
			float fY2 = 2.f;
			CObjMgr::GetInstance()->AddObject(CreateArrow<CArrow>(i * fX, (i - fY2) * fY, ARROW_DRAGON, i), OBJ_ARROW);
		}

		m_tState.iMp -= 100;

		g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->OnceKeyUp('R'))
	{
		m_eCurState = PLAYER_SHOOT;
		CObjMgr::GetInstance()->AddObject(CreateFire<CFire>(), OBJ_SKILL_FIRE);
		CSoundMgr::GetInstance()->PlaySound(L"Skill_Fire.wav", CSoundMgr::CHANNEL_SKILL);
		m_tState.iMp -= 100;

		g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->OnceKeyUp('A'))
	{
		m_eCurState = PLAYER_SHOOT;
		CObjMgr::GetInstance()->AddObject(CreateWing<CWing>(), OBJ_SKILL_WING);
		CSoundMgr::GetInstance()->PlaySound(L"Skill_Wing.wav", CSoundMgr::CHANNEL_SKILL);
		m_tState.iMp -= 500;

		g_bIsSend = true;
	}
	else
	{
		if(!m_bIsRopeColl && PLAYER_SHOOT != m_eCurState && PLAYER_SWING != m_eCurState && 
			PLAYER_DAMAGED != m_eCurState && PLAYER_JUMP != m_eCurState)
			m_eCurState = PLAYER_STAND;
	}

	if(CKeyMgr::GetInstance()->OnceKeyUp(VK_SPACE) && !m_bIsPressed)
	{
		m_eCurState = PLAYER_JUMP;
		m_fAngle = 90.f;
		if(PLAYER_DAMAGED != m_eCurState && PLAYER_JUMP != m_eCurState)
		{
			m_eCurState = PLAYER_STAND;
		}
		m_bIsJump = true;
		m_bIsPressed = true;
		m_bIsRopeColl = false;
		CSoundMgr::GetInstance()->PlaySound(L"Player_Jump.mp3", CSoundMgr::CHANNEL_PLAYER);
		//m_pImgName = L"Player_RIGHT";

		g_bIsSend = true;
	}
	else
	{
		m_bIsPressed = false;
		m_tFrame.dwFrameSpd = 150;
	}
}

void CPlayer::FrameMove()
{
	// 현재 상태가 바뀌면! 상태에 따른 애니메이션이 바뀌어야
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case PLAYER_STAND:
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iScene = 0;
			break;
		case PLAYER_JUMP:
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iScene = 6;
			break;
		case PLAYER_WALK:
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iScene = 1;
			break;
		case PLAYER_ATTACK:
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iScene = 2;
			break;
		case PLAYER_SWING:
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iScene = 3;
			break;
		case PLAYER_CRAWLATTACK:
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iScene = 4;
			break;
		case PLAYER_CRAWL:
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iScene = 5;
			break;
		case PLAYER_SHOOT:
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iScene = 6;
			break;
		case PLAYER_DAMAGED:
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iScene = 7;
			break;
		case PLAYER_ROPE:
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iScene = 0;
			break;
		}

		m_ePreState = m_eCurState;


		if (m_eCurState != PLAYER_ROPE)
			m_bIsRopeColl = false;
	}

	m_dwFrameCurTime = GetTickCount();

	if (m_dwFrameOldTime + m_tFrame.dwFrameSpd < m_dwFrameCurTime)
	{
		++(m_tFrame.iFrameStart);
		m_dwFrameOldTime = m_dwFrameCurTime;
	}

	if (m_eCurState == PLAYER_DAMAGED)
	{
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			m_eCurState = PLAYER_STAND;
	}

	if (m_eCurState != PLAYER_ROPE)
	{
		if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
		{
			// 스킬 상태시엔 스프라이트 한번 돌리고 IDLE 상태로 둘것.
			if (PLAYER_SWING == m_eCurState || PLAYER_SHOOT == m_eCurState)
			{

				m_eCurState = PLAYER_STAND;
				m_tFrame.iFrameStart = 0;
				return;
			}
			m_tFrame.iFrameStart = 0;
		}
	}
	else
	{
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			// 스킬 상태시엔 스프라이트 한번 돌리고 IDLE 상태로 둘것.
			if (PLAYER_SWING == m_eCurState || PLAYER_SHOOT == m_eCurState)
			{

				m_eCurState = PLAYER_STAND;
				m_tFrame.iFrameStart = 0;
				return;
			}
			m_tFrame.iFrameStart = 0;
		}
	}

	// 레벨업 이펙트 프레임 돌리기
	// 레벨링 중이 아니더라도 프레임은 계속 돌고 있다
	m_dwLevelUpCurTime = GetTickCount();

	if (m_dwLevelUpOldTime + m_tLevelUpFrame.dwFrameSpd > m_dwLevelUpCurTime)
	{
		++m_tLevelUpFrame.iFrameStart;
		m_dwLevelUpOldTime = m_dwLevelUpCurTime;
	}

	// 프레임을 돌리던 중 레벨링변수가 true이면 start를 100으로 세팅 (위에 render에서 처리해준다)
	if (m_tLevelUpFrame.iFrameStart >= m_tLevelUpFrame.iFrameEnd && m_bIsLeveling)
	{
		m_tLevelUpFrame.iFrameStart = 101;
		// 		g_iExp = 0;
		// 		m_tState.iMaxExp += 1000;
		// 		m_tState.iMaxHp += 3000;
		// 		m_tState.iHp = m_tState.iMaxHp;
		// 		g_iLevel++;
				//m_bIsLeveling = false;
	}
	// 레벨링 변수가 false이면 start를 100으로 세팅해버린다
	if (m_tLevelUpFrame.iFrameStart >= m_tLevelUpFrame.iFrameEnd && !m_bIsLeveling)
	{
		m_tLevelUpFrame.iFrameStart = 100;
	}
	// 	if(m_tLevelUpFrame.iFrameStart >= m_tLevelUpFrame.iFrameEnd
	// 		&& m_bIsLeveling)
	// 	{
	// 		m_tLevelUpFrame.iFrameStart = 101;
	// 	}
	// 	if(m_tLevelUpFrame.iFrameStart >= m_tLevelUpFrame.iFrameEnd
	// 		&& !m_bIsLeveling)
	// 	{
	// 		m_tLevelUpFrame.iFrameStart = 100;
	// 	}
}

void CPlayer::UpdateCollRect()
{
	// 발밑 기준으로 잡을 거기 때문에 CollRect.bottom을 먼저 설정하고 top 설정할것
	float fCollRectCX = 45.f;
	float fCollRectCY = 70.f;

	m_tCollRect.left = static_cast<LONG>(m_tInfo.pt.x - (fCollRectCX / 2.f));
	m_tCollRect.right = static_cast<LONG>(m_tInfo.pt.x + (fCollRectCX / 2.f));
	m_tCollRect.bottom = static_cast<LONG>(m_tInfo.pt.y + (fCollRectCY / 2.f));
	m_tCollRect.top = static_cast<LONG>(m_tInfo.pt.y - (fCollRectCY / 2.f));
}

void CPlayer::Release(void)
{

}

void CPlayer::LineCollision()
{
	float fY = 0;
	bool bLineCol = CLineMgr::GetInstance()->LineCollision(this, m_tInfo.pt.x, &fY);

	if(m_bIsJump)
	{
		if(m_tInfo.pt.y >= fY && bLineCol)
		{
			m_bIsJump = false;
			m_fJumpAcc = 0.f;
			m_tInfo.pt.y = fY;
			m_eCurState = PLAYER_STAND;
		}
	}
	else if(bLineCol && !m_bIsRopeColl)
	{
  		if(1 != m_iPlayerFloor)
 			return;
		m_tInfo.pt.y = fY;
	}

	if(!bLineCol)
		m_iPlayerFloor = 1;
}

bool b = true;	// 디버깅 용

void CPlayer::SendMovePacket()
{
	// 1201.
	// Server에게 내 playerinfo를 send한다. (CS_PACKET_PLAYERINFO_MOVE)
	if (g_bIsSend) {
		g_bIsSend = false;

		b = !b;
		cout << "Server에게 내 playerinfo를 send" << b << endl;

		// 1. 클라의 g_vecplayer[g_myid]에 정보를 갱신한다. 
		// 2. 보낼 공간 playerinfo를 만든다.
		// 3. playerinfo에 내 위치, frame 정보, state를 담는다.
		// 4. playerinfo를 서버에 send 한다.

		// ----------------------Progress-------------------------
		// 1. 클라의 g_vecplayer[g_myid]에 정보를 갱신한다. 
		{
			g_vecplayer[g_myid].pt.x = m_tInfo.pt.x;
			g_vecplayer[g_myid].pt.y = m_tInfo.pt.y;
			g_vecplayer[g_myid].frame = m_tInfo.frame;
			g_vecplayer[g_myid].state = m_eCurState;
		}
		// 2. 보낼 공간 playerinfo를 만든다.
		PLAYERINFO playerinfo;
		// 3. playerinfo에 내 위치, frame 정보, state를 담는다.
		{
			memcpy(&playerinfo, &(g_vecplayer[g_myid]), sizeof(PLAYERINFO));
		}
		// 4. playerinfo를 서버에 send 한다.
		{
			char buf[BUFSIZE] = {};
			// 고정 길이
			PACKETINFO packetinfo;
			packetinfo.id = g_myid;
			packetinfo.size = sizeof(PLAYERINFO);
			packetinfo.type = CS_PACKET_PLAYERINFO_MOVE;
			memcpy(buf, &packetinfo, sizeof(packetinfo));
			g_retval = send(g_sock, buf, BUFSIZE, 0);
			if (g_retval == SOCKET_ERROR) {
				MessageBox(g_hWnd, L"send()", L"send - 고정 - CS_PACKET_PLAYERINFO_MOVE", MB_OK);
				g_bIsProgramEnd = true;	// 프로그램 종료
			}

			// 가변 길이
			ZeroMemory(buf, sizeof(buf));
			memcpy(buf, &playerinfo, sizeof(playerinfo));
			g_retval = send(g_sock, buf, BUFSIZE, 0);
			if (g_retval == SOCKET_ERROR) {
				MessageBox(g_hWnd, L"send()", L"send - 가변 - CS_PACKET_PLAYERINFO_MOVE", MB_OK);
				g_bIsProgramEnd = true;	// 프로그램 종료
			}
		}
	}
}

void CPlayer::UpdateINFOinPLAYERINFO()
{
	// 이거 아닌 거 같아;;
////	m_tInfo.frame = m_playerinfo.frame; 애니메이션 동기화 구현하고나서 하는걸로             
//	m_tInfo.hp = m_playerinfo.hp;
//	m_tInfo.pt = m_playerinfo.pt;
//	m_tInfo.size = m_playerinfo.size;
//	m_eCurState = m_playerinfo.state;

	int id{ 0 };
	if (m_IsMaster) {
		id = g_myid;
	}
	else {
		if (g_myid == 0)
			id = 1;
		else
			id = 0;
	}

	// m_tInfo.frame = g_vecplayer[g_myid].frame;
	m_tInfo.hp = g_vecplayer[id].hp;
	m_tInfo.pt = g_vecplayer[id].pt;
	m_tInfo.size = g_vecplayer[id].size;
	m_eCurState = g_vecplayer[id].state;

}
