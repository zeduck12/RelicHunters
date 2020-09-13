#include "pch.h"
#include "CObjManager.h"
#include "CPlayer.h"
#include "CPlayerManager.h"
#include "CMonster.h"
#include "CCameraManager.h"
#include "CMapManager.h"
#include "CGraphicDevice.h"
#include "CTextureManager.h"
#include "CBoss.h"
#include "CEnemyManager.h"
#include "CInteractionManager.h"
#include "CItem.h"
#include "CSceneManager.h"
#include "UICameraManager.h"
#include "CHologram.h"
#include "CKamikazeCage.h"

DEFINITION_SINGLETON(CObjManager)

CObjManager::CObjManager()
	:
	m_listBullets{ },
	m_listGrenades{ },
	m_listCasings{ },
	m_listMonsters{ },
	m_listItems{ },
	m_bIsInstall{ false }
{
}

CObjManager::~CObjManager()
{
	Release();
}

bool CObjManager::Ready(void)
{
	// 몬스터 생성.
	CEnemyManager::LoadMonsterData();

	if (!GET_SINGLE(CPlayerManager)->Ready())
		return false;

	if (!GET_SINGLE(CCameraManager)->Ready())
		return false;

	if (!GET_SINGLE(UICameraManager)->Ready())
		return false;

	if (!GET_SINGLE(CMapManager)->Ready())
		return false;
	

	// 플레이어가 스폰되는 장소에 텔레포트 설치
	shared_ptr<CObj> pItem = make_shared<CTeleporter>(
		GET_SINGLE(CPlayerManager)->GetPlayer()->GetX(),
		GET_SINGLE(CPlayerManager)->GetPlayer()->GetY(),
		150.f, 100.f, IMAGE::TELEPORTER);
	pItem->Ready();
	m_listItems.emplace_back(pItem);

	shared_ptr<CObj> pHologram = nullptr;
	pHologram = make_shared<CHologram>(300.f, 400.f, 50.f, 50.f);
	pHologram->Ready();
	m_listHolograms.emplace_back(pHologram);

	pHologram = make_shared<CHologram>(500.f, 400.f, 50.f, 50.f, L"Pinky");
	pHologram->Ready();
	m_listHolograms.emplace_back(pHologram);

	pHologram = make_shared<CHologram>(700.f, 400.f, 50.f, 50.f, L"Raff");
	pHologram->Ready();
	m_listHolograms.emplace_back(pHologram);

	PlayBGM();
	SpawnKamikazeCage();

	return true;
}

void CObjManager::Update(void)
{
	GET_SINGLE(CMapManager)->Update();
	GET_SINGLE(CPlayerManager)->Update();
	GET_SINGLE(CCameraManager)->Update();
	GET_SINGLE(UICameraManager)->Update();
	for (auto& pBullet : m_listBullets) { DO_IF_IS_VALID_OBJ(pBullet) { pBullet->Update(); } }
	for (auto& pGrenade : m_listGrenades) { DO_IF_IS_VALID_OBJ(pGrenade) { pGrenade->Update(); } }
	for (auto& pCasing : m_listCasings) { DO_IF_IS_VALID_OBJ(pCasing) { pCasing->Update(); } }
	for (auto& pMonster : m_listMonsters) { DO_IF_IS_VALID_OBJ(pMonster) { pMonster->Update(); } }
	for (auto& pItem : m_listItems) { DO_IF_IS_VALID_OBJ(pItem) { pItem->Update(); } }
	for (auto& pParticle : m_listParticles) { DO_IF_IS_VALID_OBJ(pParticle) { pParticle->Update(); } }
	for (auto& pHologram : m_listHolograms) { DO_IF_IS_VALID_OBJ(pHologram) { pHologram->Update(); } }

	InstallTeleporter();
	SceneChange(); // Scene 체인지
}

void CObjManager::LateUpdate(void)
{
	GET_SINGLE(CMapManager)->LateUpdate();
	GET_SINGLE(CPlayerManager)->LateUpdate();
	GET_SINGLE(CCameraManager)->LateUpdate();
	for (auto& pBullet : m_listBullets) { DO_IF_IS_VALID_OBJ(pBullet) { pBullet->LateUpdate(); } }
	for (auto& pGrenade : m_listGrenades) { DO_IF_IS_VALID_OBJ(pGrenade) { pGrenade->LateUpdate(); } }
	for (auto& pCasing : m_listCasings) { DO_IF_IS_VALID_OBJ(pCasing) { pCasing->LateUpdate(); } }
	for (auto& pMonster : m_listMonsters) { DO_IF_IS_VALID_OBJ(pMonster) { pMonster->LateUpdate(); } }
	for (auto& pHologram : m_listHolograms) { DO_IF_IS_VALID_OBJ(pHologram) { pHologram->LateUpdate(); } }

	// 만약에 오브젝트들이 무효하면 없애주기.
	CollectGarbageObjects(m_listBullets);
	CollectGarbageObjects(m_listGrenades);
	CollectGarbageObjects(m_listCasings);
	CollectGarbageObjects(m_listMonsters);
	CollectGarbageObjects(m_listItems);
	CollectGarbageObjects(m_listParticles);
	CollectGarbageObjects(m_listHolograms);
}

void CObjManager::Render(const HDC& _hdc)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return;

	XFORM xfWorld = {};
	xfWorld = GET_SINGLE(CCameraManager)->GetWorldMatrix();
	SetWorldTransform(_hdc, &xfWorld);

	GET_SINGLE(CMapManager)->Render(_hdc);

	for (auto& pItem : m_listItems) { DO_IF_IS_VALID_OBJ(pItem) { pItem->Render(_hdc); } }
	GET_SINGLE(CPlayerManager)->Render(_hdc);
	
	for (auto& pMonster : m_listMonsters) { DO_IF_IS_VALID_OBJ(pMonster) { pMonster->Render(_hdc); } }
	for (auto& pBullet : m_listBullets) { DO_IF_IS_VALID_OBJ(pBullet) { pBullet->Render(_hdc); } }
	for (auto& pGrenade : m_listGrenades) { DO_IF_IS_VALID_OBJ(pGrenade) { pGrenade->Render(_hdc); } }
	for (auto& pCasing : m_listCasings) { DO_IF_IS_VALID_OBJ(pCasing) { pCasing->Render(_hdc); } }
	for (auto& pParticle : m_listParticles) { DO_IF_IS_VALID_OBJ(pParticle) { pParticle->Render(_hdc); } }
	for (auto& pHologram : m_listHolograms) { DO_IF_IS_VALID_OBJ(pHologram) { pHologram->Render(_hdc); } }


	// 카메라 움직임 결과
	D3DXMATRIX matWorld = GET_SINGLE(CCameraManager)->GetWorldD3DMatrix();
	GET_SINGLE(CGraphicDevice)->GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);	
	GET_SINGLE(CGraphicDevice)->GetSprite()->End();
	GET_SINGLE(CGraphicDevice)->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	GET_SINGLE(UICameraManager)->Render();

	// 여기서 포신 그려주기
	//if (GET_SINGLE(CCameraManager)->IsPressing() == true)
	//	DrawLine();
	
	XFORM xf2 = { 1,0,0,1,0,0 };
	SetWorldTransform(_hdc, &xf2);
}

void CObjManager::InstallTeleporter(void)
{
	if (m_listMonsters.size() <= 0 && m_bIsInstall == false)
	{
		m_bIsInstall = true;
		if (GET_SINGLE(CSceneManager)->GetCurSceneID() == CSceneManager::SCENE_GAME3)
		{
			shared_ptr<CObj> pItem = make_shared<CTeleporter>(
				815.f,
				1260.f,
				150.f, 100.f, IMAGE::TELEPORTER, TELEPORTER::SPAWN);
			pItem->Ready();
			m_listItems.emplace_back(pItem);
		}
		else if (GET_SINGLE(CSceneManager)->GetCurSceneID() == CSceneManager::SCENE_GAME4)
		{
			shared_ptr<CObj> pItem = make_shared<CTeleporter>(
				1600.f,
				1750.f,
				150.f, 100.f, IMAGE::TELEPORTER, TELEPORTER::SPAWN);
			pItem->Ready();
			m_listItems.emplace_back(pItem);
		}
		else if (GET_SINGLE(CSceneManager)->GetCurSceneID() == CSceneManager::SCENE_TEST)
		{
			shared_ptr<CObj> pItem = make_shared<CTeleporter>(
				460.f,
				1860.f,
				150.f, 100.f, IMAGE::TELEPORTER, TELEPORTER::SPAWN);
			pItem->Ready();
			m_listItems.emplace_back(pItem);
		}
		else
		{
			shared_ptr<CObj> pItem = make_shared<CTeleporter>(
				900.f,
				600.f,
				150.f, 100.f, IMAGE::TELEPORTER, TELEPORTER::SPAWN);
			pItem->Ready();
			m_listItems.emplace_back(pItem);
		}
	}
}

void CObjManager::SceneChange(void)
{
	if (GET_SINGLE(CSceneManager)->IsChangeScene() == true)
	{
		GET_SINGLE(CSceneManager)->SetIsChangeScene(false);

		if (dynamic_cast<CPlayer*>(GET_SINGLE(CPlayerManager)->GetPlayer())->IsDead() == true)
			GET_SINGLE(CSceneManager)->ChangeScene(CSceneManager::SCENE_LOBBY);
		else if(GET_SINGLE(CSceneManager)->GetCurSceneID() == CSceneManager::SCENE_GAME)
			GET_SINGLE(CSceneManager)->ChangeScene(CSceneManager::SCENE_GAME2);
		else if(GET_SINGLE(CSceneManager)->GetCurSceneID() == CSceneManager::SCENE_GAME2)
			GET_SINGLE(CSceneManager)->ChangeScene(CSceneManager::SCENE_GAME3);
		else if (GET_SINGLE(CSceneManager)->GetCurSceneID() == CSceneManager::SCENE_GAME3)
			GET_SINGLE(CSceneManager)->ChangeScene(CSceneManager::SCENE_GAME4);
		else if (GET_SINGLE(CSceneManager)->GetCurSceneID() == CSceneManager::SCENE_GAME4)
			GET_SINGLE(CSceneManager)->ChangeScene(CSceneManager::SCENE_END);
		else if(GET_SINGLE(CSceneManager)->GetCurSceneID() == CSceneManager::SCENE_TEST)
			GET_SINGLE(CSceneManager)->ChangeScene(CSceneManager::SCENE_GAME);
	}
}

void CObjManager::TestWeapons(void)
{
	//Test 아이템 
	//shared_ptr<CObj> pItem = make_shared<CPickUpLight>(900.f, 600.f, 50.f, 50.f, IMAGE::PICKUP_LIGHT);
	//pItem->Ready();
	//m_listBullets.emplace_back(pItem);

	//pItem = make_shared<CPickUpLight>(1000.f, 600.f, 50.f, 50.f, IMAGE::PICKUP_MEDIUM);
	//pItem->Ready();
	//m_listBullets.emplace_back(pItem);

	//pItem = make_shared<CPickUpLight>(1100.f, 600.f, 50.f, 50.f, IMAGE::PICKUP_HEAVY);
	//pItem->Ready();
	//m_listBullets.emplace_back(pItem);

	//pItem = make_shared<ShotGun>(700.f, 600.f, 50.f, 50.f, GUN::SHOTGUN);
	//pItem->Ready();
	//m_listBullets.emplace_back(pItem);

	//pItem = make_shared<HighMagGun>(600.f, 600.f, 50.f, 50.f, GUN::HIGH_MAG);
	//pItem->Ready();
	//m_listBullets.emplace_back(pItem);

	//pItem = make_shared<SniperGun>(500.f, 600.f, 50.f, 50.f, GUN::SNIPER);
	//pItem->Ready();
	//m_listBullets.emplace_back(pItem);

	//pItem = make_shared<MachineGun>(500.f, 700.f, 50.f, 50.f, GUN::MACHINEGUN);
	//pItem->Ready();
	//m_listBullets.emplace_back(pItem);

	//pItem = make_shared<FlameGun>(600.f, 700.f, 50.f, 50.f, GUN::FLAME);
	//pItem->Ready();
	//m_listBullets.emplace_back(pItem);

	//pItem = make_shared<AssaultGun>(700.f, 700.f, 50.f, 50.f, GUN::ASSAULT);
	//pItem->Ready();
	//m_listBullets.emplace_back(pItem);

	//pItem = make_shared<KeytarGun>(800.f, 700.f, 50.f, 50.f, GUN::KEYTAR);
	//pItem->Ready();
	//m_listBullets.emplace_back(pItem);

	//pItem = make_shared<PistolAssualtGun>(900.f, 700.f, 50.f, 50.f, GUN::PISTOL_ASSUALT);
	//pItem->Ready();
	//m_listBullets.emplace_back(pItem);

	//pItem = make_shared<PistolHeavyGun>(1000.f, 700.f, 50.f, 50.f, GUN::PISTOL_HEAVY);
	//pItem->Ready();
	//m_listBullets.emplace_back(pItem);

	//pItem = make_shared<PlasmaGun>(1100.f, 700.f, 50.f, 50.f, GUN::PLASMA);
	//pItem->Ready();
	//m_listBullets.emplace_back(pItem);
}

void CObjManager::DrawLine(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return;
	CPlayer* pRealPlayer = dynamic_cast<CPlayer*>(pPlayer);
	LINEINFO tPosinPos = pRealPlayer->GetPosinPos();

	LPD3DXLINE pLine = nullptr;
	D3DXCreateLine(GET_SINGLE(CGraphicDevice)->GetDevice(), &pLine);

	float fPlayerX = 0.f;
	float fPlayerY = 0.f;
	fPlayerX = GET_SINGLE(CPlayerManager)->GetPlayer()->GetX();
	fPlayerY = GET_SINGLE(CPlayerManager)->GetPlayer()->GetY();
	fPlayerX -= float(WINCX >> 1) + GET_SINGLE(CCameraManager)->GetCameraDeltaX();
	fPlayerY -= float(WINCY >> 1) + GET_SINGLE(CCameraManager)->GetCameraDeltaY();

	float fScale = GET_SINGLE(CCameraManager)->GetCameraScale();
	D3DXVECTOR2 vecList[2] =
	{
		D3DXVECTOR2(float(tPosinPos.tLPoint.fX - fPlayerX) , float(tPosinPos.tLPoint.fY - fPlayerY)),
		D3DXVECTOR2(float(tPosinPos.tRPoint.fX - fPlayerX) , float(tPosinPos.tRPoint.fY - fPlayerY))
	};
	
	pLine->Begin();
	pLine->Draw(vecList, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
	pLine->End();
	pLine->Release();
}

void CObjManager::PlayBGM(void)
{
	if (GET_SINGLE(CSceneManager)->GetNextSceneID() == CSceneManager::SCENE_GAME)
	{
		CSoundManager::Get_Instance()->StopAll();
		CSoundManager::Get_Instance()->PlayBGM((TCHAR*)L"bgm_stage1.wav");
	}
	else if (GET_SINGLE(CSceneManager)->GetNextSceneID() == CSceneManager::SCENE_GAME2)
	{
		CSoundManager::Get_Instance()->StopAll();
		CSoundManager::Get_Instance()->PlayBGM((TCHAR*)L"bgm_stage2.wav");
	}
	else if (GET_SINGLE(CSceneManager)->GetNextSceneID() == CSceneManager::SCENE_GAME3)
	{
		CSoundManager::Get_Instance()->StopAll();
		CSoundManager::Get_Instance()->PlayBGM((TCHAR*)L"bgm_stage3.wav");
	}
	else if (GET_SINGLE(CSceneManager)->GetNextSceneID() == CSceneManager::SCENE_GAME4)
	{
		CSoundManager::Get_Instance()->StopAll();
		CSoundManager::Get_Instance()->PlayBGM((TCHAR*)L"bgm_boss.wav");
	}
	else if (GET_SINGLE(CSceneManager)->GetNextSceneID() == CSceneManager::SCENE_TEST)
	{
		CSoundManager::Get_Instance()->StopAll();
		CSoundManager::Get_Instance()->PlayBGM((TCHAR*)L"bgm_main.wav");
	}
}

void CObjManager::SpawnKamikazeCage(void)
{
	if (GET_SINGLE(CSceneManager)->GetNextSceneID() != CSceneManager::SCENE_GAME3)
		return;

	shared_ptr<CObj> pMonster = make_shared<CKamikazeCage>(
		1669.f,
		756.f,
		50.f, 50.f);
	pMonster->Ready();
	GET_SINGLE(CObjManager)->GetMonsters().emplace_back(pMonster);

	pMonster = make_shared<CKamikazeCage>(
		1840.f,
		2823.f,
		50.f, 50.f);
	pMonster->Ready();
	GET_SINGLE(CObjManager)->GetMonsters().emplace_back(pMonster);
}


void CObjManager::Release(void)
{
	/*m_listItems.clear();
	m_listGrenades.clear();
	m_listCasings.clear();
	m_listBullets.clear();
	m_listMonsters.clear();
	m_listParticles.clear();
	m_listHolograms.clear();*/
	DeleteListSafe(m_listItems);
	DeleteListSafe(m_listGrenades);
	DeleteListSafe(m_listCasings);
	DeleteListSafe(m_listBullets);
	DeleteListSafe(m_listMonsters);
	DeleteListSafe(m_listParticles);
	DeleteListSafe(m_listHolograms);

	UICameraManager::Destroy_Instance();
	CCameraManager::Destroy_Instance();
	CPlayerManager::Destroy_Instance();
	CMapManager::Destroy_Instance();
}


