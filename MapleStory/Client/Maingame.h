#pragma once

class CMaingame
{
public:
	CMaingame(void);
	~CMaingame(void);
	
public:
	void Initialize(void);
	void Update(void);
	void Render(void);
	void Release(void);

private:
	HDC		m_hDC;
};
