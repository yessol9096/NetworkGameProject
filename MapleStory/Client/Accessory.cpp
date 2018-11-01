#include "StdAfx.h"
#include "Accessory.h"

CAccessory::CAccessory(void)
{
}

CAccessory::~CAccessory(void)
{
}

void CAccessory::Initialize(void)
{
	m_tInfo.fCX = 32.f;
	m_tInfo.fCY = 32.f;

	m_tState.iAtt = 20000;
	m_tState.pName = L"Accessory";

	m_pImgName = L"Accessory";

	m_fOriginalY = m_tInfo.fY;

	m_fJumpAcc = 0.f;
	m_fJumpSpeed = 13.f;

	m_fAngle=  90.f;

	m_bIsJump = true;
	m_bIsJumpUp = true;

	// 이미지
	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.dwFrameSpd = 200;

	// 렌더 타입
	m_eRenderType = RENDER_OBJ;

}

int CAccessory::Update(void)
{
	if(m_bIsDead) return 1;

	CObj::UpdateRect();

	// 점프
	Move();

	// 충돌박스 세팅
	UpdateCollRect();
	// 프레임 돌리기
	FrameMove();

	return 0;
}

void CAccessory::Render(HDC hDc)
{
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;


	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left + g_fScrollX),
		static_cast<int>(m_tRect.top + g_fScrollY), 
		static_cast<int>(m_tInfo.fCX),
		static_cast<int>(m_tInfo.fCY),
		pBit->GetMemDC(),
		0,
		0,
		static_cast<int>(m_tInfo.fCX),
		static_cast<int>(m_tInfo.fCY),
		RGB(0, 255, 0));

}

void CAccessory::Release(void)
{

}

void CAccessory::FrameMove()
{

}

void CAccessory::Move()
{
	if(m_bIsJump)
	{
		m_fJumpAcc += 0.25f;

		float fY = m_fJumpSpeed * m_fJumpAcc * sinf(m_fAngle * PI / 180.f)
			- GRAVITY * m_fJumpAcc * m_fJumpAcc * 0.5f;

		if(fY < 0)
		{
			m_bIsJumpUp = false;
		}
		else
			m_bIsJumpUp = true;

		m_tInfo.fY -= fY;

		if(m_tInfo.fY > m_fOriginalY + 10.f && !m_bIsJumpUp)
		{
			m_fJumpAcc = 0.f;
			m_bIsJump = 0.f;
		}
	}
}

void CAccessory::UpdateCollRect()
{
	m_tCollRect = m_tRect;
}
