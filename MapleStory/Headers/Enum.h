#pragma once

// �ֿ� ������Ʈ ����. (�÷��̾�, ��, ���� ..)
enum OBJECT_TYPE{
	OBJ_FIRSTFLOOR, OBJ_SECONDFLOOR, OBJ_THIRDFLOOR, OBJ_FOURTHFLOOR, OBJ_FIFTHFLOOR, 
	OBJ_PLAYER,
	OBJ_NPC,
	OBJ_ROPE,
	OBJ_ARROW, OBJ_SKILL_WING, OBJ_SKILL_SHOOT, OBJ_SKILL_SWING, OBJ_SKILL_DRAGON, OBJ_SKILL_FIRE,
	OBJ_MONSTER, OBJ_GRRENMUSH, OBJ_COUPLEMUSH,
	OBJ_ITEM,
	OBJ_EFFECT,
	OBJ_UI, OBJ_HPBAR,
	OBJ_MOUSE,
	OBJ_MYFONT,
	OBJ_END
};

enum OBJECT_DIR {
	DIR_LEFT, DIR_RIGHT, DIR_ROPE, DIR_END
};

enum SCENE_TYPE {
	SCENE_LOGO, SCENE_MENU, SCENE_EDIT,
	SCENE_MAKINGPLAYER,
	SCENE_FIELD, SCENE_STAGE1, 
	SCENE_END
};

enum PLAYER_STATE {
	PLAYER_STAND, PLAYER_WALK, PLAYER_JUMP, PLAYER_ROPE, 
	PLAYER_ATTACK, PLAYER_SWING,
	PLAYER_CRAWLATTACK, PLAYER_CRAWL,
	PLAYER_SHOOT, PLAYER_DAMAGED
};

enum MONSTER_STATE {
	MONSTER_STAND, MONSTER_WALK,/* MONSTER_JUMP, MONSTER_FLY,*/
	MONSTER_ATTACK1, MONSTER_ATTACK2, MONSTER_SKILL1, MONSTER_SKILL2, MONSTER_SKILL3, MONSTER_SKILL4, MONSTER_SKILL5,
	MONSTER_DAMAGED, MONSTER_DEAD
};

enum MOUSE_STATE { MOUSE_BASIC, MOUSE_GRAB };



enum GOLD_ID { GOLD_0, GOLD_1, GOLD_2, GOLD_3 };

enum ARROW_ID { ARROW_BASIC, ARROW_DRAGON };

// �� �� ������Ʈ. (����, �浹, ��Ʈ ��..)

enum RENDER_TYPE { RENDER_BACKGROUND, RENDER_OBJ, RENDER_EFFECT, RENDER_UI, RENDER_END };

enum LINE_ID { LINE_FIELD, LINE_STAGE1, LINE_STAGE2, LINE_BOSS, LINE_END };

enum COLLISION_ID {
	COL_PLAYER_ROPE,
	COL_PLAYER_FIRSTFLOOR, COL_PLAYER_SECONDFLOOR, COL_PLAYER_THIRDFLOOR, COL_PLAYER_FOURTHFLOOR, COL_PLAYER_FIFTHFLOOR,
	COL_MONSTER_FIRSTFLOOR, COL_MONSTER_SECONDFLOOR, COL_MONSTER_THIRDFLOOR, COL_MONSTER_FOURTHFLOOR, COL_MONSTER_FIFTHFLOOR,
	COL_MONSTER_SKILL, COL_MONSTER_ARROW, COL_MONSTER_SWING, COL_MONSTER_EFFECT,

	COL_PLAYER_MONSTER,
	COL_PLAYER_BOSS,

	COL_PLAYER_ITEM,
	COL_END
};

enum FLOORBOX_ID { FLOORBOX_HEIGHT, FLOORBOX_WIDTH, FLOORBOX_END };


enum FONT_ID { FONTID_DAMAGED, FONTID_ATTACK, FONTID_CRITICAL };

// Player
enum PLAYER_JOB { JOB_STRIKER, JOB_CAPTIN, JOB_END };

// Skill
enum SKILL_TYPE { SKILL_SHOOT, SKILL_FIRE, SKILL_END };

//--------------------------------------------------------------
// ���� --------------------------------------------------------
// �浹 üũ �����ؼ� ��Ʈ ��� ��.
enum FONT_TYPE { FONTTYPE_UNIT, FONTTYPE_TENS, FONTTYPE_HUND, FONTTYPE_THOU, FONTTYPE_TENTH };
enum FONT_FLOOR { FONT_FIRSTFLOOR, FONT_SECONDFLOOR, FONT_THIRDFLOOR, FONT_FOURTHFLOOR, FONT_FIFTHFLOOR };

enum PACKET_TYPE { // CS - Client to Server. SC - Server to Client.
	CS_PACKET_PLAYERINFO_INITIALLY,		// Ŭ���̾�Ʈ : ���� ���� �����߾�. ���� �г��Ӱ� ������ ���߰�, �̰� playerinfo�� ä���� ������. // ���� : �г��Ӱ� ����? �� �˾Ҿ�. �� ���̵� �ο��ϱ� ���� �۾��� �Ұ�.
	CS_PACKET_PLAYERINFO_MOVE, // Ŭ���̾�Ʈ : �� ��������! �� playerinfo ���� �ٰ�. // ���� : g_vecplayer[�� id]�� �����Ұ�. �ٸ� �÷��̾�Ե� �� ������ �����Ұ�.
	CS_PACKET_PLAYERINFO_INCHANGINGSCENE,// Ŭ���̾�Ʈ : �� �� �ٲ۴�! �ٲ� �� ��ǥ ������ �����ٰ�. // ���� : g_vecplayer[�� id]�� �����Ұ�. �ٸ� �÷��̾����׵� �� ������ �����Ұ� �׷�.
	CS_PACKET_SKILL_CREATE,	// Ŭ���̾�Ʈ : �� ��ų ���! �� Skillinfo�� �����ٰ�. // ���� : �׷� �� ��ų�� id�� �ο��Ұ�. �׸��� g_vecSkill[id]�� �����Ұ�. �׸��� �ٸ� Ŭ���̾�Ʈ���Ե� �� ��ų ������ ������.
	CS_PACKET_SKILL_MOVE,

	CS_PACKET_GRRENMUSH,
	//----------------------------------------
	SC_PACKET_YOUR_PLAYERINFO, // ���� : �� ���� �ٲ����? or ���� ��������? �� playerinfo�� �̰ž�. // �� �˾Ҿ�! �� g_vecplayer[�� ���̵�]�� �����Ұ�.
	SC_PACKET_OTHER_PLAYERINFO, 	// ���� : �ٸ� Ŭ���̾�Ʈ�� ���°� �ٲ����. �� ������ info�� �̷�. // Ŭ���̾�Ʈ : �˾Ҿ�. ���� g_vecplayer[���� �ٲ� Ŭ���� id]�� �����Ұ�.
	SC_PACKET_NEW_OTHER_PLAYERINFO, // ����:  ���ο� �ٸ� Ŭ���̾�Ʈ�� ���Ծ�. �� ������ info�� �̷�. // Ŭ���̾�Ʈ : �˾Ҿ�. CreateObj�� �ؼ� ���� ������ �ְ�, ���� g_vecplayer[���� �ٲ� Ŭ���� id]�� �����Ұ�.
	SC_PACKET_SKILL_CREATE, // ���� : � Ŭ���̾�Ʈ�� �̷� ��ų�� �����߾�. ������ ������. // Ŭ���̾�Ʈ : �׷��� �� ��ų�� ���� �����.
	SC_PACKET_SKILL_MOVE,

	SC_PACKET_CLIENT_END,	// Ŭ���̾�Ʈ ���� �������� �˸���
	SC_PACKET_GRRENMUSH
};