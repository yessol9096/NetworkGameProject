#pragma once

class CObj;
class CMonster;
class CCommuication;
class CParty;
class CFloorBox;

template <typename T>
class CAbstractFactory
{
public:
	static CObj* CreateObj()
	{
		CObj* pObj = new T;
		pObj->Initialize();
		return pObj;
	}
	static CObj* CreateObj(float fX, float fY)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(fX, fY);
		return pObj;
	}
	static CObj* CreateObj(float fX, float fY, float fAngle)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(fX, fY);
		pObj->SetAngle(fAngle);
		return pObj;
	}
	static CObj* CreateMonster(float fX, float fY, OBJECT_DIR eDir, int iPattern)
	{
		CObj* pObj = new T;
		pObj->SetDir(eDir);
		pObj->SetPos(fX, fY);
		(dynamic_cast<CMonster*>(pObj))->SetPattern(iPattern);
		pObj->Initialize();
		return pObj;
	}
	static CObj* CreateObj(float fX, float fY, float cx, float cy)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(fX, fY);
		pObj->SetSize(cx, cy);
		return pObj;
	}
	static CObj* CreateObj(TCHAR* pName, float fX, float fY, float cx, float cy)
	{
		CObj* pObj = new T(pName, fX, fY, cx, cy);
		pObj->Initialize();
		return pObj;
	}
	static CObj* CreateFloorBox(float fX, float fY, float cx, float cy, FLOORBOX_ID eID)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(fX, fY);
		pObj->SetSize(cx, cy);
		(dynamic_cast<CFloorBox*>(pObj))->SetFloorBoxID(eID);
		return pObj;
	}
	static CObj* CreateCommunication(
		float fX, float fY, int iRenderNumber, bool bIsQuestSucessed)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(fX, fY);
		(dynamic_cast<CCommuication*>(pObj))->SetRenderNumber(iRenderNumber);
		return pObj;
	}
};