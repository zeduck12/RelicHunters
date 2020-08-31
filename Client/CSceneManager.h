#pragma once

class CScene;
class CSceneManager
{
	// ½Ì±ÛÅæ ¼±¾ð
	DECLARE_SINGLETON(CSceneManager)

public:
	enum ID { SCENE_LOGO, SCENE_INTRO, SCENE_LOBBY, SCENE_MENU, SCENE_GAME, SCENE_END };
public:
	void ChangeScene(ID _eSceneID);
	void Update(void);
	void LateUpdate(void);
	void Render(const HDC& _hdc);
	void Release(void);

public:
	bool IsChangeScene(void) const { return m_bIsChangeScene; }
	void SetIsChangeScene(bool _bIsChange) { m_bIsChangeScene = _bIsChange; }

private:
	CSceneManager();
	~CSceneManager();

private:
	bool m_bIsChangeScene = false;

	CScene* m_pScene = nullptr;

	ID		m_eCurScene = SCENE_END;
	ID		m_eNextScene = SCENE_END;
};

