#pragma once
#include "scene.h"

class CField :
	public CScene
{
public:
	CField(void);
	virtual ~CField(void);

public:
	virtual void Initialize();
	virtual int Update();
	virtual void Render(HDC hDc);
	virtual void Release();
};
