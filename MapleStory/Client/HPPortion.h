#pragma once
#include "item.h"

class CHPPortion :
	public CItem
{
public:
	CHPPortion(void);
	virtual ~CHPPortion(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();
	virtual void	Move();
	virtual void	UpdateCollRect();

private:
	bool	m_bIsUsed;
	bool	m_bIsInStore; // false¸é framestart = 1, true¸é 0
};
