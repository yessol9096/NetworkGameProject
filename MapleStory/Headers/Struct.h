#pragma once

// ��ȹ �κ�
typedef struct tagFrame
{
	int			iFrameStart; // ��������Ʈ �̹����� x�� ������.
	int			iFrameEnd; // ��������Ʈ �̹����� x�� ��������
	int			iScene;		// ��������Ʈ �̹����� y��.
	DWORD		dwFrameSpd; // �ִϸ��̼� ��� �ӵ�
}FRAME;

typedef struct MySize {
	MySize() {}
	MySize(float cx, float cy) : cx(cx), cy(cy) {}
	float cx;
	float cy;
}MYSIZE;

typedef struct MyPoint {
	MyPoint() {}
	MyPoint(float x, float y) : x(x), y(y) {}
	float x;
	float y;
}MYPOINT;

typedef struct Info {
	OBJECT_TYPE type;
	MYPOINT		pt;
	MYSIZE		size;
	FRAME		frame;
	int			hp;
}INFO;

typedef struct PlayerInfo {
	short		id;
	char		nickname[512];
	bool		ready;
	MYPOINT		pt;
	MYSIZE		size;
	PLAYER_JOB	job;
	int			hp;
	int			money;
	FRAME		frame;
	float		attackAccValue;
	bool		connected; // 1119 �߰�
}PLAYERINFO;

typedef struct MonsterInfo {
	int			id; //vector index ��
	MYPOINT		pt;
	int			hp;
	int			money;
	OBJECT_DIR  dir;
	int			pattern;
}MONSTERINFO;

typedef struct PacketInfo {
	PACKET_TYPE type;
	int					size;
	int					id{ -1 };
}PACKETINFO;

//--------------------------------------------------------------------------------

//typedef struct tagInfo
//{
//	tagInfo() {}
//	tagInfo(float x, float y, float cx, float cy)
//		: fX(x), fY(y), size.cx(cx), size.cy(cy) {}
//
//	float fX;
//	float fY;
//	float size.cx;
//	float size.cy;
//}INFO;

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
	tagLineInfo(MYPOINT tLPoint, MYPOINT tRPoint)
	{
		this->tLPoint = tLPoint;
		this->tRPoint = tRPoint;
	}

	MYPOINT	tLPoint;
	MYPOINT	tRPoint;
}LINEINFO;

