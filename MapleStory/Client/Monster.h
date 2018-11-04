#pragma once
#include "obj.h"
#include "Gold.h"

class CMonster :
	public CObj
{
public:
	CMonster(void);
	virtual ~CMonster(void);

public:
	void SetPattern(int iPattern) { m_iPattern = iPattern; }

	void SetIsFloorBoxColl(bool bFloorBoxColl) { m_bIsFloorBoxColl = bFloorBoxColl; }
	bool GetIsFloorBoxColl() { return m_bIsFloorBoxColl; }

	virtual void	MoveInPattern() = 0;
	virtual void	UpdateCollRect() = 0;
	virtual void	KnockBack() = 0;

	void SetKnockBack(float fKnockBack) { m_fKnockBack += fKnockBack; }

	float GetKnockBackMax() { return m_fKnockBackMax; }

public:
	MONSTER_STATE GetMonsterState() { return m_eCurState; }
	void		  SetMonsterState(MONSTER_STATE eState) { m_eCurState = eState; }

public:
	template <typename T>
	CObj*	CreateGold()
	{
		float fHeight =  static_cast<float>(m_tCollRect.bottom - 17.5 * 0.5f);
		float fWidth = 15.f;
		CObj* pGold = CAbstractFactory<T>::CreateObj(
			m_tInfo.pt.x - fWidth, fHeight);
		dynamic_cast<CGold*>(pGold)->SetGold(m_tState.iGold);
		dynamic_cast<CGold*>(pGold)->SetOriginalY(m_tInfo.pt.y);
		return pGold;
	}

public:
	template <typename T>
	CObj*	CreateGoldBoss(float fSpeed, float fAngle, bool bIsGoldBoss, bool bIsXReverse)
	{
		float fHeight =  static_cast<float>(m_tCollRect.bottom - 17.5 * 0.5f);
		float fWidth = 150.f;
		CObj* pGold = CAbstractFactory<T>::CreateObj(
			m_tCollRect.left - fWidth, fHeight);
		dynamic_cast<CGold*>(pGold)->SetGold(m_tState.iGold);
		dynamic_cast<CGold*>(pGold)->SetOriginalY(m_tInfo.pt.y);
		dynamic_cast<CGold*>(pGold)->SetBossGold(true);
		dynamic_cast<CGold*>(pGold)->SetIsXReverse(bIsXReverse);
		pGold->SetSpeed(fSpeed);
		pGold->SetAngle(fAngle);
		return pGold;
	}

public:
	template <typename T>
	CObj*	CreateItem()
	{
		float fHeight =  static_cast<float>(m_tCollRect.bottom - 17.5 * 0.5f);
		float fWidth = 15.f;
		CObj* pItem = CAbstractFactory<T>::CreateObj(
			m_tInfo.pt.x + fWidth, fHeight);
		dynamic_cast<CItem*>(pItem)->SetOriginalY(m_tInfo.pt.y);
		return pItem;
	}

protected:
	MONSTER_STATE   m_eCurState;
	MONSTER_STATE	m_ePreState;

	bool		m_bCombatible;
	int			m_iPattern;

	DWORD		m_dwCreateCurTime;
	DWORD		m_dwCreateOldTime;

	bool		m_bIsFloorBoxColl;

	float		m_fKnockBack;
	float		m_fKnockBackMax;

};
