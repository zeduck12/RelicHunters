#pragma once

class CScene;
class CSceneManager
{
	// ½Ì±ÛÅæ ¼±¾ð
	DECLARE_SINGLETON(CSceneManager)

public:
	enum ID { SCENE_LOGO, SCENE_INTRO, SCENE_LOBBY, SCENE_GAME, SCENE_END };
public:
	void ChangeScene(ID _eSceneID);
	void Update(void);
	void LateUpdate(void);
	void Render(const HDC& _hdc);
	void Release(void);

private:
	CSceneManager();
	~CSceneManager();

private:
	CScene* m_pScene = nullptr;

	ID		m_eCurScene = SCENE_END;
	ID		m_eNextScene = SCENE_END;
};

