#include "StdAfx.h"
#include "Field.h"
#include "Mouse.h"
#include "Player.h"
#include "Rope.h"
#include "FirstFloor.h"
#include "SecondFloor.h"
#include "Portal.h"
#include "Shoot.h"
#include "UI.h"
#include "Inven.h"

CField::CField(void)
{
}

CField::~CField(void)
{
	Release();
}

void CField::Initialize()
{
	CBitmapMgr::GetInstance()->LoadImageByScene(SCENE_FIELD);

	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CUI>::CreateObj(), OBJ_UI);

	// 마우스
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMouse>::CreateObj(), OBJ_MOUSE);

	// 플레이어
	CObj* pPlayer = CAbstractFactory<CPlayer>::CreateObj();
	CObjMgr::GetInstance()->AddObject(pPlayer
		, OBJ_PLAYER);

	// 밧줄
	for(int i = 0; i < 7; ++i)
	{
		CObjMgr::GetInstance()->AddObject(
			CAbstractFactory<CRope>::CreateObj(495.f, 300.f + i * TILECY), OBJ_ROPE);
	}

	
	// 1층 박스 (가로)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(WINCX * 0.5f, 500.f, 1920.f, 20.f, FLOORBOX_WIDTH ), OBJ_FIRSTFLOOR);

	// 1층 박스 (세로)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(25.f, 380.f, 20.f, 300.f, FLOORBOX_HEIGHT ), OBJ_FIRSTFLOOR);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(600.f, 380.f, 40.f, 300.f, FLOORBOX_HEIGHT ), OBJ_FIRSTFLOOR);

	// 2층 박스
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CSecondFloor>::CreateFloorBox(300.f, 280.f, 488.f, 20.f, FLOORBOX_WIDTH ), OBJ_SECONDFLOOR);

	// 포탈
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CPortal>::CreatePortal(1800.f, 390.f, 2), OBJ_PORTAL);

	// 인벤토리
	//CObjMgr::GetInstance()->AddObject(
	//	CAbstractFactory<CInven>::CreateInventory(WINCX * 0.3f, WINCY * 0.3f, pPlayer), OBJ_INVENTORY);

	

	//CTileMgr::GetInstance()->LoadTile();

	LoadData();
}

int CField::Update()
{
	CObjMgr::GetInstance()->UpdateObj();
	//CTileMgr::GetInstance()->Update();

	return 0;
}

void CField::Render(HDC hDc)
{
	HDC hMemDC = (CBitmapMgr::GetInstance()->
		GetMapBit()[L"BackGround"])->GetMemDC();

	// ** 왜 이렇게 해야 되는지 모름 ㅠㅠ
	BitBlt(hDc, static_cast<int>(g_fScrollX), static_cast<int>(-80 + (int)g_fScrollY), FIELDCX, FIELDCY,
		hMemDC, 0, 0, SRCCOPY);


	if(GetAsyncKeyState('1'))
		CLineMgr::GetInstance()->Render(hDc);
	// 타일 버리자
	//CTileMgr::GetInstance()->Render(hDc);
	CObjMgr::GetInstance()->RenderObj(hDc);
}

void CField::Release()
{
	//CTileMgr::GetInstance()->DestroyInstance();
	CObjMgr::GetInstance()->ReleaseAll();
}

void CField::UpdateTile()
{


}

void CField::RenderTile(HDC hDc)
{

}

void CField::LoadData()
{


}
