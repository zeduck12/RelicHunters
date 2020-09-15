#include "pch.h"
#include "CSceneManager.h"
#include "UICameraManager.h"
#include "CGame.h"
#include "CIntro.h"
#include "CLogo.h"
#include "CLobby.h"
#include "CMenu.h"
#include "CEnd.h"


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
		Safe_Delete(m_pScene);
		switch (m_eNextScene)
		{
		case CSceneManager::SCENE_LOGO:
			m_pScene = new CLogo;
			break;
		case CSceneManager::SCENE_INTRO:
			m_pScene = new CIntro;
			break;
		case CSceneManager::SCENE_LOBBY:
			m_pScene = new CLobby;
			break;
		case CSceneManager::SCENE_MENU:
			m_pScene = new CMenu;
			break;
		case CSceneManager::SCENE_GAME:
			m_pScene = new CGame;
			break;
		case CSceneManager::SCENE_GAME2:
			m_pScene = new CGame;
			break;
		case CSceneManager::SCENE_GAME3:
			m_pScene = new CGame;
			break;
		case CSceneManager::SCENE_GAME4:
			m_pScene = new CGame;
			break;
		case CSceneManager::SCENE_TEST:
			m_pScene = new CGame;
			break;
		case CSceneManager::SCENE_EVENT:
			m_pScene = new CGame;
			break;
		case CSceneManager::SCENE_END:
			m_pScene = new CEnd;
			break;
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
	Safe_Delete(m_pScene);
}
