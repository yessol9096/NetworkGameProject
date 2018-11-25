#pragma once
#include "Item.h"

class CGold :
	public CItem
{
public:
	CGold(void);
	virtual ~CGold(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();
	virtual void	Move();
	virtual void	UpdateCollRect();	

	void SetGold(int iGold) { m_iGold = iGold; }
	void SetBossGold(bool bIsBossGold) { m_bIsBossGold = bIsBossGold; }
	void SetIsXReverse(bool bIsXReverse) { m_bIsXReverse = bIsXReverse; }

private:
	int			m_iGold;
	GOLD_ID		m_eID;
	bool		m_bIsBossGold;
	bool		m_bIsXReverse;
};
