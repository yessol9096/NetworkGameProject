#pragma once

class CMaingame
{

public:
	CMaingame(void) {};
	~CMaingame(void) { Release(); };

	// 스킬 생성
private:
	template <typename T>
	static CObj*	CreateSkill(MYPOINT pt, OBJECT_DIR dir)
	{
		int fireX = 250; int fireY = 50;
		if (dir == DIR_LEFT)
			fireX *= -1.f;
		CObj* pSkill = CAbstractFactory<T>::CreateObj(
			pt.x + fireX, pt.y - fireY);
		pSkill->SetAtt(100);
		pSkill->SetDir(dir);
		return pSkill;
	}

public:
	void Initialize(void);
	void Update(void);
	void Render(void);
	void Release(void);
	static int recvn(SOCKET, char*, int, int);
	static DWORD WINAPI RecvThread(LPVOID);

private:
	HDC		m_hDC;
};
