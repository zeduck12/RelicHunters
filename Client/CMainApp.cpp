#include "pch.h"
#include "CMainApp.h"
#include "CCollisionManager.h"
#include "CObjManager.h"
#include "CBullet.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CSceneManager.h"
#include "CKeyManager.h"
#include "CGraphicDevice.h"
#include "CTextureManager.h"

CMainApp::CMainApp()
	:
	m_hDC{0}
{
}

CMainApp::~CMainApp()
{
	Release();
}

void CMainApp::Ready()
{
	// 장치 초기화
	if (E_FAIL == GET_SINGLE(CGraphicDevice)->Ready())
		return;
	
	SetImages();

	m_hDC = GetDC(g_hWND);
	SetGraphicsMode(m_hDC, GM_ADVANCED);

	// 키매니저 생성
	GET_SINGLE(CKeyManager);

	// 씬 로딩
	GET_SINGLE(CSceneManager)->ChangeScene(CSceneManager::SCENE_GAME);
}

void CMainApp::Update()
{
	GET_SINGLE(CKeyManager)->Update();
	GET_SINGLE(CSceneManager)->Update();
}

void CMainApp::LateUpdate()
{
	GET_SINGLE(CSceneManager)->LateUpdate();
}

void CMainApp::Render()
{
	GET_SINGLE(CSceneManager)->Render(m_hDC);
}

void CMainApp::Release()
{
	ReleaseDC(g_hWND, m_hDC);
	CKeyManager::Destroy_Instance();
	CCollisionManager::Destroy_Instance();
	CTextureManager::Destroy_Instance();
	CGraphicDevice::Destroy_Instance();
	CSceneManager::Destroy_Instance();
}

void CMainApp::SetImages(void)
{
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Tile/Tile%d.png", L"Terrain", L"Tile", 3))
		return;

	// Tileset
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Tile/tileset%d.png", L"Terrain", L"Tileset", 5))
		return;

	// Player Test Image
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Idle/idle_%d.png", L"Player", L"Idle", 12))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Move/move_%d.png", L"Player", L"Move", 6))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Attacked/attacked_%d.png", L"Player", L"Attacked", 2))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Dash/dash_%d.png", L"Player", L"Dash", 3))
		return;

	// Duck Image
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Duck/Idle/idle_%d.png", L"Duck", L"Idle", 7))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Duck/Move/move_%d.png", L"Duck", L"Move", 13))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Duck/Attacked/attacked_%d.png", L"Duck", L"Attacked", 2))
		return;

	// Kamikaze Image
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Kamikaze/Idle/spr_kamikaze_idle_%d.png", L"Kamikaze", L"Idle", 16))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Kamikaze/Move/spr_kamikaze_walk_%d.png", L"Kamikaze", L"Move", 8))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Kamikaze/Attacked/spr_kamikaze_hit_%d.png", L"Kamikaze", L"Attacked", 2))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Kamikaze/Attack/spr_kamikaze_attack_%d.png", L"Kamikaze", L"Attack", 8))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Kamikaze/Dash/spr_kamikaze_dash_fx_%d.png", L"Kamikaze", L"Dash", 8))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Kamikaze/Death/spr_kamikaze_death_%d.png", L"Kamikaze", L"Death", 13))
		return;

	// Pistol
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/Pistol/pistol_%d.png", L"Weapon", L"Pistol", 3))
		return;

	// Bullet
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Bullet/Small/bullet_small_%d.png", L"Bullet", L"Small", 2))
		return;

	// Object
	if (E_FAIL == FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Tile/structure%d.png", L"Terrain", L"Object", 2)))
		return;

}



