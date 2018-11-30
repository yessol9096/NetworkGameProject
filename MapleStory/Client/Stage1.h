#pragma once
#include "scene.h"

class CStage1 :
	public CScene
{
public:
	CStage1(void);
	virtual ~CStage1(void);

public:
	virtual void Initialize();
	virtual int Update();
	virtual void Render(HDC hDc);
	virtual void Release();
	virtual void RecvMonsterInitalInfo();
};
