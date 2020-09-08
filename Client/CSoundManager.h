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
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;
};

