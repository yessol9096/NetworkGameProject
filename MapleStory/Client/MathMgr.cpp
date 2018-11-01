#include "StdAfx.h"
#include "MathMgr.h"
#include "Obj.h"

CMathMgr::CMathMgr(void)
{
}

CMathMgr::~CMathMgr(void)
{
}

float CMathMgr::CalcDistance( CObj* pDest,  CObj* pSour)
{
	float fWidth = pDest->GetInfo().fX - pSour->GetInfo().fX;
	float fHeight = pDest->GetInfo().fY - pSour->GetInfo().fY;

	return sqrtf(fWidth * fWidth + fHeight * fHeight);
}

float CMathMgr::CalcAngleToRadian( CObj* pDest,  CObj* pSour)
{
	// ������ pDest�� ������ pSour�� ���� ���� �ؼ� ���.
	float fWidth = pDest->GetInfo().fX - pSour->GetInfo().fX;
	float fHeight = pDest->GetInfo().fY - pSour->GetInfo().fY;

	float fDist = sqrtf(fWidth * fWidth + fHeight * fHeight);

	float fAngle = acosf(fWidth / fDist);

	if(pDest->GetInfo().fY > pSour->GetInfo().fY)
		fAngle *= -1.f;

	return fAngle;	
}

float CMathMgr::CalcAngleToDegree( CObj* pDest,  CObj* pSour)
{
	// ������ pDest�� ������ pSour�� ���� ���� �ؼ� ���.
	float fWidth = pDest->GetInfo().fX - pSour->GetInfo().fX;
	float fHeight = pDest->GetInfo().fY - pSour->GetInfo().fY;

	float fDist = sqrtf(fWidth * fWidth + fHeight * fHeight);

	float fAngle = acosf(fWidth / fDist);

	if(pDest->GetInfo().fY > pSour->GetInfo().fY)
		fAngle *= -1.f;

	return fAngle * 180.f / PI;	
}