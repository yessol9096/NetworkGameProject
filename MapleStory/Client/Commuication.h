#pragma once
#include "obj.h"

class CCommuication :
	public CObj
{
public:
	CCommuication(void);
	virtual ~CCommuication(void);

public:
	void SetRenderNumber(int iNumber) { m_tFrame.iFrameStart = iNumber; }
	void SetIsSucessed(bool bIsSucessed) { m_bIsSucessed = bIsSucessed; }
	virtual void SetNextButtonRect();

protected:
	int		m_iRenderNumber;
	int		m_iRenderNumberOld;

	bool	m_bIsSucessed;
	RECT	m_tNextButton;
};
