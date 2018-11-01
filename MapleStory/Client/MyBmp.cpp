#include "StdAfx.h"
#include "MyBmp.h"

CMyBmp::CMyBmp(void)
{
}

CMyBmp::~CMyBmp(void)
{
}

CMyBmp* CMyBmp::LoadBmp(const TCHAR* pFileName)
{
	m_hDc = GetDC(g_hWnd);

	// �޸� DC (���α׷� ���ο� ��Ʈ���� �̸� �׸����� �� ��)
	//CreateCompatibleDC �Լ��� ���� ��� DC�� ���� ������ ���� �޸� DC ����
	// ���� �� �޸� DC�� �̸� �׷��� ���ҽ��� ��� DC�� ��� �����ؼ� ȭ�鿡 ���
	m_hMemDc = CreateCompatibleDC(m_hDc);

	// �Լ� ȣ�� ������ �̹����� �ڵ� ����
	m_bitmap = (HBITMAP)LoadImage(NULL, pFileName, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	// ���н� NULL ����
	if(NULL == m_bitmap)
	{
		MessageBox(g_hWnd, pFileName, L"Image Load Failed (LoadImage)!!", MB_OK);
		return NULL;
	}

	// �޸� DC�� �̸� �׸����� �ϴ� GDI ������Ʈ�� �����ϴ� �Լ�.
	// ���� ���� ���õǱ� ������ GDI ������Ʈ
	m_oldBmp = (HBITMAP)SelectObject(m_hMemDc, m_bitmap);

	ReleaseDC(g_hWnd, m_hDc);

	return this;

}

void CMyBmp::Release()
{
	SelectObject(m_hMemDc, m_oldBmp);
	DeleteObject(m_bitmap);
	DeleteDC(m_hMemDc);
}
