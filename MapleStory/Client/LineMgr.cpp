#include "StdAfx.h"
#include "LineMgr.h"
#include "Line.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CLineMgr)

CLineMgr::CLineMgr(void)
{
	Initialize();
}

CLineMgr::~CLineMgr(void)
{
	Release();
}

void CLineMgr::Initialize()
{
	m_eLineID = LINE_FIELD;

	LoadData();
}

void CLineMgr::Render(HDC hDC)
{
	switch(g_eScene)
	{
	case SCENE_FIELD:
		m_eLineID = LINE_FIELD;
		break;
	case SCENE_STAGE1:
		m_eLineID = LINE_STAGE1;
		break;
	case SCENE_STAGE2:
		m_eLineID = LINE_STAGE2;
		break;
	case SCENE_BOSS:
		m_eLineID = LINE_BOSS;
		break;
	}
	list<CLine*>::iterator iter_begin = m_LineList[m_eLineID].begin();
	list<CLine*>::iterator iter_end = m_LineList[m_eLineID].end();

	for(; iter_begin != iter_end; ++iter_begin)
		(*iter_begin)->Render(hDC);
}

void CLineMgr::Release()
{
	for_each(m_LineList[m_eLineID].begin(), m_LineList[m_eLineID].end(), SafeDelete<CLine*>);
	m_LineList[m_eLineID].clear();
}

bool CLineMgr::LineCollision(CObj* pPlayer, float fX, float* pY)
{
	if(m_LineList[m_eLineID].empty())  return false;

	if(NULL == pY || NULL == pPlayer)		return false;

	CLine* pLine = NULL; // 1층 라인
	CLine* pLine2 = NULL; // 2층 라인
	CLine* pLine3 = NULL;
	CLine* pLine4 = NULL;
	CLine* pLine5 = NULL;

	bool bCheck = false;

	//float fFirstLineY = 0.f, fSecondLineY = 0.f, fThirdLineY = 0.f, fFourthLineY = 0.f, fFifthLineY = 0.f; // 라인의 높이값 비교 위함


	

	//방법3. 노가다로 넣기
	if(SCENE_FIELD == g_eScene)
	{
		m_eLineID = LINE_FIELD;
		list<CLine*>::iterator iter_begin = (m_LineList[m_eLineID]).begin();
		list<CLine*>::iterator iter_end = (m_LineList[m_eLineID]).end();

		pLine = (*iter_begin);
		++iter_begin;
		++iter_begin;
		pLine2 = (*iter_begin);
	}
	if(SCENE_STAGE1 == g_eScene)
	{
		m_eLineID = LINE_STAGE1;
		list<CLine*>::iterator iter_begin = (m_LineList[m_eLineID]).begin();
		list<CLine*>::iterator iter_end = (m_LineList[m_eLineID]).end();

		pLine = (*iter_begin);
		++iter_begin;
		++iter_begin;
		pLine2 = (*iter_begin);
		++iter_begin;
		++iter_begin;
		pLine3 = (*iter_begin);
		++iter_begin;
		++iter_begin;
		pLine4 = (*iter_begin);
		++iter_begin;
		++iter_begin;
		pLine5 = (*iter_begin);
	}
	if(SCENE_STAGE2 == g_eScene)
	{
		m_eLineID = LINE_STAGE2;
		list<CLine*>::iterator iter_begin = (m_LineList[m_eLineID]).begin();
		list<CLine*>::iterator iter_end = (m_LineList[m_eLineID]).end();
		pLine = (*iter_begin);
		++iter_begin;
		++iter_begin;
		pLine2 = (*iter_begin);
		++iter_begin;
		++iter_begin;
		pLine3 = (*iter_begin);
		++iter_begin;
		++iter_begin;
		pLine4 = (*iter_begin);
	}
	if(SCENE_BOSS == g_eScene)
	{
		m_eLineID = LINE_BOSS;
		list<CLine*>::iterator iter_begin = (m_LineList[m_eLineID]).begin();
		list<CLine*>::iterator iter_end = (m_LineList[m_eLineID]).end();
		pLine = (*iter_begin);
	}


	// 방법1.
// 	pLine = (*iter_begin);
// 	//fFirstLineY = (*iter_begin)->GetInfo().tLPoint.fY;
// 
// 	// FirstLine 걸러내기
// 	for(; iter_begin != iter_end; ++iter_begin)
// 	{
// 		if(pLine->GetInfo().tLPoint.fY < (*iter_begin)->GetInfo().tLPoint.fY)
// 		{
// 			pLine = (*iter_begin);
// 			break;
// 		}
// 		else
// 		{
// 			pLine2 = (*iter_begin);
// 			continue;
// 		}
// 	}
// 
// 	// SecondLine 걸러내기
// 	for(; iter_begin != iter_end; ++iter_begin)
// 	{
// 		// 첫번째 라인보다 위에 있으면 우선 line2에 넣고
// 		if(pLine->GetInfo().tLPoint.fY > (*iter_begin)->GetInfo().tLPoint.fY)
// 		{
// 			pLine2 = (*iter_begin);
// 
// 			// 근데 들어온 iter_begin이 기존 pLine2보다 더 위에 있으면
// 			if(pLine2->GetInfo().tLPoint.fY > (*iter_begin)->GetInfo().tLPoint.fY)
// 			{
// 				continue;
// 			}
// 
// 			break;
// 		}
// 		else
// 		{
// 			continue;
// 		}
// 	}
		


// 방법2.
		// 1층, 2층 나누기
// 		if(fFirstLineY > (*iter_begin)->GetInfo().tLPoint.fY)
// 		{
// 			fSecondLineY = (*iter_begin)->GetInfo().tLPoint.fY;
// 			
// 			// 플레이어 - 라인 Lpoint RPoint 비교
// 			if(fX >= (*iter_begin)->GetInfo().tLPoint.fX &&
// 				fX <= (*iter_begin)->GetInfo().tRPoint.fX)
// 			{
// 				pLine2 = (*iter_begin);
// 				continue;
// 			}
// 		}
// 		if(fFirstLineY <= (*iter_begin)->GetInfo().tLPoint.fY)
// 		{
// 			if((*iter_begin)->GetInfo().tRPoint.fX - (*iter_begin)->GetInfo().tLPoint.fX <= 50)
// 				continue;
// 			fFirstLineY = (*iter_begin)->GetInfo().tLPoint.fY;
// 			pLine = (*iter_begin);
// 
// 			// 플레이어 - 라인 Lpoint RPoint 비교
// 			if(fX >= (*iter_begin)->GetInfo().tLPoint.fX &&
// 				fX <= (*iter_begin)->GetInfo().tRPoint.fX)
// 			{
// 				pLine = (*iter_begin);
// 				continue;
// 			}
// 		}
// 
// 
// 		
// 	}

	if(1 == dynamic_cast<CPlayer*>(pPlayer)->GetPlayerFloor() && pLine != NULL)
	{
		float x1 = pLine->GetInfo().tLPoint.fX;
		float y1 = pLine->GetInfo().tLPoint.fY;
		float x2 = pLine->GetInfo().tRPoint.fX;
		float y2 = pLine->GetInfo().tRPoint.fY;


		float fGradient = float(y2 - y1) / (x2 - x1);
		*pY = fGradient * (fX - x1) + y1;

		bCheck = true;
	}

	else if(2 == dynamic_cast<CPlayer*>(pPlayer)->GetPlayerFloor() && pLine2 != NULL)
	{
		float x1 = pLine2->GetInfo().tLPoint.fX;
		float y1 = pLine2->GetInfo().tLPoint.fY;
		float x2 = pLine2->GetInfo().tRPoint.fX;
		float y2 = pLine2->GetInfo().tRPoint.fY;


		float fGradient = float(y2 - y1) / (x2 - x1);
		*pY = fGradient * (fX - x1) + y1;

		bCheck = true;
	}

	else if(3 == dynamic_cast<CPlayer*>(pPlayer)->GetPlayerFloor() && pLine3 != NULL)
	{
		float x1 = pLine3->GetInfo().tLPoint.fX;
		float y1 = pLine3->GetInfo().tLPoint.fY;
		float x2 = pLine3->GetInfo().tRPoint.fX;
		float y2 = pLine3->GetInfo().tRPoint.fY;


		float fGradient = float(y2 - y1) / (x2 - x1);
		*pY = fGradient * (fX - x1) + y1;

		bCheck = true;
	}


	else if(4 == dynamic_cast<CPlayer*>(pPlayer)->GetPlayerFloor() && pLine4 != NULL)
	{
		float x1 = pLine4->GetInfo().tLPoint.fX;
		float y1 = pLine4->GetInfo().tLPoint.fY;
		float x2 = pLine4->GetInfo().tRPoint.fX;
		float y2 = pLine4->GetInfo().tRPoint.fY;


		float fGradient = float(y2 - y1) / (x2 - x1);
		*pY = fGradient * (fX - x1) + y1;

		bCheck = true;
	}

	else if(5 == dynamic_cast<CPlayer*>(pPlayer)->GetPlayerFloor() && pLine5 != NULL)
	{
		float x1 = pLine5->GetInfo().tLPoint.fX;
		float y1 = pLine5->GetInfo().tLPoint.fY;
		float x2 = pLine5->GetInfo().tRPoint.fX;
		float y2 = pLine5->GetInfo().tRPoint.fY;


		float fGradient = float(y2 - y1) / (x2 - x1);
		*pY = fGradient * (fX - x1) + y1;

		bCheck = true;
	}



	return bCheck;	
}

void CLineMgr::LoadData()
{
	LINEINFO tTemp;
	DWORD dwByte = 0;


	HANDLE hFile = CreateFile(__T("../Data/Line_Field.dat"), GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"../Data/Line_Field.dat", L"Load Failed!!!", MB_OK);
		return;
	}

	while(true)
	{
		ReadFile(hFile, &tTemp, sizeof(LINEINFO), &dwByte, NULL);

		if(0 == dwByte)
			break;

		m_LineList[LINE_FIELD].push_back(new CLine(tTemp.tLPoint, tTemp.tRPoint));
	}


	hFile = CreateFile(__T("../Data/Line_Henesis.dat"), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"../Data/Line_Henesis.dat", L"Load Failed!!!", MB_OK);
		return;
	}

	while(true)
	{
		ReadFile(hFile, &tTemp, sizeof(LINEINFO), &dwByte, NULL);

		if(0 == dwByte)
			break;

		m_LineList[LINE_STAGE1].push_back(new CLine(tTemp.tLPoint, tTemp.tRPoint));
	}



	hFile = CreateFile(__T("../Data/Line_Kritias.dat"), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"../Data/Line_Kritias.dat", L"Load Failed!!!", MB_OK);
		return;
	}

	while(true)
	{
		ReadFile(hFile, &tTemp, sizeof(LINEINFO), &dwByte, NULL);

		if(0 == dwByte)
			break;

		m_LineList[LINE_STAGE2].push_back(new CLine(tTemp.tLPoint, tTemp.tRPoint));
	}


	hFile = CreateFile(__T("../Data/Line_Boss.dat"), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"../Data/Line_Boss.dat", L"Load Failed!!!", MB_OK);
		return;
	}

	while(true)
	{
		ReadFile(hFile, &tTemp, sizeof(LINEINFO), &dwByte, NULL);

		if(0 == dwByte)
			break;

		m_LineList[LINE_BOSS].push_back(new CLine(tTemp.tLPoint, tTemp.tRPoint));
	}




	CloseHandle(hFile);
}
