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

DEFINITION_SINGLETON(CObjManager)

CObjManager::CObjManager()
	:
	m_listBullets{ },
	m_listGrenades{ },
	m_listCasings{ },
	m_listMonsters{ },
	m_listItems{ }
{
}

CObjManager::~CObjManager()
{
	Release();
}

bool CObjManager::Ready(void)
{
	if (!GET_SINGLE(CPlayerManager)->Ready())
		return false;

	if (!GET_SINGLE(CCameraManager)->Ready())
		return false;

	if (!GET_SINGLE(CMapManager)->Ready())
		return false;


	// 몬스터 생성.
	CEnemyManager::LoadMonsterData();

	//Test 아이템 
	shared_ptr<CObj> pItem = make_shared<CPickUpLight>(800.f, 600.f, 50.f, 50.f, IMAGE::PICKUP_LIGHT);
	pItem->Ready();
	m_listBullets.emplace_back(pItem);

	pItem = make_shared<CPickUpLight>(900.f, 600.f, 50.f, 50.f, IMAGE::PICKUP_MEDIUM);
	pItem->Ready();
	m_listBullets.emplace_back(pItem);

	pItem = make_shared<CPickUpLight>(1000.f, 600.f, 50.f, 50.f, IMAGE::PICKUP_HEAVY);
	pItem->Ready();
	m_listBullets.emplace_back(pItem);

	// 보스생성
	//shared_ptr<CObj> pMonster = make_shared<CBoss>(800.f, 1400.f, 120.f, 120.f,
	//	cfMosterDefaultSpeed, cfMosterDefaultHp, IMAGE::BOSS);
	//pMonster->Ready();
	//m_listMonsters.emplace_back(pMonster);

	return true;
}

void CObjManager::Update(void)
{
	GET_SINGLE(CMapManager)->Update();
	GET_SINGLE(CPlayerManager)->Update();
	GET_SINGLE(CCameraManager)->Update();
	for (auto& pBullet : m_listBullets) { DO_IF_IS_VALID_OBJ(pBullet) { pBullet->Update(); } }
	for (auto& pGrenade : m_listGrenades) { DO_IF_IS_VALID_OBJ(pGrenade) { pGrenade->Update(); } }
	for (auto& pCasing : m_listCasings) { DO_IF_IS_VALID_OBJ(pCasing) { pCasing->Update(); } }
	for (auto& pMonster : m_listMonsters) { DO_IF_IS_VALID_OBJ(pMonster) { pMonster->Update(); } }
	for (auto& pItem : m_listItems) { DO_IF_IS_VALID_OBJ(pItem) { pItem->Update(); } }
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

	// 만약에 오브젝트들이 무효하면 없애주기.
	CollectGarbageObjects(m_listBullets);
	CollectGarbageObjects(m_listGrenades);
	CollectGarbageObjects(m_listCasings);
	CollectGarbageObjects(m_listMonsters);
	CollectGarbageObjects(m_listItems);
}

void CObjManager::Render(const HDC& _hdc)
{
	// 그림시작
	GET_SINGLE(CGraphicDevice)->RenderBegin();

	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return;

	XFORM xfWorld = {};
	xfWorld = GET_SINGLE(CCameraManager)->GetWorldMatrix();
	SetWorldTransform(_hdc, &xfWorld);

	GET_SINGLE(CMapManager)->Render(_hdc);
	GET_SINGLE(CPlayerManager)->Render(_hdc);
	
	for (auto& pMonster : m_listMonsters) { DO_IF_IS_VALID_OBJ(pMonster) { pMonster->Render(_hdc); } }
	for (auto& pBullet : m_listBullets) { DO_IF_IS_VALID_OBJ(pBullet) { pBullet->Render(_hdc); } }
	for (auto& pGrenade : m_listGrenades) { DO_IF_IS_VALID_OBJ(pGrenade) { pGrenade->Render(_hdc); } }
	for (auto& pCasing : m_listCasings) { DO_IF_IS_VALID_OBJ(pCasing) { pCasing->Render(_hdc); } }
	for (auto& pItem : m_listItems) { DO_IF_IS_VALID_OBJ(pItem) { pItem->Render(_hdc); } }


	// 카메라 움직임 결과
	D3DXMATRIX matWorld = GET_SINGLE(CCameraManager)->GetWorldD3DMatrix();
	GET_SINGLE(CGraphicDevice)->GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
	
	
	GET_SINGLE(CGraphicDevice)->GetSprite()->End(); 
	GET_SINGLE(CGraphicDevice)->RenderEnd();
	
	XFORM xf2 = { 1,0,0,1,0,0 };
	SetWorldTransform(_hdc, &xf2);
}

void CObjManager::Release(void)
{
	m_listGrenades.clear();
	m_listCasings.clear();
	m_listBullets.clear();
	m_listMonsters.clear();

	CCameraManager::Destroy_Instance();
	CPlayerManager::Destroy_Instance();
	CMapManager::Destroy_Instance();
	CTextureManager::Destroy_Instance();
}


