#pragma once
class CSoundManager
{
public:
	DECLARE_SINGLETON(CSoundManager)

public:
	enum CHANNELID { BGM, PLAYER, MONSTER, BUTTON, GUN_DEFAULT, MOUSE, STRUCTURE, EFFECT, UI, MAXCHANNEL };

private:
	CSoundManager();
	~CSoundManager();

public:
	bool Ready();
	void Release();

public:
	void PlaySound(TCHAR* _pSoundKey, CHANNELID _eID);
	void PlayBGM(TCHAR* _pSoundKey);
	void StopSound(CHANNELID _eID);
	void StopAll(void);

private:
	bool LoadSoundFile(void);

private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;
};

