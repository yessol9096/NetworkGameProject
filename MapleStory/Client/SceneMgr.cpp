#include "StdAfx.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "MakingPlayer.h"
#include "Field.h"
#include "Stage1.h"
#include "Stage2.h"
#include "BossStage.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr(void)
: m_pScene(NULL), m_bChangeCheck(false)
{
}

CSceneMgr::~CSceneMgr(void)
{
	Release();
}

void CSceneMgr::SetScene(SCENE_TYPE eType)
{
	m_eSceneType = eType;

	// 기존에 Scene이 할당 되어 있다면 해제할 것
	if(NULL != m_pScene)
	{
		SafeDelete<CScene*>(m_pScene);
// 		g_fScrollX = 0.f;
// 		g_fScrollY = 0.f;
	}

	switch(eType)
	{
	case SCENE_LOGO:
		m_pScene = new CLogo;
		break;
 	case SCENE_MAKINGPLAYER:
 		m_pScene = new CMakingPlayer;
 		break;
// 	case SCENE_EDIT:
// 		m_pScene = new CMyEdit;
// 		break;
	case SCENE_FIELD:
		m_pScene = new CField;
		break;
	case SCENE_STAGE1:
		m_pScene = new CStage1;
		break;
	//case SCENE_BOSS:
 //		m_pScene = new CBossStage;
	}
	m_pScene->Initialize();
}

void CSceneMgr::Update()
{
	if(m_bChangeCheck)
	{
		m_bChangeCheck = false;

		//
		SetScene(m_eSceneType);
	}

	if(m_pScene)
		m_pScene->Update();
}

void CSceneMgr::Render(HDC hDc)
{
	if(m_pScene)
		m_pScene->Render(hDc);
}

void CSceneMgr::Release()
{
	SafeDelete<CScene*>(m_pScene);
}

void CSceneMgr::SetChangeScene(SCENE_TYPE eType)
{
	m_eSceneType = eType;
	m_bChangeCheck = true;
}
