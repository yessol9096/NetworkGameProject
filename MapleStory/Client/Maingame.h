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
	int recvn(SOCKET, char*, int , int );
private:
	HDC		m_hDC;
};
