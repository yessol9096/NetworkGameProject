#pragma once

#include "Skill.h"

class CShoot :
	public CSkill
{
public:
	CShoot(void);
	virtual ~CShoot(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();
	virtual void	UpdateCollRect();

	// ����Ʈ ��ȿ �ð�?
//private:


	
};
