#pragma once
#include "obj.h"

class CPortal :
	public CObj
{
public:
	CPortal(void);
	virtual ~CPortal(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();

public:
	int GetWhereTogo() { return m_iWhereToGo; }
	void SetWhereTogo(int iWhereTogo) { m_iWhereToGo = iWhereTogo; }

public:
	void UpdateCollRect();

private:
	int	 m_iWhereToGo;
};
