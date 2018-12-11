#include "StdAfx.h"
#include "Player.h"
#include "Shoot.h"
#include "Swing.h"
#include "SwingEffect.h"
#include "Arrow.h"
#include "Dragon.h"
#include "Fire.h"
#include "Wing.h"

// ������ ����ȭ
CRITICAL_SECTION cs2;

bool g_bIsSend = false;

int g_iLevel = 65;
int g_iExp = 0;

CPlayer::CPlayer(void)
	: m_bIsRopeColl(false), m_bIsJumpUp(true), m_bLineColl(true),
	m_fLineY(0.f), m_bIsSkillPressed(false), m_iPlayerFloor(1), m_fOffSetGapY(0.f), m_bIsInvincible(false),
	m_IsMaster(true)
{
}

CPlayer::~CPlayer(void)
{
}

void CPlayer::Initialize(void)
{
	// ���� �߰�.
	{
		// if (���� ���� ���� Ŭ���̾�Ʈ���) {
	//		MakingPlayer���� g_vecplayer[g_myid]�� playerinfo�� �޾Ҵ�.
	//		g_vecplayer[g_myid]�κ��� CPlayer::m_playerinfo�� ���� �ѹ� �޾� �;� �Ѵ�.
	// }
	// else if (���� ������ �� ����, �ٸ� Ŭ���̾�Ʈ���) { 
	//		MakingPlayer���� g_vecplayer[�ٸ� Ŭ���̾�Ʈ id]�� playerinfo�� �޾Ҵ�.
	//		g_vecplayer[�ٸ� Ŭ���̾�Ʈ id]�κ��� CPlayer::m_playerinfo�� ���� �ѹ� �޾� �;� �Ѵ�.

	// --------------- Process -------------------
		if (!m_IsMaster) {
			if (g_myid == 0)
				g_vecplayer[g_myid + 1].job = m_playerinfo.job;
			else if (g_myid == 1)
				g_vecplayer[g_myid - 1].job = m_playerinfo.job;
		}
	}

	// �Ӱ� ���� �ʱ�ȭ
	InitializeCriticalSection(&cs2);

	//// �� �κ��� ���� �� �ص� �ɵ�..?
	//m_tInfo.pt.x = 100.f;
	//m_tInfo.pt.y = 500.f;

	m_tInfo.size.cx = 100.f;
	m_tInfo.size.cy = 100.f;

	// �������ͽ� �ʱ⼳��
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

	// ����
	m_eDir = DIR_RIGHT;

	// ���ǵ�
	m_fSpeed = 7.f;
	m_fSpeedY = 4.f;

	// ������ ����
	m_fAngle = 90.f;
	m_fJumpAcc = 0.f;
	m_fJumpSpeed = 15.f;

	m_bIsJump = false;
	m_bIsPressed = false;

	// ������
	m_fOffSet = WINCX / 2.f;
	m_fOffSetY = WINCY / 2.f;

	// ���� ����
	m_eCurState = PLAYER_STAND;
	m_ePreState = m_eCurState;

	// job�� ���� �̹��� ����.
	UpdateImageInJob(DIR_LEFT);

	m_dwDamageTime = GetTickCount();

	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iScene = 0;
	m_tFrame.dwFrameSpd = 50;

	m_eRenderType = RENDER_OBJ;

	// ������ ����Ʈ
	//m_tLevelUpFrame.iFrameStart = 0;
	//m_tLevelUpFrame.iFrameEnd = 20;
	//m_tLevelUpFrame.iScene = 0;
	//m_tLevelUpFrame.dwFrameSpd = 150;

	//m_dwLevelUpCurTime = 0;
	//m_dwLevelUpOldTime = GetTickCount();

	UpdateCollRect();

	// HP, Exp, MP Bar ����
	CObjMgr::GetInstance()->AddObject(CreateHpBar<CPlayerHP>(), OBJ_HPBAR);
	CObjMgr::GetInstance()->AddObject(CreateExpBar<CPlayerExp>(), OBJ_HPBAR);
	CObjMgr::GetInstance()->AddObject(CreateMpBar<CPlayerMP>(), OBJ_HPBAR);
}

int CPlayer::Update(void)
{
	// ������ ������Ʈ ����
	if (true == m_bIsDead)	return 1;

	// UpdateINFOinPLAYERINFO();
	 
	if (m_IsMaster) {
		KeyCheck();
		FrameMove();
		LevelUp();
		PreventOut();
		Scroll();
		LineCollision();
		InChangingScene();
	}
	else {

	}

	Jump();
	InInvincible();

	int id = WhatIsID();
	UpdateImageInJob(g_vecplayer[id].dir); // ���� Ű �Է� ���� ���Ӹ� �ƴ϶�, ����ؼ� �̹��� ������Ʈ ����� ��.

	// ���� ������ Ŭ���̾�Ʈ�� ���� ��� ������ �ɷ�.
	/// ����ؼ� �����ϱ� ���� ����� Ű �Էµ� �� �޾�����..
	//if (/*m_IsMaster*/1) {
	//	g_bIsSend = true;
	//}

	if(m_IsMaster)	// Ű �Է� �޾Ƽ� �����°� ���� ������ Ŭ���̾�Ʈ�� ���� ������ ����.
		SendMovePacket(); 	// move �� ������ �������� MOVE_PACKET�� ������.
	// m_tInfo�� ��� ����, �������� ��� ���ŵ� playerinfo�� �������� ������Ʈ �� ��� �Ѵ�.


	CObj::UpdateRect();
	UpdateCollRect();


	return 0;
}

void CPlayer::Render(HDC hDc)
{
#ifdef DEBUG
	cout << "m_tInfo : (" << m_tInfo.pt.x << ", " << m_tInfo.pt.y << ")" << endl;
#endif
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);
	if (NULL == pBit)  return;

	int id = WhatIsID();

	RECT rc{ g_vecplayer[id].pt.x - (g_vecplayer[id].size.cx * 0.5f),
		g_vecplayer[id].pt.y - (g_vecplayer[id].size.cy * 0.5f),
		g_vecplayer[id].pt.x + (g_vecplayer[id].size.cx * 0.5f),
		g_vecplayer[id].pt.y + (g_vecplayer[id].size.cy * 0.5f)
	};

	TransparentBlt(hDc,
		static_cast<int>(rc.left + g_fScrollX),
		static_cast<int>(rc.top + g_fScrollY),
		static_cast<int>(g_vecplayer[id].size.cx),
		static_cast<int>(g_vecplayer[id].size.cy),
		pBit->GetMemDC(),
		static_cast<int>(g_vecplayer[id].frame.iFrameStart * g_vecplayer[id].size.cx),
		static_cast<int>(g_vecplayer[id].frame.iScene * g_vecplayer[id].size.cy),
		static_cast<int>(g_vecplayer[id].size.cx),
		static_cast<int>(g_vecplayer[id].size.cy),
		RGB(0, 255, 0));


	//TransparentBlt(hDc,
	//	static_cast<int>(m_tRect.left + g_fScrollX),
	//	static_cast<int>(m_tRect.top + g_fScrollY),
	//	static_cast<int>(m_tInfo.size.cx),
	//	static_cast<int>(m_tInfo.size.cy),
	//	pBit->GetMemDC(),
	//	static_cast<int>(m_tFrame.iFrameStart * m_tInfo.size.cx),
	//	static_cast<int>(m_tFrame.iScene * m_tInfo.size.cy),
	//	static_cast<int>(m_tInfo.size.cx),
	//	static_cast<int>(m_tInfo.size.cy),
	//	RGB(0, 255, 0));

	// ��Ʈ�ڽ�
	if (GetAsyncKeyState('2'))
	{
		Rectangle(hDc,
			static_cast<int>(m_tCollRect.left + g_fScrollX),
			static_cast<int>(m_tCollRect.top + g_fScrollY),
			static_cast<int>(m_tCollRect.right + g_fScrollX),
			static_cast<int>(m_tCollRect.bottom + g_fScrollY));
	}

	// ������ ����Ʈ
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

		g_bIsSend = true;
	}
}

void CPlayer::Scroll()
{
	//if (!m_bIsAdmitSend_inFrame) {
	//	return;
	//}

	int id = WhatIsID();

#ifdef DEBUG_SCROLL
	cout << "g_fScrollX : " << g_fScrollX << endl;
	cout << "g_vecplayer[id].pt.x : " << g_vecplayer[id].pt.x << endl;
	cout << "m_tInfo.pt.x : " << m_tInfo.pt.x << endl;
	cout << "m_fOffSet : " << m_fOffSet << endl;
#endif 

	bool next = true;

	if (g_fScrollX > 0) {
		g_fScrollX = 0.f;
		next = false;

	}
	if (g_fScrollY > 0) {
		g_fScrollY = 0.f;
		next = false;
	}

	// �ʵ��� �� ��ũ�� ����
	if (SCENE_FIELD == g_eScene)
	{
		if (g_fScrollX < WINCX - FIELDCX) {
			g_fScrollX = WINCX - FIELDCX;
			next = false;
		}
		if (g_fScrollY < WINCY - FIELDCY) {
			g_fScrollY = WINCY - FIELDCY;
			next = false;
		}
	}

	// ��׽ý��϶� ��ũ�� ����
	if (SCENE_STAGE1 == g_eScene)
	{
		if (g_fScrollY < WINCY - HENESISCY) {
			g_fScrollY = WINCY - HENESISCY;
			next = false;
		}

		if (g_fScrollX < WINCX - HENESISCX) {
			g_fScrollX = WINCX - HENESISCX;
			next = false;
		}
	}

	//if (!next)
	//	return;

	switch (g_eScene)
	{
	case SCENE_FIELD:
		m_fOffSetGapY = 200.f;
		break;
	case SCENE_STAGE1:
		m_fOffSetGapY = 50.f;
		break;
	}
	// Player ��ǥ�� Offset���� 200��ŭ �̵��� ��� ��ũ���� �����Ѵ�.

	if (g_vecplayer[id].pt.x > m_fOffSet + 200.f)
	{
		g_fScrollX -= m_fSpeed /** 0.5f*/; // 1�ʿ� 60���� �ƴ϶�, 30���� ������ ������.. �̷��� �� �ʿ� ����.
		m_fOffSet += m_fSpeed;
	}
	else if (g_vecplayer[id].pt.x <= m_fOffSet - 200.f)
	{
		g_fScrollX += m_fSpeed/* * 0.5f*/;
		m_fOffSet -= m_fSpeed;
	}


	if (g_vecplayer[id].pt.y > m_fOffSetY + m_fOffSetGapY)
	{
		g_fScrollY -= m_fSpeedY;
		m_fOffSetY += m_fSpeedY;
	}
	else if (g_vecplayer[id].pt.y < m_fOffSetY - m_fOffSetGapY)
	{
		g_fScrollY += m_fSpeedY;
		m_fOffSetY -= m_fSpeedY;
	}


}

void CPlayer::KeyCheck()
{
	int id = WhatIsID();

	// �÷��̾� �⺻ ����
	if(CKeyMgr::GetInstance()->StayKeyDown(VK_LEFT))
	{
		m_tInfo.pt.x -= m_fSpeed;
		m_eDir = DIR_LEFT;
		if (PLAYER_DAMAGED != m_eCurState) {
			m_eCurState = PLAYER_WALK;
			g_vecplayer[id].state = PLAYER_WALK;
		}
		m_bIsRopeColl = false;
		UpdateImageInJob(DIR_LEFT);

		g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->StayKeyDown(VK_RIGHT))
	{
		m_tInfo.pt.x += m_fSpeed;
		m_eDir = DIR_RIGHT;
		if (PLAYER_DAMAGED != m_eCurState) {
			m_eCurState = PLAYER_WALK;
			g_vecplayer[id].state = PLAYER_WALK;
		}
		m_bIsRopeColl = false;
		UpdateImageInJob(DIR_RIGHT);

		g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->StayKeyDown(VK_DOWN) && !m_bIsRopeColl)
	{
		m_eCurState = PLAYER_CRAWL;
		g_vecplayer[id].state = PLAYER_CRAWL;

		if(CKeyMgr::GetInstance()->StayKeyDown(VK_CONTROL))
		{
			m_eCurState = PLAYER_CRAWLATTACK;
			g_vecplayer[id].state = PLAYER_CRAWLATTACK;
			m_tFrame.dwFrameSpd = 50;
			g_vecplayer[id].frame.dwFrameSpd = 50;
		}

		g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->StayKeyDown(VK_UP) && m_bIsRopeColl)
	{
		m_eDir = DIR_ROPE;
		m_eCurState = PLAYER_ROPE;
		g_vecplayer[id].state = PLAYER_ROPE;
		UpdateImageInJob(DIR_ROPE);
		m_tInfo.pt.y -= m_fSpeedY;

		if(CKeyMgr::GetInstance()->OnceKeyDown(VK_SPACE))
		{
			m_eCurState = PLAYER_JUMP;
			g_vecplayer[id].state = PLAYER_JUMP;
			UpdateImageInJob(DIR_LEFT);
			m_bIsRopeColl = false;
		}

		g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->StayKeyDown(VK_DOWN) && m_bIsRopeColl)
	{
		m_eCurState = PLAYER_ROPE;
		g_vecplayer[id].state = PLAYER_ROPE;
		UpdateImageInJob(DIR_ROPE);
		m_tInfo.pt.y += m_fSpeedY;

		g_bIsSend = true;
	}

	// ��ų
	else if(CKeyMgr::GetInstance()->OnceKeyUp('Q'))
	{
		//m_eCurState = PLAYER_SWING;
		//g_vecplayer[id].state = PLAYER_SWING;
		//CObjMgr::GetInstance()->AddObject(CreateSkill<CSwing>(), OBJ_SKILL_SWING);
		//CObjMgr::GetInstance()->AddObject(CreateEffect<CSwingEffect>(), OBJ_EFFECT);

		//g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->OnceKeyUp('W'))
	{
		//m_eCurState = PLAYER_SHOOT;
		//g_vecplayer[id].state = PLAYER_SHOOT;
		//CObjMgr::GetInstance()->AddObject(CreateSkill<CShoot>(), OBJ_SKILL_SHOOT);
		//CObjMgr::GetInstance()->AddObject(CreateArrow<CArrow>(0.f, 0.f, ARROW_BASIC, 0), OBJ_ARROW);
		//CSoundMgr::GetInstance()->PlaySound(L"Skill_Shoot.MP3", CSoundMgr::CHANNEL_SKILL);

		//g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->OnceKeyUp('E'))
	{
		//m_eCurState = PLAYER_SHOOT;
		//g_vecplayer[id].state = PLAYER_SHOOT;
		//CObjMgr::GetInstance()->AddObject(CreateDragon<CDragon>(), OBJ_SKILL_DRAGON);

		//CSoundMgr::GetInstance()->PlaySound(L"Skill_Dragon.MP3", CSoundMgr::CHANNEL_SKILL);
		//for(int i = 0; i < 5; ++i)
		//{
		//	float fX = 15.f;
		//	float fY = 10.f;
		//	float fY2 = 2.f;
		//	CObjMgr::GetInstance()->AddObject(CreateArrow<CArrow>(i * fX, (i - fY2) * fY, ARROW_DRAGON, i), OBJ_ARROW);
		//}

		//m_tState.iMp -= 100;

		//g_bIsSend = true;
	}
	else if(CKeyMgr::GetInstance()->OnceKeyUp('R'))
	{
		m_eCurState = PLAYER_SHOOT;
		g_vecplayer[id].state = PLAYER_SHOOT;
		CObjMgr::GetInstance()->AddObject(CreateFire<CFire>(), OBJ_SKILL_FIRE);
		CSoundMgr::GetInstance()->PlaySound(L"Skill_Fire.wav", CSoundMgr::CHANNEL_SKILL);
		m_tState.iMp -= 100;
		g_bIsSend = true;

		SendSkillCreatePacket(SKILL_FIRE);
	}
	else if(CKeyMgr::GetInstance()->OnceKeyUp('A'))
	{
		//m_eCurState = PLAYER_SHOOT;
		//g_vecplayer[id].state = PLAYER_SHOOT;
		//CObjMgr::GetInstance()->AddObject(CreateWing<CWing>(), OBJ_SKILL_WING);
		//CSoundMgr::GetInstance()->PlaySound(L"Skill_Wing.wav", CSoundMgr::CHANNEL_SKILL);
		//m_tState.iMp -= 500;

		//g_bIsSend = true;
	}
	else
	{
		if (!m_bIsRopeColl && PLAYER_SHOOT != m_eCurState && PLAYER_SWING != m_eCurState &&
			PLAYER_DAMAGED != m_eCurState && PLAYER_JUMP != m_eCurState) {
			m_eCurState = PLAYER_STAND;
			g_vecplayer[id].state = PLAYER_STAND;
		}
	}

	if(CKeyMgr::GetInstance()->OnceKeyUp(VK_SPACE) && !m_bIsPressed)
	{
		//m_eCurState = PLAYER_JUMP;
		//g_vecplayer[id].state = PLAYER_JUMP;
		//m_fAngle = 90.f;
		//if(PLAYER_DAMAGED != m_eCurState && PLAYER_JUMP != m_eCurState)
		//{
		//	m_eCurState = PLAYER_STAND;
		//	g_vecplayer[id].state = PLAYER_STAND;
		//}
		//m_bIsJump = true;
		//m_bIsPressed = true;
		//m_bIsRopeColl = false;
		//CSoundMgr::GetInstance()->PlaySound(L"Player_Jump.mp3", CSoundMgr::CHANNEL_PLAYER);
		////m_pImgName = L"Player_RIGHT";

		//g_bIsSend = true;
	}
	else
	{
		m_bIsPressed = false;
		m_tFrame.dwFrameSpd = 150;
		g_vecplayer[id].frame.dwFrameSpd = 150;
	}
}

void CPlayer::FrameMove()
{
	// ���� ���°� �ٲ��! ���¿� ���� �ִϸ��̼��� �ٲ���
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

		g_bIsSend = true;
	}

	m_dwFrameCurTime = GetTickCount();

	if (m_dwFrameOldTime + m_tFrame.dwFrameSpd < m_dwFrameCurTime)
	{
		++(m_tFrame.iFrameStart);
		m_dwFrameOldTime = m_dwFrameCurTime;

		g_bIsSend = true;
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
			// ��ų ���½ÿ� ��������Ʈ �ѹ� ������ IDLE ���·� �Ѱ�.
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
			// ��ų ���½ÿ� ��������Ʈ �ѹ� ������ IDLE ���·� �Ѱ�.
			if (PLAYER_SWING == m_eCurState || PLAYER_SHOOT == m_eCurState)
			{

				m_eCurState = PLAYER_STAND;
				m_tFrame.iFrameStart = 0;
				return;
			}
			m_tFrame.iFrameStart = 0;
		}
	}

	// ������ ����Ʈ�� �� ��.
	//// ������ ����Ʈ ������ ������
	//// ������ ���� �ƴϴ��� �������� ��� ���� �ִ�
	//m_dwLevelUpCurTime = GetTickCount();

	//if (m_dwLevelUpOldTime + m_tLevelUpFrame.dwFrameSpd > m_dwLevelUpCurTime)
	//{
	//	++m_tLevelUpFrame.iFrameStart;
	//	m_dwLevelUpOldTime = m_dwLevelUpCurTime;
	//}

	//// �������� ������ �� ������������ true�̸� start�� 100���� ���� (���� render���� ó�����ش�)
	//if (m_tLevelUpFrame.iFrameStart >= m_tLevelUpFrame.iFrameEnd && m_bIsLeveling)
	//{
	//	m_tLevelUpFrame.iFrameStart = 101;
	//	// 		g_iExp = 0;
	//	// 		m_tState.iMaxExp += 1000;
	//	// 		m_tState.iMaxHp += 3000;
	//	// 		m_tState.iHp = m_tState.iMaxHp;
	//	// 		g_iLevel++;
	//			//m_bIsLeveling = false;
	//}
	//// ������ ������ false�̸� start�� 100���� �����ع�����
	//if (m_tLevelUpFrame.iFrameStart >= m_tLevelUpFrame.iFrameEnd && !m_bIsLeveling)
	//{
	//	m_tLevelUpFrame.iFrameStart = 100;
	//}

	
}

void CPlayer::UpdateCollRect()
{
	// �߹� �������� ���� �ű� ������ CollRect.bottom�� ���� �����ϰ� top �����Ұ�
	float fCollRectCX = 45.f;
	float fCollRectCY = 70.f;

	m_tCollRect.left = static_cast<LONG>(m_tInfo.pt.x - (fCollRectCX / 2.f));
	m_tCollRect.right = static_cast<LONG>(m_tInfo.pt.x + (fCollRectCX / 2.f));
	m_tCollRect.bottom = static_cast<LONG>(m_tInfo.pt.y + (fCollRectCY / 2.f));
	m_tCollRect.top = static_cast<LONG>(m_tInfo.pt.y - (fCollRectCY / 2.f));
}

void CPlayer::Release(void)
{
	// �Ӱ� ���� ����
	DeleteCriticalSection(&cs2);
}

void CPlayer::LineCollision()
{
	int id = WhatIsID();
	float fY = 0;
	bool bLineCol = CLineMgr::GetInstance()->LineCollision(this, g_vecplayer[id].pt.x/*m_tInfo.pt.x*/, &fY);

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

bool b = true;	// ����� ��

void CPlayer::SendMovePacket()
{
	//m_bIsAdmitSend_inFrame = !m_bIsAdmitSend_inFrame;
	//if (!m_bIsAdmitSend_inFrame) {
	//	return;
	//}

	// 1201.
	// Server���� �� playerinfo�� send�Ѵ�. (CS_PACKET_PLAYERINFO_MOVE)
	if (g_bIsSend) {
		g_bIsSend = false;

		// ���� ���� ���� Ŭ���̾�Ʈ����, �ٸ� Ŭ���̾�Ʈ���� ����.
		int id = WhatIsID();

		b = !b;
//		cout << "Server���� �� playerinfo�� send" << b << endl;

		// 1. Ŭ���� g_vecplayer[g_myid]�� ������ �����Ѵ�. 
		// 2. ���� ���� playerinfo�� �����.
		// 3. playerinfo�� �� ��ġ, frame ����, state�� ��´�.
		// 4. playerinfo�� ������ send �Ѵ�.

		// ----------------------Progress-------------------------
		// 1. Ŭ���� g_vecplayer[g_myid]�� ������ �����Ѵ�. 
		{
			g_vecplayer[id].pt.x = m_tInfo.pt.x;
			g_vecplayer[id].pt.y = m_tInfo.pt.y;
			g_vecplayer[id].frame = m_tFrame;
			g_vecplayer[id].state = m_eCurState;
			g_vecplayer[id].prestate = m_ePreState;
			g_vecplayer[id].dir = m_eDir;
		}
		// 2. ���� ���� playerinfo�� �����.
		PLAYERINFO playerinfo;
		// 3. playerinfo�� �� ��ġ, frame ����, state�� ��´�.
		{
			memcpy(&playerinfo, &(g_vecplayer[id]), sizeof(PLAYERINFO));
		}
		// 4. playerinfo�� ������ send �Ѵ�.
		{
			char buf[BUFSIZE] = {};
			// ���� ����
			PACKETINFO packetinfo;
			packetinfo.id = id;
			packetinfo.size = sizeof(PLAYERINFO);
			packetinfo.type = CS_PACKET_PLAYERINFO_MOVE;
			memcpy(buf, &packetinfo, sizeof(packetinfo));
			g_retval = send(g_sock, buf, BUFSIZE, 0);
			if (g_retval == SOCKET_ERROR) {
				MessageBox(g_hWnd, L"send()", L"send - ���� - CS_PACKET_PLAYERINFO_MOVE", MB_OK);
				g_bIsProgramEnd = true;	// ���α׷� ����
			}

			// ���� ����
			ZeroMemory(buf, sizeof(buf));
			memcpy(buf, &playerinfo, sizeof(playerinfo));
			g_retval = send(g_sock, buf, BUFSIZE, 0);
			if (g_retval == SOCKET_ERROR) {
				MessageBox(g_hWnd, L"send()", L"send - ���� - CS_PACKET_PLAYERINFO_MOVE", MB_OK);
				g_bIsProgramEnd = true;	// ���α׷� ����
			}
		}
	}
}

void CPlayer::SendSceneChangePacket()
{
	// ���� �ٲ���� ��,
	// Server���� �� playerinfo�� send�Ѵ�. (CS_PACKET_PLAYERINFO_MOVE)


	// ���� ���� ���� Ŭ���̾�Ʈ����, �ٸ� Ŭ���̾�Ʈ���� ����.
	int id = WhatIsID();


	// 1. Ŭ���� g_vecplayer[g_myid]�� ������ �����Ѵ�. 
	// 2. ���� ���� playerinfo�� �����.
	// 3. playerinfo�� �� ��ġ, frame ����, state�� ��´�.
	// 4. playerinfo�� ������ send �Ѵ�.

	// ----------------------Progress-------------------------
	// 1. Ŭ���� g_vecplayer[g_myid]�� ������ �����Ѵ�. 
	{
		g_vecplayer[id].pt.x = m_tInfo.pt.x;
		g_vecplayer[id].pt.y = m_tInfo.pt.y;
		g_vecplayer[id].frame = m_tInfo.frame;
		g_vecplayer[id].state = m_eCurState;
	}
	// 2. ���� ���� playerinfo�� �����.
	PLAYERINFO playerinfo;
	// 3. playerinfo�� �� ��ġ, frame ����, state�� ��´�.
	{
		memcpy(&playerinfo, &(g_vecplayer[id]), sizeof(PLAYERINFO));
	}
	// 4. playerinfo�� ������ send �Ѵ�.
	{
		char buf[BUFSIZE] = {};
		// ���� ����
		PACKETINFO packetinfo;
		packetinfo.id = id;
		packetinfo.size = sizeof(PLAYERINFO);
		packetinfo.type = CS_PACKET_PLAYERINFO_INCHANGINGSCENE;
		memcpy(buf, &packetinfo, sizeof(packetinfo));
		g_retval = send(g_sock, buf, BUFSIZE, 0);
		if (g_retval == SOCKET_ERROR) {
			MessageBox(g_hWnd, L"send()", L"send - ���� - CS_PACKET_PLAYERINFO_INCHANGINGSCENE", MB_OK);
			g_bIsProgramEnd = true;	// ���α׷� ����
		}

		// ���� ����
		ZeroMemory(buf, sizeof(buf));
		memcpy(buf, &playerinfo, sizeof(playerinfo));
		g_retval = send(g_sock, buf, BUFSIZE, 0);
		if (g_retval == SOCKET_ERROR) {
			MessageBox(g_hWnd, L"send()", L"send - ���� - CS_PACKET_PLAYERINFO_INCHANGINGSCENE", MB_OK);
			g_bIsProgramEnd = true;	// ���α׷� ����
		}
	}
	
}

void CPlayer::SendSkillCreatePacket(SKILL_TYPE eType)
{
	// ��ų Ű�� �Է� ���� ������ ȣ��ȴ�.
	// CS_PACKET_SKILL_CREATE 
	
	// 0. �ʿ��� ������.
	char buf[BUFSIZE];
	int id = WhatIsID();
	// 1. �������� ���� ���� ��Ŷ�� ������.
	{
		PACKETINFO packetinfo = {};
		packetinfo.id = WhatIsID();
		packetinfo.size = sizeof(SKILLINFO);
		packetinfo.type = CS_PACKET_SKILL_CREATE;
		
		memcpy(buf, &packetinfo, sizeof(packetinfo));
		g_retval == send(g_sock, buf, BUFSIZE, 0);

		if (g_retval == SOCKET_ERROR) {
			MessageBox(g_hWnd, L"send()", L"send() - ���� - CS_PACKET_SKILL_CREATE", NULL);
			return;
		}
	}
	// 2. �������� ���� ���� ��Ŷ�� ������.
	{
		SKILLINFO skillinfo = {};
		skillinfo.type = eType;
		if(m_eDir == DIR_LEFT)
			skillinfo.pt = MYPOINT{ g_vecplayer[id].pt.x - FIREX, g_vecplayer[id].pt.y - FIREY };
		else
			skillinfo.pt = MYPOINT{ g_vecplayer[id].pt.x + FIREX, g_vecplayer[id].pt.y - FIREY };
		skillinfo.id = -1;

		ZeroMemory(buf, sizeof(buf));
		memcpy(buf, &skillinfo, sizeof(skillinfo));
		g_retval == send(g_sock, buf, BUFSIZE, 0);

		if (g_retval == SOCKET_ERROR) {
			MessageBox(g_hWnd, L"send()", L"send() - ���� - CS_PACKET_SKILL_CREATE", NULL);
			return;
		}
	}
	
}

void CPlayer::UpdateINFOinPLAYERINFO()
{
	// �̰� �ƴ� �� ����;;
////	m_tInfo.frame = m_playerinfo.frame; �ִϸ��̼� ����ȭ �����ϰ��� �ϴ°ɷ�             
//	m_tInfo.hp = m_playerinfo.hp;
//	m_tInfo.pt = m_playerinfo.pt;
//	m_tInfo.size = m_playerinfo.size;
//	m_eCurState = m_playerinfo.state;

	int id = WhatIsID();

	m_tInfo.frame = g_vecplayer[id].frame;
	m_tInfo.hp = g_vecplayer[id].hp;
	m_tInfo.pt = g_vecplayer[id].pt;
	m_tInfo.size = g_vecplayer[id].size;
	m_eCurState = g_vecplayer[id].state;

}

void CPlayer::UpdateImageInJob(OBJECT_DIR dir)
{
	if (dir == DIR_LEFT) {
		if (!m_IsMaster) {	// ���� �Ұ�����, �ٸ� Ŭ���̾�Ʈ���
			if (g_myid == 0) { // id�� �޾ƿ���
				if (g_vecplayer[g_myid + 1].job == JOB_STRIKER)
					m_pImgName = L"Player_LEFT";
				else
					m_pImgName = L"Captin_LEFT";
			}
			else if (g_myid == 1) {
				if (g_vecplayer[g_myid - 1].job == JOB_STRIKER)
					m_pImgName = L"Player_LEFT";
				else
					m_pImgName = L"Captin_LEFT";

			}
		}
		else { // ���� ������ ���� Ŭ���̾�Ʈ���
			if (g_vecplayer[g_myid].job == JOB_STRIKER)
				m_pImgName = L"Player_LEFT";
			else
				m_pImgName = L"Captin_LEFT";
		}
	}
	else if (dir == DIR_RIGHT) {
		if (!m_IsMaster) { // ���� �Ұ�����, �ٸ� Ŭ���̾�Ʈ���
			if (g_myid == 0) { // id�� �޾ƿ���
				if (g_vecplayer[g_myid + 1].job == JOB_STRIKER)
					m_pImgName = L"Player_RIGHT";
				else
					m_pImgName = L"Captin_RIGHT";
			}
			else if (g_myid == 1) {
				if (g_vecplayer[g_myid - 1].job == JOB_STRIKER)
					m_pImgName = L"Player_RIGHT";
				else
					m_pImgName = L"Captin_RIGHT";

			}
		}
		else { // ���� ������ ���� Ŭ���̾�Ʈ���
			if (g_vecplayer[g_myid].job == JOB_STRIKER)
				m_pImgName = L"Player_RIGHT";
			else
				m_pImgName = L"Captin_RIGHT";
		}
	}
	else if (dir == DIR_ROPE) {
		if (!m_IsMaster) { // ���� �Ұ�����, �ٸ� Ŭ���̾�Ʈ���
			if (g_myid == 0) { // id�� �޾ƿ���
				if (g_vecplayer[g_myid + 1].job == JOB_STRIKER)
					m_pImgName = L"Player_ROPE";
				else
					m_pImgName = L"Captin_ROPE";
			}
			else if (g_myid == 1) {
				if (g_vecplayer[g_myid - 1].job == JOB_STRIKER)
					m_pImgName = L"Player_ROPE";
				else
					m_pImgName = L"Captin_ROPE";

			}
		}
		else { // ���� ������ ���� Ŭ���̾�Ʈ���
			if (g_vecplayer[g_myid].job == JOB_STRIKER)
				m_pImgName = L"Player_ROPE";
			else
				m_pImgName = L"Captin_ROPE";
		}
	}
}

int CPlayer::WhatIsID()
{
	// ���� ���� ���� Ŭ���̾�Ʈ����, �ٸ� Ŭ���̾�Ʈ���� ����.
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

	return id;
}

int CPlayer::WhatIsOtherID()
{
	// ���� ���� ���� Ŭ���̾�Ʈ����, �ٸ� Ŭ���̾�Ʈ���� ����.
	int id{ 0 };
	if (!m_IsMaster) {
		id = g_myid;
	}
	else {
		if (g_myid == 0)
			id = 1;
		else
			id = 0;
	}

	return id;
}

void CPlayer::PreventOut()
{
	// ȭ�� ������ �������Բ�
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
}

void CPlayer::InChangingScene()
{
	// �� �ٲ� ��
	if (true == g_bIsSceneChange)
	{
		int id = WhatIsID();
		int otherid = WhatIsOtherID();
		// �÷��̾� ��ǥ ����
		g_vecplayer[id].pt.x = 100.f;
		g_vecplayer[otherid].pt.x = 100.f;
		m_tInfo.pt.x = 100.f;
		// ������ �� ����ġ
		m_fOffSet = WINCX / 2.f;

		if (g_eScene == SCENE_STAGE1)
		{
			g_fScrollX = 0.f;
			g_fScrollY = (WINCY - HENESISCY);
			m_fOffSetY = WINCY / 2.f + 100.f;
		}

		g_bIsSceneChange = false;
		// g_bIsSend = true;
		// SendMovePacket();
		SendSceneChangePacket();
	}
}

void CPlayer::InInvincible()
{
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
}

void CPlayer::FrameMove_otherclient()
{
	int id = WhatIsID();
	// ���� �Ұ����� Ŭ���̾�Ʈ�� ��.
	if (g_vecplayer[id].state != g_vecplayer[id].prestate)
	{
		switch (g_vecplayer[id].state)
		{
		case PLAYER_STAND:
			g_vecplayer[id].frame.iFrameEnd = 4;
			g_vecplayer[id].frame.iScene = 0;
			break;
		case PLAYER_JUMP:
			g_vecplayer[id].frame.iFrameEnd = 0;
			g_vecplayer[id].frame.iScene = 6;
			break;
		case PLAYER_WALK:
			g_vecplayer[id].frame.iFrameEnd = 2;
			g_vecplayer[id].frame.iScene = 1;
			break;
		case PLAYER_ATTACK:
			g_vecplayer[id].frame.iFrameEnd = 2;
			g_vecplayer[id].frame.iScene = 2;
			break;
		case PLAYER_SWING:
			g_vecplayer[id].frame.iFrameEnd = 3;
			g_vecplayer[id].frame.iScene = 3;
			break;
		case PLAYER_CRAWLATTACK:
			g_vecplayer[id].frame.iFrameEnd = 1;
			g_vecplayer[id].frame.iScene = 4;
			break;
		case PLAYER_CRAWL:
			g_vecplayer[id].frame.iFrameEnd = 0;
			g_vecplayer[id].frame.iScene = 5;
			break;
		case PLAYER_SHOOT:
			g_vecplayer[id].frame.iFrameEnd = 3;
			g_vecplayer[id].frame.iScene = 6;
			break;
		case PLAYER_DAMAGED:
			g_vecplayer[id].frame.iFrameEnd = 2;
			g_vecplayer[id].frame.iScene = 7;
			break;
		case PLAYER_ROPE:
			g_vecplayer[id].frame.iFrameEnd = 1;
			g_vecplayer[id].frame.iScene = 0;
			break;
		}

		g_vecplayer[id].prestate = g_vecplayer[id].state;


		if (g_vecplayer[id].state != PLAYER_ROPE)
			m_bIsRopeColl = false;
	}

	m_dwFrameCurTime = GetTickCount();

	if (m_dwFrameOldTime + g_vecplayer[id].frame.dwFrameSpd < m_dwFrameCurTime)
	{
		++(g_vecplayer[id].frame.iFrameStart);
		m_dwFrameOldTime = m_dwFrameCurTime;
	}

	if (g_vecplayer[id].state == PLAYER_DAMAGED)
	{
		if (g_vecplayer[id].frame.iFrameStart == g_vecplayer[id].frame.iFrameEnd)
			g_vecplayer[id].state = PLAYER_STAND;
	}

	if (g_vecplayer[id].state != PLAYER_ROPE)
	{
		if (g_vecplayer[id].frame.iFrameStart >= g_vecplayer[id].frame.iFrameEnd)
		{
			// ��ų ���½ÿ� ��������Ʈ �ѹ� ������ IDLE ���·� �Ѱ�.
			if (PLAYER_SWING == g_vecplayer[id].state || PLAYER_SHOOT == g_vecplayer[id].state)
			{

				g_vecplayer[id].state = PLAYER_STAND;
				g_vecplayer[id].frame.iFrameStart = 0;
				return;
			}
			g_vecplayer[id].frame.iFrameStart = 0;
		}
	}
	else
	{
		if (g_vecplayer[id].frame.iFrameStart > g_vecplayer[id].frame.iFrameEnd)
		{
			// ��ų ���½ÿ� ��������Ʈ �ѹ� ������ IDLE ���·� �Ѱ�.
			if (PLAYER_SWING == g_vecplayer[id].state || PLAYER_SHOOT == g_vecplayer[id].state)
			{

				g_vecplayer[id].state = PLAYER_STAND;
				g_vecplayer[id].frame.iFrameStart = 0;
				return;
			}
			g_vecplayer[id].frame.iFrameStart = 0;
		}
	}
}

void CPlayer::LevelUp()
{	// ������ 
		if (g_iExp >= m_tState.iMaxExp)
		{
			m_bIsLeveling = true; // ������ ����
			// ������ ����Ʈ ���鶧 ���� ����..
			g_iLevel++;
			g_iExp = 0;
			m_dwLevelUpOldTime = GetTickCount();
		}

}
