#pragma once

class CLine
{
public:
	CLine(void);
	CLine(LINEPOINT tLPoint, LINEPOINT tRPoint);
	~CLine(void);

public:
	const LINEINFO& GetInfo() { return m_tInfo; }

public:
	void Render(HDC hDC);

private:
	LINEINFO	m_tInfo;
};
