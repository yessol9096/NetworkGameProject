#pragma once

class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)
public:
	enum CHANNEL_ID { CHANNEL_PLAYER, CHANNEL_MONSTER,  CHANNEL_SKILL, CHANNEL_EFFECT, CHANNEL_BGM, CHANNEL_END };

private:
	CSoundMgr(void);
	~CSoundMgr(void);

public:
	void Initialize();
	void LoadSoundFile();
	void PlaySound(const TCHAR* pSoundKey, CHANNEL_ID eChannel);
	void PlayBGM(const TCHAR* pSoundKey);
	void UpdateSound();
	void StopSound(CHANNEL_ID eChannel);
	void StopSoundAll();
	void Release();

private:
	FMOD_SYSTEM*	m_pSystem; // ���带 ������ �ý��� ������.
	FMOD_CHANNEL*	m_pChannelArr[CHANNEL_END];	// ���带 ����� ä���� ����.

	map<TCHAR*, FMOD_SOUND*> m_MapSound;
};
