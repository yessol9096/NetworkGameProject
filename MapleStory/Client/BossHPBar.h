#pragma once
#include "obj.h"

class BossHPBar :
	public CObj
{
public:
	BossHPBar(void);
	virtual ~BossHPBar(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	FrameMove();
	virtual void	Release(void);


public:
	void SetBoss(CObj* pBoss) { m_pBoss = pBoss; }

private:
	RECT	m_tBarRect;
	INFO	m_tBarInfo;

	CObj*	m_pBoss;
	int		m_iBossHp;
	int		m_iBossHpMax;
};
