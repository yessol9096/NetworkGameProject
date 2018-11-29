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
	static int recvn(SOCKET, char*, int , int );
	static DWORD WINAPI RecvThread(LPVOID);
private:
	HDC		m_hDC;
};
