#include "pch.h"
#include "CSceneManager.h"
#include "CGame.h"

// ½Ì±ÛÅæ Á¤ÀÇ
DEFINITION_SINGLETON(CSceneManager)

CSceneManager::CSceneManager()
{
}

CSceneManager::~CSceneManager()
{
	Release();
}

void CSceneManager::ChangeScene(ID _eSceneID)
{
	// ¾ÀÀ» »ý¼ºÇÏ°í ¸¸µé°í ¹¹½Ã±â ÇÒ²¨. 
	m_eNextScene = _eSceneID;
	if (m_eCurScene != m_eNextScene)
	{
		SAFE_DELETE(m_pScene);
		switch (m_eNextScene)
		{
		case CSceneManager::SCENE_LOGO:
			break;
		case CSceneManager::SCENE_INTRO:
			break;
		case CSceneManager::SCENE_LOBBY:
			break;
		case CSceneManager::SCENE_GAME:
			m_pScene = new CGame;
			break;
		case CSceneManager::SCENE_END:
			return;
		default:
			break;
		}

		if (!m_pScene->Ready())
			return;

		m_eCurScene = m_eNextScene;
	}
}

void CSceneManager::Update(void)
{
	m_pScene->Update();
}

void CSceneManager::LateUpdate(void)
{
	m_pScene->LateUpdate();
}

void CSceneManager::Render(const HDC& _hdc)
{
	m_pScene->Render(_hdc);
}

void CSceneManager::Release(void)
{
	SAFE_DELETE(m_pScene);
}
