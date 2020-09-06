#pragma once

class CScene;
class CSceneManager
{
	// �̱��� ����
	DECLARE_SINGLETON(CSceneManager)

public:
	enum ID { SCENE_LOGO, SCENE_INTRO, SCENE_LOBBY, SCENE_MENU,
		SCENE_GAME, SCENE_GAME2, SCENE_GAME3, SCENE_GAME4, SCENE_END, SCENE_TEST, END };
public:
	void ChangeScene(ID _eSceneID);
	void Update(void);
	void LateUpdate(void);
	void Render(const HDC& _hdc);
	void Release(void);

public:
	void SetPlayerID(PLAYER::ID _eID) { m_eCurID = _eID; }
	PLAYER::ID GetPlayerID(void) const { return m_eCurID; }

	ID   GetCurSceneID(void) const { return m_eCurScene; }
	ID   GetNextSceneID(void) const { return m_eNextScene; }
	bool IsChangeScene(void) const { return m_bIsChangeScene; }
	void SetIsChangeScene(bool _bIsChange) { m_bIsChangeScene = _bIsChange; }

private:
	CSceneManager();
	~CSceneManager();

private:
	bool m_bIsChangeScene = false;

	CScene* m_pScene = nullptr;

	ID		m_eCurScene = END;
	ID		m_eNextScene = END;

	PLAYER::ID m_eCurID = PLAYER::JIMMY;
};

