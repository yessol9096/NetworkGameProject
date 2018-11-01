#pragma once
#include "obj.h"

class CItem :
	public CObj
{
public:
	CItem(void);
	CItem(char* szName, int iHp, int iMp, int iAtt, int iMaxHp, int iExp, int iLev, int iGold);
	virtual ~CItem(void);


public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();

	virtual void Move() = 0;
	virtual void UpdateCollRect() = 0;

	void SetOriginalY(float fOriginalY) { m_fOriginalY = fOriginalY; }
	void SetIsInInven(bool bIsInInven) { m_bIsInInven = bIsInInven; }

protected:
	float		m_fOriginalY;

	float		m_fJumpAcc;
	float		m_fJumpSpeed;

	bool		m_bIsJump;
	bool		m_bIsJumpUp;

	bool		m_bIsInInven;

	float		m_fXAngle;
};
