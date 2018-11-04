#pragma once

class CObj;
class CMonster;
class CInven;
class CPortal;
class CCommuication;
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
	static CObj* CreatePortal(float fX, float fY, int iWhereTogo)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(fX, fY);
		(dynamic_cast<CPortal*>(pObj))->SetWhereTogo(iWhereTogo);
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
		(dynamic_cast<CCommuication*>(pObj))->SetIsSucessed(bIsQuestSucessed);
		return pObj;
	}
	static CObj* CreateInventory(
		float fX, float fY, CObj* pPlayer)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		if(pPlayer != nullptr)
			(dynamic_cast<CInven*>(pObj))->SetPlayer(pPlayer);
		return pObj;
	}

	// ÆùÆ®¿ë
// 	static CObj* CreateObj(
// 		CObj* pTarget, int iAtt, FONT_ID eID, FONT_TYPE eType, FONT_FLOOR eFloor)
// 	{
// 		CObj* pObj = new T;
// 		dynamic_cast<Font*>(pObj)->SetNumber(iAtt);
// 		dynamic_cast<Font*>(pObj)->SetID(eID);
// 		dynamic_cast<Font*>(pObj)->SetType(eType);
// 		dynamic_cast<Font*>(pObj)->SetFloor(eFloor);
// 		pObj->Initialize();
//		pObj->SetPos(fX, fY);
//		dynamic_cast<CCommuication*>(pObj)->SetRenderNumber(iRenderNumber);
//		dynamic_cast<CCommuication*>(pObj)->SetIsSucessed(bIsQuestSucessed);
//		return pObj;
//	}

// 	static CObj* CreateGold(float fX, float fY, GOLD_ID eID)
// 	{
// 		CObj* pObj = new T;
// 		dynamic_cast<CGold*>(pObj)->SetGoldID(eID);
// 		pObj->Initialize();
// 		return pObj;
// 	}
// 	static CObj* CreateSkill(float fX, float fY, int iAtt)
// 	{
// 		CObj* pObj = new T;
// 		pObj->Initialize();
// 		pObj->SetAtt(iAtt);
// 	}

};