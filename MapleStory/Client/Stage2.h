#pragma once
#include "scene.h"

class CStage2 :
	public CScene
{
public:
	CStage2(void);
	virtual ~CStage2(void);

public:
	virtual void Initialize();
	virtual int Update();
	virtual void Render(HDC hDc);
	virtual void Release();
};
