#include "StdAfx.h"
#include "Stage1.h"
#include "Player.h"
#include "Mouse.h"
#include "Rope.h"
#include "FirstFloor.h"
#include "SecondFloor.h"
#include "ThirdFloor.h"
#include "FifthFloor.h"
#include "FourthFloor.h"
#include "Green.h"
#include "Shoot.h"
#include "UI.h"
#include "Result.h"

CStage1::CStage1(void)
{
}

CStage1::~CStage1(void)
{
	Release();
}

void CStage1::Initialize()
{
	CBitmapMgr::GetInstance()->LoadImageByScene(SCENE_STAGE1);

	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CUI>::CreateObj(), OBJ_UI);

	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMouse>::CreateObj(), OBJ_MOUSE);

	// 1-2�� ����
	for (int i = 0; i < 8; ++i)
		CObjMgr::GetInstance()->AddObject(
			CAbstractFactory<CRope>::CreateObj(635.f, HENESISCY - 440.f + i * TILECY), OBJ_ROPE);
	// 2-3�� ����
	for (int i = 0; i < 8; ++i)
		CObjMgr::GetInstance()->AddObject(
			CAbstractFactory<CRope>::CreateObj(995.f, HENESISCY - 700.f + i * TILECY), OBJ_ROPE);
	// 3-4�� ����
	for (int i = 0; i < 8; ++i)
		CObjMgr::GetInstance()->AddObject(
			CAbstractFactory<CRope>::CreateObj(790.f, HENESISCY - 910.f + i * TILECY), OBJ_ROPE);
	// 4-5�� ����
	for (int i = 0; i < 8; ++i)
		CObjMgr::GetInstance()->AddObject(
			CAbstractFactory<CRope>::CreateObj(1055.f, HENESISCY - 1170.f + i * TILECY), OBJ_ROPE);
	// 1�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(HENESISCX * 0.5f, HENESISCY - 220.f, HENESISCX, 20.f, FLOORBOX_WIDTH), OBJ_FIRSTFLOOR);
	// 1�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(150.f, 1035.f, 230.f, 420.f, FLOORBOX_HEIGHT), OBJ_FIRSTFLOOR);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(1705.f, 1035.f, 230.f, 420.f, FLOORBOX_HEIGHT), OBJ_FIRSTFLOOR);
	// 2�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CSecondFloor>::CreateFloorBox(HENESISCX * 0.5f + 30.f, HENESISCY - 456.f, 1300.f, 20.f, FLOORBOX_WIDTH), OBJ_SECONDFLOOR);
	// 2�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CSecondFloor>::CreateFloorBox(1550.f, 750.f, 100.f, 500.f, FLOORBOX_HEIGHT), OBJ_SECONDFLOOR);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CSecondFloor>::CreateFloorBox(320.f, 750.f, 100.f, 500.f, FLOORBOX_HEIGHT), OBJ_SECONDFLOOR);
	// 3�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CThirdFloor>::CreateFloorBox(HENESISCX * 0.5f + 30.f, HENESISCY - 700.f, 1110.f, 20.f, FLOORBOX_WIDTH), OBJ_THIRDFLOOR);
	// 3�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CThirdFloor>::CreateFloorBox(410.f, 590.f, 78.f, 360.f, FLOORBOX_HEIGHT), OBJ_THIRDFLOOR);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CThirdFloor>::CreateFloorBox(1445.f, 590.f, 78.f, 360.f, FLOORBOX_HEIGHT), OBJ_THIRDFLOOR);
	// 4�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFourthFloor>::CreateFloorBox(HENESISCX * 0.5f + 30.f, HENESISCY - 935.f, 930.f, 20.f, FLOORBOX_WIDTH), OBJ_FOURTHFLOOR);
	// 4�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFourthFloor>::CreateFloorBox(505.f, 365.f, 80.f, 315.f, FLOORBOX_HEIGHT), OBJ_FOURTHFLOOR);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFourthFloor>::CreateFloorBox(1365.f, 365.f, 80.f, 315.f, FLOORBOX_HEIGHT), OBJ_FOURTHFLOOR);
	// 5�� �ڽ�
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFifthFloor>::CreateFloorBox(HENESISCX * 0.5f + 30.f, HENESISCY - 1180.f, 760.f, 20.f, FLOORBOX_WIDTH), OBJ_FIFTHFLOOR);

	// ���â
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CResult>::CreateObj(), OBJ_UI);

	///////////////////////////////// ����
	// �ʷϹ���  (2��)
	for (int i = 0; i < MAX_GREEN; ++i)
	{
		CObjMgr::GetInstance()->AddObject(
			CAbstractFactory<CGreen>::CreateMonster(g_vecgreen[i]), OBJ_MONSTER);
		//cout << "�׸��׸��µ�" << g_vecgreen[i].pt.x << endl;
	}
}

int CStage1::Update()
{
#ifdef DEBUG_1206
	cout << "----------���� ���� ����-----------" << endl;
	cout << "[0��° Ŭ���̾�Ʈ] �г��� : " << g_vecplayer[0].nickname << ", ���� : " << g_vecplayer[0].job << "��ǥ (" << g_vecplayer[0].pt.x << ", " << g_vecplayer[0].pt.y << ")" << endl;
	cout << "[1��° Ŭ���̾�Ʈ] �г��� : " << g_vecplayer[1].nickname << ", ���� : " << g_vecplayer[1].job << "��ǥ (" << g_vecplayer[1].pt.x << ", " << g_vecplayer[1].pt.y << ")" << endl;
#endif

	CObjMgr::GetInstance()->UpdateObj();

	OBJLIST MonsterList = CObjMgr::GetInstance()->GetMonsterOBJList();
	// ���� ��ǥ 
	for (int i = 0; i < OBJ_END; ++i)
	{
		OBJITER iter_begin = MonsterList.begin();
		OBJITER iter_end = MonsterList.end();
		int j = 0;
		for (; iter_begin != iter_end; ++iter_begin)
		{
			(*iter_begin)->SetPos(g_vecgreen[j].pt.x, g_vecgreen[j].pt.y);
			(*iter_begin)->SetDir(g_vecgreen[j].dir);
			g_vecgreen[j].hp = (*iter_begin)->GetState().iHp;
			++j;
		}
	}
	return 0;
}

void CStage1::Render(HDC hDc)
{
	HDC hMemDC = (CBitmapMgr::GetInstance()->GetMapBit()[L"BackGround_Henesis"])->GetMemDC();

	// ** �� �̷��� �ؾ� �Ǵ��� �� �Ф�
	BitBlt(hDc, static_cast<int>(g_fScrollX), 30 + static_cast<int>(g_fScrollY), HENESISCX, HENESISCY,
		hMemDC, 0, 0, SRCCOPY);

	if (GetAsyncKeyState('1'))
		CLineMgr::GetInstance()->Render(hDc);
	CObjMgr::GetInstance()->RenderObj(hDc);
}

void CStage1::Release()
{
	CObjMgr::GetInstance()->ReleaseAll();
}