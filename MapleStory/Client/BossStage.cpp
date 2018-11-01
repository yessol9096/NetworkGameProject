#include "StdAfx.h"
#include "BossStage.h"
#include "Player.h"
#include "Mouse.h"
#include "Rope.h"
#include "FirstFloor.h"
#include "Portal.h"
#include "Shoot.h"
#include "Boss.h"
#include "UI.h"

CBossStage::CBossStage(void)
{
}

CBossStage::~CBossStage(void)
{
	Release();
}

void CBossStage::Initialize()
{
	CBitmapMgr::GetInstance()->LoadImageByScene(SCENE_BOSS);
	// UI
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CUI>::CreateObj(), OBJ_UI);

	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMouse>::CreateObj(), OBJ_MOUSE);

	// 1층 박스 (가로)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(BOSSMAPCX * 0.5f, 1265.f, BOSSMAPCX, 20.f, FLOORBOX_WIDTH ), OBJ_FIRSTFLOOR);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CBoss>::CreateObj(BOSSMAPCX * 0.5f, 215.f), OBJ_BOSS);


}

int CBossStage::Update()
{
	CObjMgr::GetInstance()->UpdateObj();
	return 0;
}

void CBossStage::Render(HDC hDc)
{
	HDC hMemDC = (CBitmapMgr::GetInstance()->
		GetMapBit()[L"BackGround_Boss"])->GetMemDC();

	// ** 왜 이렇게 해야 되는지 모름 ㅠㅠ
	BitBlt(hDc, static_cast<int>(g_fScrollX), static_cast<int>(-105 + g_fScrollY), BOSSMAPCX, BOSSMAPCY,
		hMemDC, 0, 0, SRCCOPY);


	// 타일 버리자
	if(GetAsyncKeyState('1'))
		CLineMgr::GetInstance()->Render(hDc);
	CObjMgr::GetInstance()->RenderObj(hDc);

}

void CBossStage::Release()
{
	CObjMgr::GetInstance()->ReleaseAll();
}
