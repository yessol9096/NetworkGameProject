#pragma once

typedef struct tagInfo
{
	tagInfo() {}
	tagInfo(float x, float y, float cx, float cy)
		: fX(x), fY(y), fCX(cx), fCY(cy) {}

	float fX;
	float fY;
	float fCX;
	float fCY;
}INFO;

typedef struct tagState
{
	tagState() {}
	tagState(TCHAR* _pName, int _iAtt, int _iHp, int _iMaxHp, int _iMp, int _iMaxMp, int _iExp, int _iMaxExp, int _iLevel, int _iGold)
		: iAtt(_iAtt), iHp(_iHp), iMaxHp(_iMaxHp), iMp(_iMp), iMaxMp(_iMaxMp), iExp(_iExp), iMaxExp(_iMaxExp), iLevel(_iLevel), iGold(_iGold)
	{
		//strcpy_s(szName, 16, _pName);
		iHp = iMaxHp;
	}

	const TCHAR*	pName;
	int iAtt;
	int iHp;
	int iMaxHp;
	int iMp;
	int iMaxMp;
	int iExp;
	int iMaxExp;
	int iLevel;
	int iGold;
}STATE;



typedef struct tagLinePoint
{
	tagLinePoint() {}
	tagLinePoint(float fX, float fY)
	{
		this->fX = fX;
		this->fY = fY;
	}

	float	fX;
	float	fY;

}LINEPOINT;

typedef struct tagLineInfo
{
	tagLineInfo() {}
	tagLineInfo(LINEPOINT tLPoint, LINEPOINT tRPoint)
	{
		this->tLPoint = tLPoint;
		this->tRPoint = tRPoint;
	}

	LINEPOINT	tLPoint;
	LINEPOINT	tRPoint;
}LINEINFO;

typedef struct tagFrame
{
	int iFrameStart; // 스프라이트 이미지의 x축 시작점.
	int iFrameEnd; // 스프라이트 이미지의 x축 마지막점
	int iScene;		// 스프라이트 이미지의 y축.
	DWORD dwFrameSpd; // 애니메이션 재생 속도
}FRAME;