#include "StdAfx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr(void)
{
	// Ű ���� �ʱ�ȭ.
	ZeroMemory(m_bKeyDown, 256);
	ZeroMemory(m_bKeyUp, 256);
}

CKeyMgr::~CKeyMgr(void)
{
}

// ����ؼ� �������� üũ
bool CKeyMgr::StayKeyDown( INT nKey )
{
	if(GetAsyncKeyState(nKey) & 0x8000)
		return true;

	return false;
}

// �ѹ��� �������� üũ
bool CKeyMgr::OnceKeyDown( INT nKey )
{
	if(GetAsyncKeyState(nKey) & 0x8000)
	{
		// Ű�� ������ ���� true
		if(m_bKeyDown[nKey] == false)
		{
			m_bKeyDown[nKey] = true;
			return true;
		}
	}
	else
	{
		// Ű�� ���� ���� false
		m_bKeyDown[nKey] = false;
	}
	return false;
}

// �ѹ��� �����ٰ� ������ üũ
bool CKeyMgr::OnceKeyUp( INT nKey )
{
	if(GetAsyncKeyState(nKey) & 0x8000)
		m_bKeyUp[nKey] = true;
	else
	{
		// ������ ���� true�� �ǹǷ� true�϶��� �����Ѵ�.
		if(m_bKeyUp[nKey] == true)
		{
			// Ű ���¸� false�� ����� �������� ���´�.
			m_bKeyUp[nKey] = false;

			return true;
		}
	}

	return false;
}

bool CKeyMgr::IsToggleKey( INT nKey )
{
	//GetKeyState�� 0x0001�� ������ ���ȴ����� üũ�Ѵ�.
	if(GetKeyState(nKey) & 0x0001)
		return true;

	return false;
}
