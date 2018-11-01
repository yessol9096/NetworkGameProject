#pragma once
#include "item.h"

class CAccessory :
	public CItem
{
public:
	CAccessory(void);
	virtual ~CAccessory(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();

	virtual void Move();
	virtual void UpdateCollRect();
};
