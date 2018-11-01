#include "StdAfx.h"
#include "Stage2.h"
#include "Player.h"
#include "Mouse.h"
#include "Rope.h"
#include "FirstFloor.h"
#include "SecondFloor.h"
#include "ThirdFloor.h"
#include "FifthFloor.h"
#include "FourthFloor.h"
#include "Portal.h"
#include "Shoot.h"

CStage2::CStage2(void)
{
}

CStage2::~CStage2(void)
{
	Release();
}

void CStage2::Initialize()
{
	CBitmapMgr::GetInstance()->LoadImageByScene(SCENE_STAGE2);

	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMouse>::CreateObj(), OBJ_MOUSE);

	// 1-2�� ����
	for(int i = 0; i < 8; ++i)
	{
		CObjMgr::GetInstance()->AddObject(
			CAbstractFactory<CRope>::CreateObj(492.f, 965.f + i * TILECY), OBJ_ROPE);
	}
	// 2-3�� ����1
	for(int i = 0; i < 8; ++i)
	{
		CObjMgr::GetInstance()->AddObject(
			CAbstractFactory<CRope>::CreateObj(1280.f, 700.f + i * TILECY), OBJ_ROPE);
	}
	// 3-4�� ����
	for(int i = 0; i < 8; ++i)
	{
		CObjMgr::GetInstance()->AddObject(
			CAbstractFactory<CRope>::CreateObj(492.f, 435.f + i * TILECY), OBJ_ROPE);
	}

	// 1�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(KRITIASCX * 0.5f, 1265.f, KRITIASCX, 20.f, FLOORBOX_WIDTH ), OBJ_FIRSTFLOOR);
	// 1�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(200.f, 835.f, 325.f, 800.f, FLOORBOX_HEIGHT ), OBJ_FIRSTFLOOR);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(1585.f, 835.f, 325.f, 800.f, FLOORBOX_HEIGHT  ), OBJ_FIRSTFLOOR);
	// 2�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CSecondFloor>::CreateFloorBox(KRITIASCX * 0.5f, 980.f, 1030.f , 20.f, FLOORBOX_WIDTH ), OBJ_SECONDFLOOR);
	// 3�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CThirdFloor>::CreateFloorBox(KRITIASCX * 0.5f, 720.f, 1030.f, 20.f, FLOORBOX_WIDTH ), OBJ_THIRDFLOOR);
	// 4�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFourthFloor>::CreateFloorBox(KRITIASCX * 0.5f, 450.f, 1030.f, 20.f, FLOORBOX_WIDTH ), OBJ_FOURTHFLOOR);

	// ��Ż
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CPortal>::CreatePortal(100.f, KRITIASCY - 330.f, 2), OBJ_PORTAL);

	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CPortal>::CreatePortal(KRITIASCX - 300.f, KRITIASCY - 330.f, 4), OBJ_PORTAL);

}

int CStage2::Update()
{
	CObjMgr::GetInstance()->UpdateObj();
	return 0;
}

void CStage2::Render(HDC hDc)
{
	HDC hMemDC = (CBitmapMgr::GetInstance()->
		GetMapBit()[L"BackGround_Kritias"])->GetMemDC();

	// ** �� �̷��� �ؾ� �Ǵ��� �� �Ф�
	BitBlt(hDc, static_cast<int>(g_fScrollX), static_cast<int>(30 + g_fScrollY), KRITIASCX, KRITIASCY,
		hMemDC, 0, 0, SRCCOPY);


	// Ÿ�� ������
	if(GetAsyncKeyState('1'))
		CLineMgr::GetInstance()->Render(hDc);
	CObjMgr::GetInstance()->RenderObj(hDc);

}

void CStage2::Release()
{
	CObjMgr::GetInstance()->ReleaseAll();
}
