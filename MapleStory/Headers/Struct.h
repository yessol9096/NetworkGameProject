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
	int iFrameStart; // ��������Ʈ �̹����� x�� ������.
	int iFrameEnd; // ��������Ʈ �̹����� x�� ��������
	int iScene;		// ��������Ʈ �̹����� y��.
	DWORD dwFrameSpd; // �ִϸ��̼� ��� �ӵ�
}FRAME;