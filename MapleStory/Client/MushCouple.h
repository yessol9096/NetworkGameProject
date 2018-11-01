#pragma once
#include "Obj.h"
#include "Monster.h"

class CMushCouple :
	public CMonster
{
public:
	CMushCouple(void);
	virtual ~CMushCouple(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	FrameMove();
	virtual void	Release(void);
	virtual void	MoveInPattern();
	virtual void	UpdateCollRect();
	virtual void	KnockBack();


private:
	DWORD m_dwDamageCurTime;
	DWORD m_dwDamageOldTime;
	DWORD m_dwDamageTime;
};
