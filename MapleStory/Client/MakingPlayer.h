#pragma once
#include "Scene.h"

class CMakingPlayer :
	public CScene
{
public:
	CMakingPlayer();
	virtual ~CMakingPlayer();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void Render(HDC hDc);
	virtual void Release();
	
private:
	// ���� �߰�.
	INITIALINFO m_initInfo;
	HDC				m_hDC;

	const TCHAR*	m_pImgName;
	int				sceneNum{ 0 };
};

