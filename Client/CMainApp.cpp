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

	srand(static_cast<unsigned int>(time(nullptr)));	// 랜덤값을 생성하기 위한 시드 전달.

	// 키매니저 생성
	GET_SINGLE(CKeyManager);

	// 씬 로딩
	GET_SINGLE(CSceneManager)->ChangeScene(CSceneManager::SCENE_LOGO);
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
	
	CTimeManager::Destroy_Instance();
	CKeyManager::Destroy_Instance();
	CCollisionManager::Destroy_Instance();
	CSceneManager::Destroy_Instance();
	CObjManager::Destroy_Instance();
	CGraphicDevice::Destroy_Instance();
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

	// Weapon
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/Pistol/pistol_%d.png", L"Weapon", L"Pistol", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/Flame/flame_%d.png", L"Weapon", L"Flame", 3))
		return;
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Weapon/Boss/spr_boss_gun_%d.png", L"Weapon", L"Boss", 3))
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

	// Casing
	if (E_FAIL == CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_SINGLE, L"../Texture/Casing/Lite/casing_0.png", L"CasingLite"))
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

	// ITEM
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Item/PickUp/Light/spr_pickup_light_%d.png", L"PickUpLight", L"Start", 16)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Item/PickUp/Medium/spr_pickup_medium_%d.png", L"PickUpMedium", L"Start", 16)))
		return;
	if (FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, L"../Texture/Item/PickUp/Heavy/spr_pickup_heavy_%d.png", L"PickUpHeavy", L"Start", 16)))
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

}



