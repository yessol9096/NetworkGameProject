#pragma once
#include "obj.h"

class CSkill :
	public CObj
{
public:
	CSkill(void);
	virtual ~CSkill(void);

public:
	virtual void SetPlayer(CObj* pPlayer); 

	virtual void	UpdateCollRect() = 0;
	

protected:
	CObj*		m_pPlayer;
};
