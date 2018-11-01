#include "StdAfx.h"
#include "HPPortion.h"

CHPPortion::CHPPortion(void)
{
}

CHPPortion::~CHPPortion(void)
{
}

void CHPPortion::Initialize(void)
{
	m_tInfo.fCX = 32.f;
	m_tInfo.fCY = 32.f;

	m_tState.iHp = 1000;
	m_tState.pName = L"HPPortion";

	m_pImgName = L"HPPortion";

	m_fOriginalY = m_tInfo.fY;

	m_fJumpAcc = 0.f;
	m_fJumpSpeed = 13.f;

	m_bIsJump = true;
	m_bIsJumpUp = true;

	m_bIsInStore = false;


	// 이미지
	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.dwFrameSpd = 200;

	// 렌더 타입
	m_eRenderType = RENDER_OBJ;


	m_bIsUsed = false;
}

int CHPPortion::Update(void)
{

	if(m_bIsUsed) 
	{
		// 사운드
		m_bIsDead = true;
	}

	if(m_bIsDead)
		return 1;


	CObj::UpdateRect();

	// 점프
	if(m_bIsInInven)
		Move();

	// 충돌박스 세팅
	UpdateCollRect();
	// 프레임 돌리기
	FrameMove();
	

	return 0;
}

void CHPPortion::Render(HDC hDc)
{
	CObj::UpdateRect();
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;


	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left),
		static_cast<int>(m_tRect.top), 
		static_cast<int>(m_tInfo.fCX),
		static_cast<int>(m_tInfo.fCY),
		pBit->GetMemDC(),
		static_cast<int>(m_tInfo.fCX) * m_tFrame.iFrameStart,
		0,
		static_cast<int>(m_tInfo.fCX),
		static_cast<int>(m_tInfo.fCY),
		RGB(0, 255, 0));
}

void CHPPortion::FrameMove()
{

}

void CHPPortion::Release(void)
{

}

void CHPPortion::Move()
{
	if(m_bIsJump)
	{
		m_fJumpAcc += 0.25f;

		float fY = m_fJumpSpeed * m_fJumpAcc 
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

void CHPPortion::UpdateCollRect()
{
	m_tCollRect = m_tRect;
}
