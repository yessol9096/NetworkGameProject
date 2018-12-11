#include "StdAfx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "Player.h"
#include "FloorBox.h"
#include "Monster.h"
#include "Arrow.h"
#include "Font.h"

static unsigned int state[16];
static unsigned int index = 0;
static bool			m_bIsFontCreated = false;

CollisionMgr::CollisionMgr(void)
{
}

CollisionMgr::~CollisionMgr(void)
{
}

void CollisionMgr::CollisionRect(OBJLIST& DstList, OBJLIST& SrcList, COLLISION_ID eID)
{
	// �� ����Ʈ�� ��ȸ�Ѵ�.
	OBJITER dst_begin = DstList.begin();
	OBJITER dst_end = DstList.end();

	RECT rcTemp = {};

	if (true == g_bIsSceneChange)
		return;

	for (; dst_begin != dst_end; ++dst_begin)
	{
		OBJITER src_begin = SrcList.begin();
		OBJITER src_end = SrcList.end();

		if ((*dst_begin)->GetDeadState())
			continue;

		for (; src_begin != src_end; ++src_begin)
		{
			if ((*src_begin)->GetDeadState())
				continue;

			/////////////////////////////////////////////////////////////////////////////////////
			// 1) ������Ʈ �� �� ��Ʈ�ڽ��� �浹�ϴ� ���
			if (IntersectRect(&rcTemp, &((*dst_begin)->GetCollRect()), &((*src_begin)->GetCollRect())))
			{
				switch (eID)
				{
				case COL_PLAYER_MONSTER:
					// ���� ������ ��, ������ Ÿ�� ���� ���� ���� ó���Ѵ�
					if (!dynamic_cast<CPlayer*>(*dst_begin)->GetIsInvincible() &&
						PLAYER_ROPE != dynamic_cast<CPlayer*>(*dst_begin)->GetPlayerState())
					{
						// ���� ������ �������� �÷��̾ �о��
						if (DIR_RIGHT == (*src_begin)->GetDir())
						{
							dynamic_cast<CPlayer*>(*dst_begin)->SetIsJump(true);
							dynamic_cast<CPlayer*>(*dst_begin)->SetAngle(45.f);
						}
						else
						{
							dynamic_cast<CPlayer*>(*dst_begin)->SetIsJump(true);
							dynamic_cast<CPlayer*>(*dst_begin)->SetAngle(45.f);
						}
						dynamic_cast<CPlayer*>(*dst_begin)->SetPlayerState(PLAYER_DAMAGED);


						///////////// ** ������ ��Ʈ
						{
							int iCipher = CalculatingCipher((*src_begin)->GetState().iAtt);
							int iDamage = ReturningRandomNumber((*src_begin)->GetState().iAtt);
							FONT_TYPE eFontType = ReturningFontType(iCipher);


							if (!dynamic_cast<CPlayer*>(*dst_begin)->GetIsInvincible())
							{
								CObjMgr::GetInstance()->AddObject(CreateFont<CFont>((*dst_begin),
									iDamage,
									FONTID_DAMAGED, eFontType, FONT_FIRSTFLOOR),
									OBJ_MYFONT);

								dynamic_cast<CPlayer*>(*dst_begin)->SetDamage(iDamage);
							}
						}

						// ������ ��Ʈ ���� ���� ���� ���� ����
						dynamic_cast<CPlayer*>(*dst_begin)->SetIsInvincible(true);
					}
					break;

					// ** 0717
				case COL_MONSTER_ARROW:
					if (MONSTER_DEAD != dynamic_cast<CMonster*>(*dst_begin)->GetMonsterState() &&
						(*src_begin)->GetCollMode())
					{
						dynamic_cast<CMonster*>(*dst_begin)->SetMonsterState(MONSTER_DAMAGED);
						dynamic_cast<CMonster*>(*dst_begin)->SetDamage((*src_begin)->GetState().iAtt);
						CSoundMgr::GetInstance()->PlaySound(L"MonsterDamaged.MP3", CSoundMgr::CHANNEL_MONSTER);
						(*src_begin)->SetCollMode(false); // �ѹ� �ε������� �׸� �ε����� �浹 off�� ����

						if (DIR_RIGHT == (*src_begin)->GetDir())
							(*dst_begin)->SetDir(DIR_LEFT);
						else
							(*dst_begin)->SetDir(DIR_RIGHT);

						(*src_begin)->SetDeadState(true);

						///////////// ** ������ ��Ʈ
						{
							int iCipher = CalculatingCipher((*src_begin)->GetState().iAtt);
							int iDamage = ReturningRandomNumber((*src_begin)->GetState().iAtt);
							int iOrder = dynamic_cast<CArrow*>(*src_begin)->GetOrder();

							FONT_TYPE eFontType = ReturningFontType(iCipher);
							FONT_FLOOR eFloor = FONT_FIRSTFLOOR;

							switch (iOrder)
							{
							case 0:
								eFloor = FONT_FIRSTFLOOR;
								break;
							case 1:
								eFloor = FONT_SECONDFLOOR;
								break;
							case 2:
								eFloor = FONT_THIRDFLOOR;
								break;
							case 3:
								eFloor = FONT_FOURTHFLOOR;
								break;
							case 4:
								eFloor = FONT_FIFTHFLOOR;
								break;
							}

							CObjMgr::GetInstance()->AddObject(CreateFont<CFont>((*dst_begin),
								iDamage, FONTID_ATTACK, eFontType, eFloor),
								OBJ_MYFONT);
						}
					}
					break;

				case COL_MONSTER_SWING:
					if (MONSTER_DAMAGED != dynamic_cast<CMonster*>(*dst_begin)->GetMonsterState()
						&& MONSTER_DEAD != dynamic_cast<CMonster*>(*dst_begin)->GetMonsterState())
					{
						dynamic_cast<CMonster*>(*dst_begin)->SetMonsterState(MONSTER_DAMAGED);
						dynamic_cast<CMonster*>(*dst_begin)->SetDamage((*src_begin)->GetState().iAtt);

						if (DIR_RIGHT == (*src_begin)->GetDir())
							(*dst_begin)->SetDir(DIR_LEFT);
						else
							(*dst_begin)->SetDir(DIR_RIGHT);

						CSoundMgr::GetInstance()->PlaySound(L"MonsterDamaged.MP3", CSoundMgr::CHANNEL_MONSTER);

						///////////// ** ������ ��Ʈ
						{
							int iCipher = CalculatingCipher((*src_begin)->GetState().iAtt);
							int iDamage = ReturningRandomNumber((*src_begin)->GetState().iAtt);
							FONT_TYPE eFontType = ReturningFontType(iCipher);

							CObjMgr::GetInstance()->AddObject(CreateFont<CFont>((*dst_begin),
								iDamage,
								FONTID_ATTACK, eFontType, FONT_FIRSTFLOOR),
								OBJ_MYFONT);
						}
					}
					break;

				case COL_MONSTER_EFFECT:
					if (MONSTER_DAMAGED != dynamic_cast<CMonster*>(*dst_begin)->GetMonsterState()
						&& MONSTER_DEAD != dynamic_cast<CMonster*>(*dst_begin)->GetMonsterState())
					{
						dynamic_cast<CMonster*>(*dst_begin)->SetMonsterState(MONSTER_DAMAGED);
						dynamic_cast<CMonster*>(*dst_begin)->SetDamage((*src_begin)->GetState().iAtt);

						if (DIR_RIGHT == (*src_begin)->GetDir())
							(*dst_begin)->SetDir(DIR_LEFT);
						else
							(*dst_begin)->SetDir(DIR_RIGHT);
						CSoundMgr::GetInstance()->PlaySound(L"MonsterDamaged.MP3", CSoundMgr::CHANNEL_MONSTER);

						///////////// ** ������ ��Ʈ
						{
							int iCipher = CalculatingCipher((*src_begin)->GetState().iAtt);
							int iDamage = ReturningRandomNumber((*src_begin)->GetState().iAtt);
							FONT_TYPE eFontType = ReturningFontType(iCipher);

							CObjMgr::GetInstance()->AddObject(CreateFont<CFont>((*dst_begin),
								iDamage,
								FONTID_ATTACK, eFontType, FONT_FIRSTFLOOR),
								OBJ_MYFONT);
						}
					}
					break;
				case COL_PLAYER_ITEM:
				{
					dynamic_cast<CPlayer*>(*dst_begin)->SetGold((*src_begin)->GetState().iGold);
					CSoundMgr::GetInstance()->PlaySound(L"ItemTake.mp3", CSoundMgr::CHANNEL_EFFECT);
					(*src_begin)->SetDeadState(true);
				}
				break;
				}
			}

			/////////////////////////////////////////////////////////////////////////////////////
			// 2) ������Ʈ(dst)�� ��Ʈ�ڽ��� �浹�ϴ� ��� (�ַ� ������Ʈ - ����)
			if (IntersectRect(&rcTemp, &((*dst_begin)->GetCollRect()), &((*src_begin)->GetRect())))
			{
				// ���� ���� �� ���⿡ ����..��
				OBJECT_DIR eDir = DIR_END;
				//

				switch (eID)
				{
					///////////////////////////////////////////////// �÷��̾� - ���� /////////////////////////////////////////////////
				case COL_PLAYER_ROPE:
					dynamic_cast<CPlayer*>(*dst_begin)->SetIsRopeColl(true);
					return;
				case COL_PLAYER_FIRSTFLOOR:
					dynamic_cast<CPlayer*>(*dst_begin)->SetPlayerFloor(1);
					break;
				case COL_PLAYER_SECONDFLOOR:
					dynamic_cast<CPlayer*>(*dst_begin)->SetPlayerFloor(2);
					break;
				case COL_PLAYER_THIRDFLOOR:
					dynamic_cast<CPlayer*>(*dst_begin)->SetPlayerFloor(3);
					break;
				case COL_PLAYER_FOURTHFLOOR:
					dynamic_cast<CPlayer*>(*dst_begin)->SetPlayerFloor(4);
					break;
				case COL_PLAYER_FIFTHFLOOR:
					dynamic_cast<CPlayer*>(*dst_begin)->SetPlayerFloor(5);
					break;

					///////////////////////////////////////////////// ���� - ���� /////////////////////////////////////////////////
				case COL_MONSTER_FIRSTFLOOR:
					if (FLOORBOX_HEIGHT == dynamic_cast<CFloorBox*>(*src_begin)->GetFloorBoxID()
						&& false == dynamic_cast<CMonster*>(*dst_begin)->GetIsFloorBoxColl())
					{
						if (DIR_RIGHT == (*dst_begin)->GetDir())
						{
							(*dst_begin)->SetPos((*src_begin)->GetRect().left - (*dst_begin)->GetInfo().size.cx / 2.f, (*dst_begin)->GetInfo().pt.y);
							eDir = DIR_LEFT;
						}
						else
						{
							(*dst_begin)->SetPos((*src_begin)->GetRect().right + (*dst_begin)->GetInfo().size.cx / 2.f, (*dst_begin)->GetInfo().pt.y);
							eDir = DIR_RIGHT;
						}

						(*dst_begin)->SetDir(eDir);
						dynamic_cast<CMonster*>(*dst_begin)->SetIsFloorBoxColl(true);

					}
					break;
				case COL_MONSTER_SECONDFLOOR:
					if (FLOORBOX_HEIGHT == dynamic_cast<CFloorBox*>(*src_begin)->GetFloorBoxID()
						&& false == dynamic_cast<CMonster*>(*dst_begin)->GetIsFloorBoxColl())
					{
						if (DIR_RIGHT == (*dst_begin)->GetDir())
						{
							eDir = DIR_LEFT;
							(*dst_begin)->SetDir(eDir);
							(*dst_begin)->SetPos((*src_begin)->GetRect().left - (*dst_begin)->GetInfo().size.cx / 2.f, (*dst_begin)->GetInfo().pt.y);

						}
						else
						{
							eDir = DIR_RIGHT;
							(*dst_begin)->SetDir(eDir);
							(*dst_begin)->SetPos((*src_begin)->GetRect().right + (*dst_begin)->GetInfo().size.cx / 2.f, (*dst_begin)->GetInfo().pt.y);

						}

						dynamic_cast<CMonster*>(*dst_begin)->SetIsFloorBoxColl(true);

					}
					break;
				case COL_MONSTER_THIRDFLOOR:
					if (FLOORBOX_HEIGHT == dynamic_cast<CFloorBox*>(*src_begin)->GetFloorBoxID()
						&& false == dynamic_cast<CMonster*>(*dst_begin)->GetIsFloorBoxColl())
					{
						if (DIR_RIGHT == (*dst_begin)->GetDir())
						{
							eDir = DIR_LEFT;
							(*dst_begin)->SetDir(eDir);
							(*dst_begin)->SetPos((*src_begin)->GetRect().left - (*dst_begin)->GetInfo().size.cx / 2.f, (*dst_begin)->GetInfo().pt.y);
						}
						else
						{
							eDir = DIR_RIGHT;
							(*dst_begin)->SetDir(eDir);
							(*dst_begin)->SetPos((*src_begin)->GetRect().right + (*dst_begin)->GetInfo().size.cx / 2.f, (*dst_begin)->GetInfo().pt.y);
						}


						dynamic_cast<CMonster*>(*dst_begin)->SetIsFloorBoxColl(true);

					}
					break;
				case COL_MONSTER_FOURTHFLOOR:
					if (FLOORBOX_HEIGHT == dynamic_cast<CFloorBox*>(*src_begin)->GetFloorBoxID()
						&& false == dynamic_cast<CMonster*>(*dst_begin)->GetIsFloorBoxColl())
					{
						if (DIR_RIGHT == (*dst_begin)->GetDir())
						{
							eDir = DIR_LEFT;
							(*dst_begin)->SetPos((*src_begin)->GetRect().left - (*dst_begin)->GetInfo().size.cx / 2.f, (*dst_begin)->GetInfo().pt.y);
						}
						else
						{
							eDir = DIR_RIGHT;
							(*dst_begin)->SetPos((*src_begin)->GetRect().right + (*dst_begin)->GetInfo().size.cx / 2.f, (*dst_begin)->GetInfo().pt.y);
						}

						(*dst_begin)->SetDir(eDir);
						dynamic_cast<CMonster*>(*dst_begin)->SetIsFloorBoxColl(true);

					}
					break;
				case COL_MONSTER_FIFTHFLOOR:
					if (FLOORBOX_HEIGHT == dynamic_cast<CFloorBox*>(*src_begin)->GetFloorBoxID()
						&& false == dynamic_cast<CMonster*>(*dst_begin)->GetIsFloorBoxColl())
					{
						if (DIR_RIGHT == (*dst_begin)->GetDir())
						{
							(*dst_begin)->SetPos((*src_begin)->GetRect().left - (*dst_begin)->GetInfo().size.cx / 2.f, (*dst_begin)->GetInfo().pt.y);
							eDir = DIR_LEFT;
						}
						else
						{
							(*dst_begin)->SetPos((*src_begin)->GetRect().right + (*dst_begin)->GetInfo().size.cx / 2.f, (*dst_begin)->GetInfo().pt.y);
							eDir = DIR_RIGHT;
						}

						(*dst_begin)->SetDir(eDir);
						dynamic_cast<CMonster*>(*dst_begin)->SetIsFloorBoxColl(true);

					}
					break;
				}
			}
		}
	}
}

int CollisionMgr::CalculatingCipher(int iNumber)
{
	int iCipher = 1;

	for(iCipher; iNumber >= 10;)
	{
		iNumber = iNumber / 10;
		iCipher++;
	}

	return iCipher;
}

int CollisionMgr::ReturningRandomNumber(int iNumber)
{
	// ** 30000~ 39999 ������ ���� ����.

	// �ڸ��� ���Ѵ�����
	int iCipher = CalculatingCipher(iNumber);

	// �ְ� �ڸ��� (���� 31231�̸� 30000�� ���� ��)
	int iHigherNumber = static_cast<int>((iNumber / (pow(10.0, iCipher)) * pow(10.0, iCipher)));

	// �� ���� �ְ� �ڸ��� (���� 31231�̸� 1000�� ���� ��)
//	int iSecondHigherNumber = (iNumber - iHigherNumber) / pow(10.0, (iCipher-1)) * pow(10.0, (iCipher - 1));

	// �ڸ����� ���� (������ ����) // �ִ� 5�ڸ��� ��
	int iSecondCipher = iCipher - 1;
	int iThirdCipher = iCipher - 2;
	int iFourthCipher = iCipher - 3;
	int iFifthCipher = iCipher - 4;

	// �տ��� �ι�°, ����°, �׹�°.. �ڸ����� ������ ������ ����
	// iNumber�� 5�ڸ����� secondcipher�� 4, 1�ڸ����� secondcipher�� 0�̹Ƿ� �̶� if�� ��Ž.
	int iRandom = 0;
	int iSecondRandom = 0;
	int iThirdRandom = 0;
	int iFourthRandom = 0;
	int iFifthRandom = 0;

	if (iSecondCipher >= 1)
	{
		iSecondRandom = (WELLRNG512() % 10) * (static_cast<int>(pow(10.0, iSecondCipher - 1)));

		if (iThirdCipher >= 1)
		{
			iThirdRandom = (WELLRNG512() % 10) * (static_cast<int>(pow(10.0, iThirdCipher - 1)));

			if (iFourthCipher >= 1)
			{
				iFourthRandom = (WELLRNG512() % 10) * (static_cast<int>(pow(10.0, iFourthCipher - 1)));

				if (iFifthCipher >= 1)
					iFifthRandom = (WELLRNG512() % 10) * (static_cast<int>(pow(10.0, iFifthCipher - 1)));
			}
		}
	}

	iRandom = iHigherNumber + iSecondRandom + iThirdRandom + iFourthRandom + iFifthRandom;

	return iRandom;
}

FONT_TYPE CollisionMgr::ReturningFontType(int iCipher)
{
	// �ϴ� ���ڸ����� �ʱ�ȭ.
	FONT_TYPE eFontType = FONTTYPE_UNIT;

	switch(iCipher)
	{
	case 1:
		eFontType = FONTTYPE_UNIT;
		break;
	case 2:
		eFontType = FONTTYPE_TENS;
		break;
	case 3:
		eFontType = FONTTYPE_HUND;
		break;
	case 4:
		eFontType = FONTTYPE_THOU;
		break;
	case 5:
		eFontType = FONTTYPE_TENTH;
		break;
	}

	return eFontType;
}

unsigned long CollisionMgr::WELLRNG512(void)
{
	unsigned long a, b, c, d; 
	a = state[index]; 
	c = state[(index+13)&15]; 
	b = a^c^(a<<16)^(c<<15); 
	c = state[(index+9)&15]; 
	c ^= (c>>11); 
	a = state[index] = b^c; 
	d = a^((a<<5)&0xDA442D20UL); 
	index = (index+15)&15; 
	a = state[index]; 
	state[index] = a^b^d^(a<<2)^(b<<18)^(c<<28); 
	return state[index]; 
}

void CollisionMgr::InitWELLRNG512(unsigned long seed)
{
	for (int i = 0; i < 16; ++i)
		state[i] = seed;
}