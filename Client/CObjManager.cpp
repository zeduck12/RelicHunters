#include "pch.h"
#include "CObjManager.h"
#include "CPlayer.h"
#include "CPlayerManager.h"
#include "CMonster.h"
#include "CCameraManager.h"

DEFINITION_SINGLETON(CObjManager)

CObjManager::CObjManager()
	:
	m_listBullets{ NULL },
	m_listGrenades{ NULL },
	m_listCasings{ NULL },
	m_listMonsters{ NULL }
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

	// TEST 몬스터 생성.
	shared_ptr<CObj> pMonster = make_shared<CMonster>(500.f, 500.f, 100.f, 100.f);
	pMonster->Ready();
	m_listMonsters.emplace_back(pMonster);

	return true;
}

void CObjManager::Update(void)
{
	GET_SINGLE(CPlayerManager)->Update();
	GET_SINGLE(CCameraManager)->Update();
	for (auto& pBullet : m_listBullets) { DO_IF_IS_VALID_OBJ(pBullet) { pBullet->Update(); } }
	for (auto& pGrenade : m_listGrenades) { DO_IF_IS_VALID_OBJ(pGrenade) { pGrenade->Update(); } }
	for (auto& pCasing : m_listCasings) { DO_IF_IS_VALID_OBJ(pCasing) { pCasing->Update(); } }
	for (auto& pMonster : m_listMonsters) { DO_IF_IS_VALID_OBJ(pMonster) { pMonster->Update(); } }
}

void CObjManager::LateUpdate(void)
{
	GET_SINGLE(CPlayerManager)->LateUpdate();
	GET_SINGLE(CCameraManager)->LateUpdate();
	for (auto& pBullet : m_listBullets) { DO_IF_IS_VALID_OBJ(pBullet) { pBullet->LateUpdate(); } }
	for (auto& pGrenade : m_listGrenades) { DO_IF_IS_VALID_OBJ(pGrenade) { pGrenade->LateUpdate(); } }
	for (auto& pCasing : m_listCasings) { DO_IF_IS_VALID_OBJ(pCasing) { pCasing->LateUpdate(); } }
	for (auto& pMonster : m_listMonsters) { DO_IF_IS_VALID_OBJ(pMonster) { pMonster->LateUpdate(); } }

	// 만약에 오브젝트들이 무효하면 없애주기.
	for (auto& pBullet : m_listBullets) { DO_IF_IS_NOT_VALID_OBJ(pBullet) { pBullet.reset(); } }
	for (auto& pGrenade : m_listGrenades) { DO_IF_IS_NOT_VALID_OBJ(pGrenade) { pGrenade.reset(); } }
	for (auto& pCasing : m_listCasings) { DO_IF_IS_NOT_VALID_OBJ(pCasing) { pCasing.reset(); } }
	for (auto& pMonster : m_listMonsters) { DO_IF_IS_NOT_VALID_OBJ(pMonster) { pMonster.reset(); } }
}

void CObjManager::Render(const HDC& _hdc)
{
	RECT rc{ 0,0, WINCX, WINCY };
	FillRect(_hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));

	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return;

	XFORM xfWorld = {};
	xfWorld = GET_SINGLE(CCameraManager)->GetWorldMatrix();
	SetWorldTransform(_hdc, &xfWorld);

	GET_SINGLE(CPlayerManager)->Render(_hdc);
	for (auto& pBullet : m_listBullets) { DO_IF_IS_VALID_OBJ(pBullet) { pBullet->Render(_hdc); } }
	for (auto& pMonster : m_listMonsters) { DO_IF_IS_VALID_OBJ(pMonster) { pMonster->Render(_hdc); } }
	for (auto& pGrenade : m_listGrenades) { DO_IF_IS_VALID_OBJ(pGrenade) { pGrenade->Render(_hdc); } }
	for (auto& pCasing : m_listCasings) { DO_IF_IS_VALID_OBJ(pCasing) { pCasing->Render(_hdc); } }
		

	XFORM xf2 = { 1,0,0,1,0,0 };
	SetWorldTransform(_hdc, &xf2);
}

void CObjManager::Release(void)
{
	CCameraManager::Destroy_Instance();
	CPlayerManager::Destroy_Instance();
}


