#pragma once
#include "obj.h"
#include "Monster.h"
#include "BossHPBar.h"

class CBoss :
	public CMonster
{
public:
	CBoss(void);
	virtual ~CBoss(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	FrameMove();
	virtual void	Release(void);

	virtual void	MoveInPattern();
	virtual void	UpdateCollRect();
	virtual void	KnockBack();

	const RECT&	GetSkillRect() { return m_tSkillRect; }

private:
	template <typename T>
	CObj*	CreateHpBar()
	{
		CObj* pHpBar = CAbstractFactory<T>::CreateObj();
		dynamic_cast<BossHPBar*>(pHpBar)->SetBoss(this);
		return pHpBar;
	}
private:
	template <typename T>
	CObj*	CreateEffect()
	{
		if(DIR_LEFT == m_eDir)
		{
			CObj* pEffect = CAbstractFactory<T>::CreateObj(
				m_tInfo.fX - SWINGEFFECTX, m_tInfo.fY);
			pEffect->SetAtt(this->GetState().iAtt);
			pEffect->SetDir(m_eDir);
			return pEffect;
		}
		else
		{
			CObj* pEffect = CAbstractFactory<T>::CreateObj(
				m_tInfo.fX + SWINGEFFECTX, m_tInfo.fY);
			pEffect->SetAtt(this->GetState().iAtt);
			pEffect->SetDir(m_eDir);
			return pEffect;
		}

	}

private:
	DWORD m_dwDamageCurTime;
	DWORD m_dwDamageOldTime;
	DWORD m_dwDamageTime;

private:
	enum BOSS_PATTERN { BOSS_IDLE, BOSS_COMBAT1, BOSS_COMBAT2, BOSS_COMBAT3, BOSS_COMBAT4 };

	BOSS_PATTERN	m_ePattern;

	RECT			m_tSkillRect;
// 	BOSS_STATE m_eBossCurState;
// 	BOSS_STATE m_eBossPreState;
};
