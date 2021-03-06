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
#include "CTimeManager.h"
#include "CMouse.h"
#include "UICameraManager.h"

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
	if (!GET_SINGLE(CSoundManager)->Ready())
		return;
	
	SetImages();

	m_hDC = GetDC(g_hWND);
	SetGraphicsMode(m_hDC, GM_ADVANCED);

	srand(static_cast<unsigned int>(time(nullptr)));	// 랜덤값을 생성하기 위한 시드 전달.

	// 키매니저 생성
	GET_SINGLE(CKeyManager);
	// 마우스 생성
	m_pMouse = make_unique<CMouse>();
	m_pMouse->Ready();

	// 씬 로딩
	GET_SINGLE(CSceneManager)->ChangeScene(CSceneManager::SCENE_LOGO);
}

void CMainApp::Update()
{
	GET_SINGLE(CKeyManager)->Update();
	GET_SINGLE(CSceneManager)->Update();
	m_pMouse->Update();

}

void CMainApp::LateUpdate()
{
	GET_SINGLE(CSceneManager)->LateUpdate();
	m_pMouse->LateUpdate();
}

void CMainApp::Render()
{
	// 그림시작
	GET_SINGLE(CGraphicDevice)->RenderBegin();
	GET_SINGLE(CSceneManager)->Render(m_hDC);
	m_pMouse->Render(m_hDC);
	GET_SINGLE(CGraphicDevice)->GetSprite()->End();
	GET_SINGLE(CGraphicDevice)->RenderEnd();
}

void CMainApp::Release()
{
	ReleaseDC(g_hWND, m_hDC);
	m_pMouse.reset();
	CTimeManager::Destroy_Instance();
	CKeyManager::Destroy_Instance();
	CCollisionManager::Destroy_Instance();
	CSceneManager::Destroy_Instance();
	CSoundManager::Destroy_Instance();
	CGraphicDevice::Destroy_Instance();
	CTextureManager::Destroy_Instance();
	CObjManager::Destroy_Instance();
}

void CMainApp::SetImages(void)
{
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Tile/Tile%d.png", L"Terrain", L"Tile", 3))
		return;

	// Tileset
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Tile/tileset%d.png", L"Terrain", L"Tileset", 44))
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
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Death/spr_jimmy_death_%d.png", L"Player", L"Death", 11))
		return;
	// Pinky Image
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Pinky_Idle/spr_pinky_idle_%d.png", L"Player", L"Pinky_Idle", 9))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Pinky_Move/spr_pinky_sprint_%d.png", L"Player", L"Pinky_Move", 6))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Pinky_Attacked/spr_pinky_hit_%d.png", L"Player", L"Pinky_Attacked", 2))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Pinky_Dash/spr_pinky_dash_%d.png", L"Player", L"Pinky_Dash", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Pinky_Death/spr_pinky_death_%d.png", L"Player", L"Pinky_Death", 11))
		return;

	// Raff Image
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Raff_Idle/spr_raff_idle_%d.png", L"Player", L"Raff_Idle", 10))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Raff_Move/spr_raff_sprint_%d.png", L"Player", L"Raff_Move", 6))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Raff_Attacked/spr_raff_hit_%d.png", L"Player", L"Raff_Attacked", 2))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Raff_Dash/spr_raff_dash_%d.png", L"Player", L"Raff_Dash", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Player/Raff_Death/spr_raff_death_%d.png", L"Player", L"Raff_Death", 11))
		return;

	// Duck Image
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Duck/Idle/idle_%d.png", L"Duck", L"Idle", 7))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Duck/Move/move_%d.png", L"Duck", L"Move", 13))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Duck/Attacked/attacked_%d.png", L"Duck", L"Attacked", 2))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Duck/Death/spr_duck_death_%d.png", L"Duck", L"Death", 12))
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

	// Kamikaze Fly Imamge
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/KamikazeFly/Idle/spr_kamikazelite_idle_%d.png", L"KamikazeFly", L"Idle", 16))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/KamikazeFly/Move/spr_kamikazelite_walk_%d.png", L"KamikazeFly", L"Move", 8))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/KamikazeFly/Attacked/spr_kamikazelite_hit_%d.png", L"KamikazeFly", L"Attacked", 2))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/KamikazeFly/Fly/spr_kamikazelite_flying_0%d.png", L"KamikazeFly", L"Fly", 9))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/KamikazeFly/Death/spr_kamikazelite_death1_%d.png", L"KamikazeFly", L"Death", 16))
		return;

	// Turtle Image 
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Turtle/Idle/spr_turtle_idle_%d.png", L"Turtle", L"Idle", 12))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Turtle/Move/spr_turtle_walk_%d.png", L"Turtle", L"Move", 6))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Turtle/Attacked/spr_turtle_hit_%d.png", L"Turtle", L"Attacked", 2))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Turtle/Death/spr_turtle_death_%d.png", L"Turtle", L"Death", 13))
		return;

	// Boss Egg
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Boss/Egg/Idle/spr_boss_egg_idle_%d.png", L"BossEgg", L"Idle", 1))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Boss/Egg/Start/spr_boss_egg_start_%d.png", L"BossEgg", L"Start", 10))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Boss/Egg/Crack/spr_boss_egg_crack_%d.png", L"BossEgg", L"Crack", 21))
		return;

	// Boss Image
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Boss/Idle/spr_boss_idle_%d.png", L"Boss", L"Idle", 14))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Boss/Move/spr_boss_walk_%d.png", L"Boss", L"Move", 6))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Boss/Attacked/spr_boss_hit_%d.png", L"Boss", L"Attacked", 2))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Boss/Death/spr_boss_death_%d.png", L"Boss", L"Death", 13))
		return;

	// Generator
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Generator/Idle/spr_generator_%d.png", L"Generator", L"Idle", 16))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Generator/Spawn/spr_generatorActivation_%d.png", L"Generator", L"Spawn", 18))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Generator/Attacked/spr_generator_hit_%d.png", L"Generator", L"Attacked", 2))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/Generator/Death/spr_generatorOff_%d.png", L"Generator", L"Death", 1))
		return;

	// Cage
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/KamikazeCage/Idle/spr_cage1_%d.png", L"KamikazeCage", L"Idle", 12))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/KamikazeCage/Attacked/spr_cage1_hit_%d.png", L"KamikazeCage", L"Attacked", 1))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Monster/KamikazeCage/Death/spr_cage_destroy_%d.png", L"KamikazeCage", L"Death", 10))
		return;


	// Weapon
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/Boss/spr_boss_gun_%d.png", L"Weapon", L"Boss", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/Pistol/pistol_%d.png", L"Weapon", L"Pistol", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/Flame/flame_%d.png", L"Weapon", L"Flame", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/Assault/assault_%d.png", L"Weapon", L"Assault", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/HighMag/highmag_%d.png", L"Weapon", L"HighMag", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/Keytar/keytar_%d.png", L"Weapon", L"Keytar", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/MachineGun/machinegun_%d.png", L"Weapon", L"MachineGun", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/Pistol_Assualt/pistol_assault_%d.png", L"Weapon", L"Pistol_Assualt", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/Pistol_Heavy/pistol_heavy_%d.png", L"Weapon", L"Pistol_Heavy", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/Plasma/plasma_%d.png", L"Weapon", L"Plasma", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/Shotgun/shotgun_%d.png", L"Weapon", L"Shotgun", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/Sniper/sniper_%d.png", L"Weapon", L"Sniper", 3))
		return;

	// Bullet
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Bullet/Small/bullet_small_%d.png", L"Bullet", L"Small", 2))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Bullet/Default/bullet_%d.png", L"Bullet", L"Default", 2))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Bullet/Rocket/rocket_%d.png", L"Bullet", L"Rocket", 1))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Bullet/Blue/blue_%d.png", L"Bullet", L"Blue", 2))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Bullet/Plasma/spr_bullet_plasma_%d.png", L"Bullet", L"Plasma", 12))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Bullet/SonicBoom/sonicboom_%d.png", L"Bullet", L"SonicBoom", 2))
		return;

	// Casing
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Casing/Lite/casing_0.png", L"CasingLite"))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Casing/Middle/spr_casing_2.png", L"CasingMiddle"))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Casing/Heavy/spr_casing_1.png", L"CasingHeavy"))
		return;

	// Grenade
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Grenade/grenade_0.png", L"Grenade"))
		return;

	// Object
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Tile/structure%d.png", L"Terrain", L"Object", 4)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Structure/Barrel/barrel_%d.png", L"Structure", L"Barrel", 14)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Structure/Root1/root_%d.png", L"Structure", L"Root1", 11)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Structure/Root2/root_%d.png", L"Structure", L"Root2", 15)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Structure/Root3/root_%d.png", L"Structure", L"Root3", 12)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Structure/LongBarrel/longbox_%d.png", L"Structure", L"LongBarrel", 14)))
		return;

	// ITEM
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Item/PickUp/Light/spr_pickup_light_%d.png", L"PickUpLight", L"Start", 16)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Item/PickUp/Medium/spr_pickup_medium_%d.png", L"PickUpMedium", L"Start", 16)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Item/PickUp/Heavy/spr_pickup_heavy_%d.png", L"PickUpHeavy", L"Start", 16)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Item/PickUp/Shield/pickup_shield_0.png", L"PickUpShield")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Item/PickUp/Health/pickup_health_0.png", L"PickUpHealth")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Item/PickUp/Grenade/spr_pickup_grenade_%d.png", L"PickUpGrenade", L"Start", 17)))
		return;

	// Prompt E
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Prompt/spr_prompt_e_%d.png", L"Prompt", L"PromptE", 8)))
		return;

	// Logo Scene
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Logo/spr_splash_bg_0.png", L"Logo")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Logo/spr_splash_roguelogo_0.png", L"LogoText")))
		return;
	// Intro Scene
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Intro/spr_intro_animated_%d.png", L"Scene", L"Intro", 12)))
		return;

	// Lobby Scene
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Lobby/Background/bg_menu_main.png", L"MainBackground")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Lobby/Background/bg_pause.png", L"MainBackground2")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Lobby/Deco/spr_menu_star_0.png", L"Deco")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Lobby/Button/spr_menu_buttonMain_%d.png", L"Lobby", L"Button", 2)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Lobby/LogoBox/spr_logo_%d.png", L"Lobby", L"LogoBox", 42)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Lobby/Zueira/spr_zueira_menu_%d.png", L"Lobby", L"Zueira", 23)))
		return;

	// Menu Scene
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Menu/Background/background.png", L"MenuBackground")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Menu/StatsBox/statsbox.png", L"StatsBox")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Menu/PlayerCard/playerCard.png", L"PlayerCard")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Menu/PlayerCard/playerStat.png", L"PlayerStat")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Menu/Button/spr_char_Button_%d.png", L"Menu", L"Button", 2)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Menu/CharacterSel/Jimmy/Idle/spr_char_jimmyOn_0.png", L"JimmyCardIdle")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Menu/CharacterSel/Jimmy/Active/spr_char_jimmySel_%d.png", L"Menu", L"JimmySel", 11)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Menu/CharacterSel/Jimmy/Clicked/spr_char_jimmyFinal_%d.png", L"Menu", L"JimmyClicked", 5)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Menu/CharacterSel/Pinky/Idle/spr_char_pinkyOn_0.png", L"PinkyCardIdle")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Menu/CharacterSel/Pinky/Active/spr_char_pinkySel_%d.png", L"Menu", L"PinkySel", 11)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Menu/CharacterSel/Pinky/Clicked/spr_char_pinkyFinal_%d.png", L"Menu", L"PinkyClicked", 5)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Menu/CharacterSel/Raff/Idle/spr_char_raffOn_0.png", L"RaffCardIdle")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Menu/CharacterSel/Raff/Active/spr_char_raffSel_%d.png", L"Menu", L"RaffSel", 11)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Menu/CharacterSel/Raff/Clicked/spr_char_raffFinal_%d.png", L"Menu", L"RaffClicked", 5)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Menu/CharacterSel/Biu/Death/spr_char_biuOff_0.png", L"BiuCardDeath")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Menu/CharacterSel/Punny/Death/spr_char_punnyOff_0.png", L"PunnyCardDeath")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Menu/CharacterSel/Ass/Death/spr_char_assOff_0.png", L"AssCardDeath")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/Menu/CharacterSel/Random/Idle/spr_char_randomOn_0.png", L"RadomCardIdle")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Menu/CharacterSel/Random/Active/spr_char_randomSel_%d.png", L"Menu", L"RandomSel", 11)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Menu/CharacterSel/Random/Clicked/spr_char_randomFinal_%d.png", L"Menu", L"RandomClicked", 5)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Menu/CharacterCard/spr_card_%d.png", L"Menu", L"CharacterCard", 6)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Scene/Menu/StarBar/spr_char_statBar_%d.png", L"Menu", L"StarBar", 3)))
		return;

	// Mouse
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Mouse/CrossHair/spr_crosshair_%d.png", L"Mouse", L"CrossHair", 3)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Mouse/Reload/spr_crosshair_reload_%d.png", L"Mouse", L"Reload", 12)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Mouse/ReloadChar/spr_reload_char_%d.png", L"Mouse", L"ReloadChar", 12)))
		return;

	// Coin
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Coin/Drop/spr_coin_drop_%d.png", L"Coin", L"Drop", 5)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Coin/Idle/spr_coin_ground_%d.png", L"Coin", L"Idle", 14)))
		return;

	// Particle
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Particle/Bomb/spr_grenade_exp1_%d.png", L"Particle", L"Bomb", 11)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Particle/Hit/spr_hit_light_%d.png", L"Particle", L"Hit", 6)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Particle/Dash/spr_sprint_%d.png", L"Particle", L"Dash", 6)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Particle/Death/spr_death_generic0_%d.png", L"Particle", L"Death", 6)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Particle/Finish/finish_%d.png", L"Particle", L"Finish", 1)))
		return;
	// Attacked Particle
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Particle/Attacked/spr_crate_particles_1.png", L"AttackedParticle"))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Particle/Attacked/white.png", L"WhiteParticle"))
		return;

	// Teleporter
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Teleporter/Idle/Idle_%d.png", L"Teleporter", L"Idle", 1)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Teleporter/Spawn/spr_teleporter_spawn_%d.png", L"Teleporter", L"Spawn", 11)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Teleporter/Effect/spr_teleporter_fx_%d.png", L"Teleporter", L"Effect", 17)))
		return;

	// Player UI 관련
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/UI/Emoticon/Jimmy/spr_hud_face_jimmy_%d.png", L"Emoticon", L"Jimmy", 3)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/UI/Emoticon/Pinky/spr_hud_face_pinky_%d.png", L"Emoticon", L"Pinky", 3)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/UI/Emoticon/Raff/spr_hud_face_raff_%d.png", L"Emoticon", L"Raff", 3)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/UI/DashBar/Dash_%d.png", L"DashBar", L"DashBar", 2)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/UI/Icon/icon_%d.png", L"Icon", L"Icon", 2)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/UI/CoinBar/spr_dynamic_bounty_0.png", L"CoinBar")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/UI/Hp/spr_hud_hp_0.png", L"HpBar")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/UI/Hp/white.png", L"White")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/UI/Emoticon/Boss/spr_boss_barFace_%d.png", L"Emoticon", L"Boss", 3)))
		return;

	// Shield
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Shield/Active/spr_shield_blue_%d.png", L"Shield", L"Active", 6)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Shield/Explosion/spr_shield_explosion_blue_%d.png", L"Shield", L"Explosion", 9)))
		return;

	//Stage
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Stage/Stage1/spr_stage1_1_%d.png", L"Stage", L"Stage1", 13)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Stage/Stage2/spr_stage2_1_%d.png", L"Stage", L"Stage2", 13)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Stage/Stage3/spr_stage3_1_%d.png", L"Stage", L"Stage3", 13)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Stage/Stage4/spr_stage_boss_%d.png", L"Stage", L"Stage4", 13)))
		return;

	//End
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/End/Background/bg_end.png", L"EndBackground")))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Scene/End/Text/spr_intro_text_0.png", L"EndText")))
		return;

	// Activation
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Activation/spr_activation_marker_%d.png", L"Activation", L"Activation", 7)))
		return;

	// Hologram
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Hologram/Jimmy/spr_jimmy_holo_%d.png", L"Hologram", L"Jimmy", 27)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Hologram/Pinky/spr_pinky_holo_%d.png", L"Hologram", L"Pinky", 27)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Hologram/Raff/spr_raff_holo_%d.png", L"Hologram", L"Raff", 27)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Hologram/Altar/spr_altar_players_0.png", L"Altar")))
		return;

	// SuperArmor
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Particle/SuperArmor/spr_boss_collisionMask_0.png", L"SuperArmor")))
		return;
	//reflectBoard
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/ReflectBoard/reflectBoard.png", L"ReflectBoard")))
		return;

	// 궁극기 스프라이트
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Special/Armor/armor_%d.png", L"Special", L"Armor", 2)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Special/Particle/spr_fire_%d.png", L"Special", L"Particle", 8)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Special/Explosion/spr_grenade_exp1_%d.png", L"Special", L"Explosion", 11)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Special/Icon/icon_%d.png", L"Special", L"SpecialIcon", 3)))
		return;

	// DebugMode 
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Debug/spr_char_inputJoy1_%d.png", L"Debug", L"Debug", 7)))
		return;

	// Cards
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Card/card_%d.png", L"Card", L"Card", 7)))
		return;

	//OpenSource
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/UI/OpenSource/spr_opensource_menu_%d.png", L"Menu", L"OpenSource", 2)))
		return;
	//Clear
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Clear/CLEAR.png", L"Clear"))
		return;
}



