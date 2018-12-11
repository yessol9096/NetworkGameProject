#pragma once
#include "obj.h"
#include "PlayerHP.h"
#include "PlayerExp.h"
#include "PlayerMP.h"
#include "Arrow.h"

class CPlayer :
	public CObj
{
public:
	CPlayer(void);
	virtual ~CPlayer(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();

public:
	void	Jump();
	void	Scroll();
	void	KeyCheck();
	void	LineCollision();
	
	void	SendMovePacket();
	void SendSceneChangePacket();
	void SendSkillCreatePacket(SKILL_TYPE eType);

	void UpdateINFOinPLAYERINFO();
	void UpdateImageInJob(OBJECT_DIR dir);
	int WhatIsID();
	int WhatIsOtherID();
	void LevelUp();
	void PreventOut();
	void InChangingScene();
	void InInvincible();

	void FrameMove_otherclient();


	// ---------------------
	// ���� �߰�
	// static DWORD WINAPI SendThread(LPVOID, int, bool&, INFO&, PLAYER_STATE&)

	// �����¿��
public:
	void    SetIsRopeColl(bool bRopeColl) { m_bIsRopeColl = bRopeColl; }

	// �� �¿��
	void	SetPlayerFloor(int iPlayerFloor) { m_iPlayerFloor = iPlayerFloor; }
	int		GetPlayerFloor() { return m_iPlayerFloor; }
	bool	GetIsJumpUp() { return m_bIsJumpUp; }

	// ���Ͷ� �浹�� �� �ڷ� ������ �׸��鼭 �и���
	void	SetIsJump(bool bIsJump) { m_bIsJump = bIsJump; }
	void	SetMinusAngle(float fAngle) { m_fAngle -= fAngle; }
	void    SetPlusAngle(float fAngle) { m_fAngle += fAngle; }

	void	SetIsLineColl(bool bLineColl) { m_bLineColl = bLineColl;}
	float*  GetLinepY() { return &m_fLineY; }

public:
	// set, get (�ַ� ����)
	PLAYER_STATE GetPlayerState() { return m_eCurState; }
	void		 SetPlayerState(PLAYER_STATE eState) { m_eCurState = eState; }
	void		 SetIsLeveling(bool bLeveling) { m_bIsLeveling = bLeveling; }

	void		SetDamaged(int iDamage) { m_tState.iHp -= iDamage; }
	void		SetGold(int iGold) {
		m_tState.iGold += iGold;

		g_bIsSend = true;
		g_vecplayer[g_myid].money += iGold;
	}

	void		SetHp(int iHp) 
	{
		// �ƽ�HP - HPPORTION ���� ���� ��������
		if(m_tState.iHp > m_tState.iMaxHp - HPPORTION)	
			m_tState.iHp += (m_tState.iMaxHp - m_tState.iHp); 
		else
			m_tState.iHp += HPPORTION;
	}
	void		SetMp(int iMp) 
	{
		// �ƽ�HP - HPPORTION ���� ���� ��������
		if(m_tState.iMp > m_tState.iMaxMp - MPPORTION)	
			m_tState.iMp += (m_tState.iMaxMp - m_tState.iMp); 
		else
			m_tState.iMp += MPPORTION;
	}

	// (����) ���� �Ұ�����, �ٸ� Ŭ���̾�Ʈ�� ������ �� �����.
	void		SetPlayerInfo(PLAYERINFO playerinfo) {
		m_playerinfo = playerinfo; 
		m_IsMaster = false;
	}
	void SetInfoPt(MYPOINT pt) { m_tInfo.pt = pt; }

	// ���� ����
	bool		 GetIsInvincible() { return m_bIsInvincible; }
	void		 SetIsInvincible(bool bIsInvincible) { m_bIsInvincible = bIsInvincible; }
	
public:
	// �浹�ڽ� ������Ʈ
	void	UpdateCollRect();

	// ȭ��
private:
	template <typename T> 
	CObj*	CreateArrow(float fX = 0.f, float fY = 0.f, ARROW_ID eID = ARROW_BASIC, int iOrder = 0)
	{
		int id = WhatIsID();
		CObj* pArrow = CAbstractFactory<T>::CreateObj(
			g_vecplayer[id].pt.x/*m_tInfo.pt.x*/ + fX, g_vecplayer[id].pt.y/*m_tInfo.pt.y*/ + fY);
		pArrow->SetAtt(this->GetState().iAtt);
		pArrow->SetDir(m_eDir);
		dynamic_cast<CArrow*>(pArrow)->SetID(eID);
		dynamic_cast<CArrow*>(pArrow)->SetOrder(iOrder);
		return pArrow;
	}

	// ��, ���� (�÷��̾�� ������ ��ų)
private:
	template <typename T>
	CObj*	CreateSkill()
	{
		CObj* pSkill = CAbstractFactory<T>::CreateObj(
			m_tInfo.pt.x, m_tInfo.pt.y);
		pSkill->SetDir(m_eDir);
		pSkill->SetAtt(this->GetState().iAtt);
		dynamic_cast<CSkill*>(pSkill)->SetPlayer(this);
		return pSkill;
	}

	// �巡��
private:
	template <typename T>
	CObj*	CreateDragon()
	{
		if(DIR_LEFT == m_eDir)
		{
			CObj* pSkill = CAbstractFactory<T>::CreateObj(
				m_tInfo.pt.x - DRAGONX, m_tInfo.pt.y);
			pSkill->SetAtt(this->GetState().iAtt);
			pSkill->SetDir(m_eDir);
			return pSkill;
		}
		else
		{
			CObj* pSkill = CAbstractFactory<T>::CreateObj(
				m_tInfo.pt.x + DRAGONX, m_tInfo.pt.y);
			pSkill->SetAtt(this->GetState().iAtt);
			pSkill->SetDir(m_eDir);
			return pSkill;
		}
	}

	// ���̾� ��ų
private:
	template <typename T>
	CObj*	CreateFire()
	{
		if(DIR_LEFT == m_eDir)
		{
			CObj* pSkill = CAbstractFactory<T>::CreateObj(
				m_tInfo.pt.x - FIREX, m_tInfo.pt.y - FIREY);
//			pSkill->SetPos(m_tInfo.pt.x - FIREX, m_tInfo.pt.y - 350.f);
			pSkill->SetAtt(this->GetState().iAtt);
			pSkill->SetDir(m_eDir);
			return pSkill;
		}
		else
		{
			CObj* pSkill = CAbstractFactory<T>::CreateObj(
				m_tInfo.pt.x + FIREX, m_tInfo.pt.y- FIREY);
//			pSkill->SetPos(m_tInfo.pt.x + FIREX, m_tInfo.pt.y - 350.f);
			pSkill->SetAtt(this->GetState().iAtt);
			pSkill->SetDir(m_eDir);
			return pSkill;
		}
	}

	// ���� ����Ʈ
private:
	template <typename T>
	CObj*	CreateEffect()
	{
		if(DIR_LEFT == m_eDir)
		{
			CObj* pEffect = CAbstractFactory<T>::CreateObj(
				m_tInfo.pt.x - SWINGEFFECTX, m_tInfo.pt.y);
			pEffect->SetAtt(this->GetState().iAtt);
			pEffect->SetDir(m_eDir);
			return pEffect;
		}
		else
		{
			CObj* pEffect = CAbstractFactory<T>::CreateObj(
				m_tInfo.pt.x + SWINGEFFECTX, m_tInfo.pt.y);
			pEffect->SetAtt(this->GetState().iAtt);
			pEffect->SetDir(m_eDir);
			return pEffect;
		}
	}

	// ȭ�� �߾ӿ� ��Ÿ���� ��ų
	template <typename T>
	CObj*	CreateWing()
	{
		CObj* pSkill = CAbstractFactory<T>::CreateObj(
			WINCX * 0.5f, WINCY * 0.5f);
		pSkill->SetDir(m_eDir);
		pSkill->SetAtt(this->GetState().iAtt);
		//			dynamic_cast<CSkill*>(pSkill)->SetPlayer(this);
		return pSkill;
	}


	// HP��
private:
	template <typename T>
	CObj*	CreateHpBar()
	{
		CObj* pHpBar = CAbstractFactory<T>::CreateObj();
		dynamic_cast<CPlayerHP*>(pHpBar)->SetPlayer(this);
		return pHpBar;
	}

	// EXP
private:
	template <typename T>
	CObj*	CreateExpBar()
	{
		CObj* pExpBar = CAbstractFactory<T>::CreateObj();
		dynamic_cast<CPlayerExp*>(pExpBar)->SetPlayer(this);
		return pExpBar;
	}

	// MP
private:
		template <typename T>
		CObj*	CreateMpBar()
		{
			CObj* pMpBar = CAbstractFactory<T>::CreateObj();
			dynamic_cast<CPlayerMP*>(pMpBar)->SetPlayer(this);
			return pMpBar;
		}

private:
	float			m_fJumpSpeed; // ���� �Ŀ�
	float			m_fJumpAcc;   // ���� ���ӵ� (�ð�)

	float			m_fSpeedY;

	bool			m_bIsJump;	  // ���� �������� �Ǻ�.
	bool			m_bIsPressed; // ���� Ű�� ���ȴ��� �Ǻ�

	float			m_fOffSet;	  // ��ũ�� ���� ����
	float			m_fOffSetY;		// ��ũ�� Y
	float			m_fOffSetGapY; // ������ ������ �޸� �ֱ�

	PLAYER_STATE	m_eCurState;  // �÷��̾� ���� ����
	PLAYER_STATE	m_ePreState;  // �÷��̾� ���� ����

	bool			m_bIsLeveling;

	// �̹���
	HDC				m_hDC;
	HDC				m_hMemDC;

	HBITMAP			m_hOldBmp;
	HBITMAP			m_hBitmap;

	int				m_iBlending;

	DWORD			m_dwBlendOldTime;
	DWORD			m_dwBlendCurTime;

	// �浹
	bool			m_bIsRopeColl;
	int				m_iPlayerFloor;

	bool			m_bIsJumpUp;
	bool			m_bLineColl;

	DWORD			m_dwDamageTime;

	bool			m_bIsInvincible;

	// ����
	list<CLine*>	m_LineList;
	float			m_fLineY;

	// ��ų
	bool			m_bIsSkillPressed;

	// ������ ����Ʈ
	FRAME			m_tLevelUpFrame;
	DWORD			m_dwLevelUpCurTime;
	DWORD			m_dwLevelUpOldTime;

	// -------------------------------
	// ���� �߰�.
	PLAYERINFO	m_playerinfo;
	bool					m_IsMaster = true; // �� Ŭ���̾�Ʈ�� �÷��̾�����, �ٸ� Ŭ���̾�Ʈ�� �÷��̾����� ����.
	bool					m_bIsAdmitSend_inFrame = false; // 1�ʴ� 30���� �����ϱ� ���ؼ� ����.
};
