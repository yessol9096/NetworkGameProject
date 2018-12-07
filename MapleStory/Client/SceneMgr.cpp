#include "StdAfx.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "MakingPlayer.h"
#include "Field.h"
#include "Stage1.h"

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

	// Field�� �ִ� Player �����͸�, Stage1�� ���� �ֵ��� �Ѱ��־�� �Ѵ�.
	CPlayer* pPlayer = nullptr;
	CPlayer* pOtherPlayer = nullptr;
	// ������ Scene�� �Ҵ� �Ǿ� �ִٸ� ������ ��
	if (NULL != m_pScene)
	{
		if (g_eScene == SCENE_STAGE1) {
			// ��������1�� �Ѿ�� �ʵ� ���� ����� ����! player �����͸� �Ѱ� �ش�.
			pPlayer = dynamic_cast<CField*>(m_pScene)->GetPlayer();
			pOtherPlayer = dynamic_cast<CField*>(m_pScene)->GetOtherPlayer();
		}
		SafeDelete<CScene*>(m_pScene);
		//g_fScrollX = 0.f;
		//g_fScrollY = 0.f;
	}

	switch (eType)
	{
	case SCENE_LOGO:
		m_pScene = new CLogo;
		break;
	case SCENE_MAKINGPLAYER:
		m_pScene = new CMakingPlayer;
		break;
	case SCENE_FIELD:
		m_pScene = new CField;
		break;
	case SCENE_STAGE1:
		m_pScene = new CStage1;
		if (pPlayer != nullptr && pOtherPlayer != nullptr) {
			dynamic_cast<CStage1*>(m_pScene)->SetPlayer(pPlayer);
			dynamic_cast<CStage1*>(m_pScene)->SetPlayer(pOtherPlayer);
		}
		break;
	}
	m_pScene->Initialize();
}

void CSceneMgr::Update()
{
	if(m_bChangeCheck)
	{
		m_bChangeCheck = false;
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
