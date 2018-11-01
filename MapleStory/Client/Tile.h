#pragma once
#include "obj.h"

class CTile :
	public CObj
{
public:
	CTile(void);
	virtual ~CTile(void);


public:
	const int& GetDrawID() { return m_iDrawID; }
	const int& GetOption() { return m_iOption; }

public:
	void SetDrawID(int iDrawID) { m_iDrawID = iDrawID; }
	void SetOption(int iOption) { m_iOption = iOption; }

public:
	virtual void Initialize(void);
	virtual int Update(void);
	virtual void Render(HDC hDc);
	virtual void Release(void);
	virtual void FrameMove();

private:
	int m_iDrawID;
	int m_iOption;

};
