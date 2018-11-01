#include "StdAfx.h"
#include "Skill.h"

CSkill::CSkill(void)
: m_pPlayer(NULL)
{
}

CSkill::~CSkill(void)
{
}

void CSkill::SetPlayer(CObj* pPlayer)
{
	m_pPlayer = pPlayer;

}
