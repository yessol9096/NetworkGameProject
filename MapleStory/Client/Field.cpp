#include "StdAfx.h"
#include "Field.h"
#include "Mouse.h"
#include "Player.h"
#include "Rope.h"
#include "FirstFloor.h"
#include "SecondFloor.h"
#include "Shoot.h"
#include "UI.h"
#include "Leaf.h"
#include "Party.h"

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

	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMouse>::CreateObj(), OBJ_MOUSE);

	// �÷��̾�
	CObj* pPlayer = CAbstractFactory<CPlayer>::CreateObj();
	CObjMgr::GetInstance()->AddObject(pPlayer, OBJ_PLAYER);
	m_pPlayer = dynamic_cast<CPlayer*>(pPlayer);

	// pPlayer = CAbstractFactory<CPlayer>::CreateObj(300, 500);
	//CObjMgr::GetInstance()->AddObject(pPlayer, OBJ_PLAYER);


	// ����
	for (int i = 0; i < 7; ++i)
		CObjMgr::GetInstance()->AddObject(
			CAbstractFactory<CRope>::CreateObj(495.f, 300.f + i * TILECY), OBJ_ROPE);
	// 1�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(WINCX * 0.5f, 500.f, 1920.f, 20.f, FLOORBOX_WIDTH), OBJ_FIRSTFLOOR);
	// 1�� �ڽ� (����)
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(25.f, 380.f, 20.f, 300.f, FLOORBOX_HEIGHT), OBJ_FIRSTFLOOR);
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CFirstFloor>::CreateFloorBox(600.f, 380.f, 40.f, 300.f, FLOORBOX_HEIGHT), OBJ_FIRSTFLOOR);
	// 2�� �ڽ�
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CSecondFloor>::CreateFloorBox(300.f, 280.f, 488.f, 20.f, FLOORBOX_WIDTH), OBJ_SECONDFLOOR);

	// NPC
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CLeaf>::CreateObj(1250.f, 475.f), OBJ_NPC);

	// ��Ƽâ
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CParty>::CreateObj(), OBJ_UI);
}

int CField::Update()
{
	CObjMgr::GetInstance()->UpdateObj();

	// ��� �÷��̾ ��� ���°� �Ǹ� �� �Ѿ��
	for (int i = 0; i < g_vecplayer.size(); ++i)
	{
		if (g_vecplayer[i].id == -1)	// �÷��̾� ���� �ִ��� Ȯ��
			break;
		if (g_vecplayer[i].ready == false)	// �� �÷��̾ ��� ���°� �ƴ϶�� �Ѿ��
			break;

		if (i + 1 == g_vecplayer.size())	// ��� �÷��̾ ��� ���¶��
		{
			g_eScene = SCENE_STAGE1;
			CSceneMgr::GetInstance()->SetScene(SCENE_STAGE1);
			g_bIsSceneChange = true;
		}
	}

	// ������ ���ؼ� ^^ . ���߿� �����..
	if (KEYMGR->OnceKeyUp(VK_F1)) {
		CSceneMgr::GetInstance()->SetScene(SCENE_STAGE1);
		g_eScene = SCENE_STAGE1;
		g_bIsSceneChange = true;
	}
	return 0;
}

void CField::Render(HDC hDc)
{
	HDC hMemDC = (CBitmapMgr::GetInstance()->GetMapBit()[L"BackGround"])->GetMemDC();

	// ** �� �̷��� �ؾ� �Ǵ��� �� �Ф�
	BitBlt(hDc, static_cast<int>(g_fScrollX), static_cast<int>(-80 + (int)g_fScrollY),
		FIELDCX, FIELDCY, hMemDC, 0, 0, SRCCOPY);

	if (GetAsyncKeyState('1'))
		CLineMgr::GetInstance()->Render(hDc);

	CObjMgr::GetInstance()->RenderObj(hDc);
}

void CField::Release()
{
	CObjMgr::GetInstance()->ReleaseAll();
}