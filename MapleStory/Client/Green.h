#pragma once
#include "Obj.h"
#include "Monster.h"

class CGreen :
	public CMonster
{
public:
	CGreen(void);
	virtual ~CGreen(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();
	virtual void	MoveInPattern();
	virtual void	UpdateCollRect();

	virtual void	KnockBack();
	virtual void	SendMovePacket();
	void	LineCollision();

private:
	DWORD m_dwDamageCurTime;
	DWORD m_dwDamageOldTime;
	DWORD m_dwDamageTime;

	DWORD m_dwDeadCurTime;
	DWORD m_dwDeadOldTime;
	DWORD m_dwDeadTime;
	



};
