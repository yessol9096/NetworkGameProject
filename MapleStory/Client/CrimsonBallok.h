#pragma once
#include "Obj.h"
#include "Monster.h"

class CCrimsonBallok :
	public CMonster
{
public:
	CCrimsonBallok(void);
	virtual ~CCrimsonBallok(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	FrameMove();
	virtual void	Release(void);

	virtual void	MoveInPattern();
	virtual void	UpdateCollRect();
	virtual void	KnockBack();
};
