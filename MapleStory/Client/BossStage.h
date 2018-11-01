#pragma once
#include "scene.h"

class CBossStage :
	public CScene
{
public:
	CBossStage(void);
	virtual ~CBossStage(void);
public:
	virtual void Initialize();
	virtual int Update();
	virtual void Render(HDC hDc);
	virtual void Release();
};
