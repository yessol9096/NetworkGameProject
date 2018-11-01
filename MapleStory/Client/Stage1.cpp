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
#include "Portal.h"
#include "Green.h"
#include "MushCouple.h"
#include "Shoot.h"
#include "Leaf.h"
#include "UI.h"




void CStage1::Initialize()
{
	CBitmapMgr::GetInstance()->LoadImageByScene(SCENE_STAGE1);

	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMouse>::CreateObj(), OBJ_MOUSE);
	// UI
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CUI>::CreateObj(), OBJ_UI);

	// 1-2Ãþ ¹åÁÙ
	for(int i = 0; i < 8; ++i)
	{
		CObjMgr::GetInstance()->AddObject(
			CAbstractFactory<CRope>::CreateObj(635.f, HENESISCY - 440.f + i * TILECY), OBJ_ROPE);
	}
	// 2-3Ãþ ¹åÁÙ
	for(int i = 0; i < 8; ++i)
	{
		CObjMgr::GetInstance()->AddObject(
			CAbstractFactory<CRope>::CreateObj(995.f, HENESISCY - 700.f + i * TILECY), OBJ_ROPE);
	}
	// 3-4Ãþ ¹åÁÙ
	for(int i = 0; i < 8; ++i)
	{
		CObjMgr::GetInstance()->AddObject(
			CAbstractFactory<CRope>::CreateObj(790.f, HENESISCY - 910.f + i * TILECY), OBJ_ROPE);
	}
	// 4-5Ãþ ¹åÁÙ
	for(int i = 0; i < 8; ++i)
	{
		CObjMgr::GetInstance()->AddObject(
			CAbstractFactory<CRope>::CreateObj(1055.f, HENESISCY - 1170.f + i * TILECY), OBJ_ROPE);
	}

	// 1Ãþ ¹Ú½º (°¡·Î)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(HENESISCX * 0.5f, HENESISCY - 220.f, HENESISCX, 20.f, FLOORBOX_WIDTH ), OBJ_FIRSTFLOOR);
	// 1Ãþ ¹Ú½º (¼¼·Î)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(150.f, 1035.f, 230.f, 420.f, FLOORBOX_HEIGHT ), OBJ_FIRSTFLOOR);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(1705.f, 1035.f, 230.f, 420.f, FLOORBOX_HEIGHT  ), OBJ_FIRSTFLOOR);
	// 2Ãþ ¹Ú½º (°¡·Î)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CSecondFloor>::CreateFloorBox(HENESISCX * 0.5f + 30.f, HENESISCY - 456.f, 1300.f , 20.f, FLOORBOX_WIDTH ), OBJ_SECONDFLOOR);
	// 2Ãþ ¹Ú½º (¼¼·Î)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CSecondFloor>::CreateFloorBox(1550.f, 750.f, 100.f , 500.f, FLOORBOX_HEIGHT  ), OBJ_SECONDFLOOR);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CSecondFloor>::CreateFloorBox(320.f, 750.f, 100.f , 500.f , FLOORBOX_HEIGHT ), OBJ_SECONDFLOOR);
	// 3Ãþ ¹Ú½º (°¡·Î)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CThirdFloor>::CreateFloorBox(HENESISCX * 0.5f + 30.f, HENESISCY - 700.f, 1110.f, 20.f, FLOORBOX_WIDTH ), OBJ_THIRDFLOOR);
	// 3Ãþ ¹Ú½º (¼¼·Î)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CThirdFloor>::CreateFloorBox(410.f, 590.f, 78.f, 360.f, FLOORBOX_HEIGHT  ), OBJ_THIRDFLOOR);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CThirdFloor>::CreateFloorBox(1445.f, 590.f, 78.f, 360.f, FLOORBOX_HEIGHT  ), OBJ_THIRDFLOOR);
	// 4Ãþ ¹Ú½º (°¡·Î)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFourthFloor>::CreateFloorBox(HENESISCX * 0.5f + 30.f, HENESISCY - 935.f, 930.f, 20.f, FLOORBOX_WIDTH ), OBJ_FOURTHFLOOR);
	// 4Ãþ ¹Ú½º (¼¼·Î)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFourthFloor>::CreateFloorBox(505.f, 365.f, 80.f, 315.f, FLOORBOX_HEIGHT  ), OBJ_FOURTHFLOOR);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFourthFloor>::CreateFloorBox(1365.f, 365.f, 80.f, 315.f, FLOORBOX_HEIGHT  ), OBJ_FOURTHFLOOR);
	// 5Ãþ ¹Ú½º
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFifthFloor>::CreateFloorBox(HENESISCX * 0.5f + 30.f, HENESISCY - 1180.f, 760.f, 20.f, FLOORBOX_WIDTH ), OBJ_FIFTHFLOOR);



	//// ¸ó½ºÅÍ
	// ÃÊ·Ï¹ö¼¸  (2Ãþ)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CGreen>::CreateMonster(HENESISCX * 0.5f, HENESISCY - 460.f, DIR_LEFT, 1), OBJ_MONSTER);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CGreen>::CreateMonster(HENESISCX * 0.7f, HENESISCY - 460.f, DIR_RIGHT, 2), OBJ_MONSTER);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CGreen>::CreateMonster(HENESISCX * 0.6f, HENESISCY - 460.f, DIR_RIGHT, 3), OBJ_MONSTER);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CGreen>::CreateMonster(HENESISCX * 0.7f, HENESISCY - 460.f, DIR_LEFT, 1), OBJ_MONSTER);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CGreen>::CreateMonster(HENESISCX * 0.5f, HENESISCY - 460.f, DIR_RIGHT, 1), OBJ_MONSTER);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CGreen>::CreateMonster(HENESISCX * 0.4f, HENESISCY - 460.f, DIR_RIGHT, 3), OBJ_MONSTER);
	// Ä¿ÇÃ¹ö¼¸  (3Ãþ)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMushCouple>::CreateMonster(HENESISCX * 0.5f, HENESISCY - 700.f, DIR_RIGHT, 1), OBJ_MONSTER);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMushCouple>::CreateMonster(HENESISCX * 0.4f, HENESISCY - 700.f, DIR_RIGHT, 2), OBJ_MONSTER);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMushCouple>::CreateMonster(HENESISCX * 0.7f, HENESISCY - 700.f, DIR_RIGHT, 3), OBJ_MONSTER);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMushCouple>::CreateMonster(HENESISCX * 0.5f, HENESISCY - 700.f, DIR_LEFT, 3), OBJ_MONSTER);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMushCouple>::CreateMonster(HENESISCX * 0.5f, HENESISCY - 700.f, DIR_LEFT, 3), OBJ_MONSTER);

	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMushCouple>::CreateMonster(HENESISCX * 0.5f, HENESISCY - 700.f, DIR_LEFT, 3), OBJ_MONSTER);

	// (4Ãþ)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMushCouple>::CreateMonster(HENESISCX * 0.5f, HENESISCY - 935.f, DIR_RIGHT, 1), OBJ_MONSTER);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMushCouple>::CreateMonster(HENESISCX * 0.4f, HENESISCY - 935.f, DIR_RIGHT, 2), OBJ_MONSTER);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CGreen>::CreateMonster(HENESISCX * 0.8f, HENESISCY - 935.f, DIR_LEFT, 1), OBJ_MONSTER);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CGreen>::CreateMonster(HENESISCX * 0.5f, HENESISCY - 935.f, DIR_RIGHT, 1), OBJ_MONSTER);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CGreen>::CreateMonster(HENESISCX * 0.4f, HENESISCY - 935.f, DIR_RIGHT, 3), OBJ_MONSTER);


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// NPC (1Ãþ)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CLeaf>::CreateObj(HENESISCX * 0.5f, HENESISCY - 220.f), OBJ_NPC);






	// Æ÷Å»
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CPortal>::CreatePortal(100.f, HENESISCY - 300.f, 1), OBJ_PORTAL);

	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CPortal>::CreatePortal(HENESISCX - 300.f, HENESISCY - 300.f, 4), OBJ_PORTAL);


	//LoadData();
}

int CStage1::Update()
{
	CObjMgr::GetInstance()->UpdateObj();
	return 0;
}

void CStage1::Render(HDC hDc)
{
	HDC hMemDC = (CBitmapMgr::GetInstance()->
		GetMapBit()[L"BackGround_Henesis"])->GetMemDC();

	// ** ¿Ö ÀÌ·¸°Ô ÇØ¾ß µÇ´ÂÁö ¸ð¸§ ¤Ð¤Ð
	BitBlt(hDc, static_cast<int>(g_fScrollX), 30 + static_cast<int>(g_fScrollY), HENESISCX, HENESISCY,
		hMemDC, 0, 0, SRCCOPY);


	// Å¸ÀÏ ¹ö¸®ÀÚ
	if(GetAsyncKeyState('1'))
		CLineMgr::GetInstance()->Render(hDc);
	CObjMgr::GetInstance()->RenderObj(hDc);

}

void CStage1::Release()
{
	CObjMgr::GetInstance()->ReleaseAll();
}

void CStage1::UpdateTile()
{

}

void CStage1::RenderTile(HDC hDc)
{

}

void CStage1::LoadData()
{

}
CStage1::CStage1(void)
{
}

CStage1::~CStage1(void)
{
	Release();
}