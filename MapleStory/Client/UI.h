#pragma once
#include "obj.h"

class CUI :
	public CObj
{
public:
	CUI(void);
	virtual ~CUI(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	FrameMove();
	virtual void	Release(void);

private:
	RECT	m_tTensLvRect;
	INFO	m_tTensLvInfo;
	FRAME   m_tTensFrame;

	RECT	m_tUnitLvRect;
	INFO	m_tUnitLvInfo;
	FRAME   m_tUnitFrame;

	int		m_iTensLevel;
	int		m_iUnitLevel;
};
