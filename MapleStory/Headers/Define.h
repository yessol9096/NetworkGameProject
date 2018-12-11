#pragma once

// ����, Ŭ���̾�Ʈ ����.
// �������� ����.
#define SERVERPORT 9000
#define BUFSIZE			4000
#define MAX_USER		2
#define MAX_GREEN		12

// #define DEBUG
// #define DEBUG_SCROLL
#define DEBUG_1206

// --------------------------------------------------------------------------
// Ŭ���̾�Ʈ ������ ���� ������.

// �ʿ��� ���� ����.
#define PI	3.1415f
#define GRAVITY 9.8f


// ������ X, Y ������.
#define WINCX 800
#define WINCY 600

// ���� �� X,Y ������.
#define BOSSMAPCX 1799
#define BOSSMAPCY 781

// Ÿ�� ����, ���� ����
#define TILEX 300
#define TILEY 50

// Ÿ�� ����, ���� ������
#define TILECX 30
#define TILECY 30

// �� ��������Ʈ ũ�� ����.
#define FIELDCX 1920
#define FIELDCY 681

#define HENESISCX 1773
#define HENESISCY 1464

#define BOSSMAPCX 1799
#define BOSSMAPCY 781


// ����Ʈ(��ų) �Ÿ� ����. (�÷��̾����)
#define SWINGEFFECTX 200.f
#define DRAGONX 50.f
#define FIREX 250.f
#define FIREY 50.f

// ���� HP, MP ����
#define HPPORTION 500
#define MPPORTION 500



// �̱��� ����.
// Ŭ������ heap�� �Ҵ��� ����, �Ҵ��� ���� �ּҸ�
// �ٱ����� �� �� �ְ� �Ϸ���! (static)
#define DECLARE_SINGLETON(ClassName)	\
private:								\
	static ClassName* m_pInstance;		\
public:									\
	static ClassName* GetInstance()		\
	{									\
	if(NULL == m_pInstance)			\
	m_pInstance = new ClassName;\
	return m_pInstance;			\
	}									\
	void DestroyInstance()				\
	{									\
	if(m_pInstance)					\
		{								\
		delete m_pInstance;			\
		m_pInstance = NULL;			\
		}								\
	}

#define IMPLEMENT_SINGLETON(ClassName)			\
	ClassName* ClassName::m_pInstance = NULL;