#pragma once
#include "obj.h"

class CItem;
class CInven :
	public CObj
{
public:
	CInven(void);
	virtual ~CInven(void);

public:
	vector<CObj*>& GetInven() { return m_vecInven; }

public:
	void SetPlayer(CObj* pPlayer) { m_pPlayer = pPlayer; }
	void SetInven(vector<CObj*>& rVec) { m_vecInven = rVec; }

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();

	void			MoveToMouse();
	void			ItemSetting();

private:
	bool			m_bIsOriginalpt;
	POINT			m_ptOriginal;
	float			m_fGapX;
	float			m_fGapY;

	// 인벤토리 칸
public:
	void SettingInvenPos();
private:
	POINT	m_ptSpacePos[24];
	RECT	m_tSpaceRect[24];
	
public:
	bool GainItem(CObj* pItem);
	bool DropItem(size_t Idx);
	void EquipItem(void);
	void UnEquipItem(void);
	void SettingEquip(void);
	STATE EquipStat(void);

private:
	vector<CObj*>	m_vecInven;
	size_t			m_iMaxInven;
	CObj*			m_pEquipment[ITEM_END];
	CObj*			m_pPlayer;

private:
	bool			m_bIsShowed; // i 버튼 누르면 render되게. on/off 기능

	// 버튼

public:
	void	MouseInButton();
	void	ButtonSetting();

private:
	RECT	m_tEquipButtonRect;
	RECT	m_tPortionButtonRect;
	RECT	m_tExitButtonRect;
	RECT	m_tMoveButtonRect;

private:
	bool	m_bIsEquipButtonOn;
	bool	m_bIsPortionButtonOn;
	bool	m_bIsExitButtonOn;
	bool	m_bIsMoveButtonOn;

	bool	m_bIsHpPortionOn;
	bool	m_bIsMpPortionOn;


public:
	template <typename T>
	CObj*	CreateItem(float fX, float fY)
	{
		CObj* pItem = CAbstractFactory<T>::CreateObj(fX, fY);
		(dynamic_cast<CItem*>(pItem))->SetIsInInven(false);
		return pItem;
	}


	// 아이템 설명
private:
	RECT	m_tRectHPPortionInfo;
	RECT	m_tRectMPPortionInfo;

	bool	m_bIsHPPortionInfoOn;
	bool	m_bIsMPPortionInfoOn;

	INFO	m_tHPPortionInfo;
	INFO	m_tMPPortionInfo;

};
