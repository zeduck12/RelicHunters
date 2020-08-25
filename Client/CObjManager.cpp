#include "pch.h"
#include "CObjManager.h"
#include "CPlayer.h"
#include "CPlayerManager.h"
#include "CMonster.h"
#include "CCameraManager.h"
#include "CMapManager.h"
#include "CGraphicDevice.h"
#include "CTextureManager.h"

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

	if (!GET_SINGLE(CMapManager)->Ready())
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
	// 그림시작
	GET_SINGLE(CGraphicDevice)->RenderBegin();

	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return;

	XFORM xfWorld = {};
	xfWorld = GET_SINGLE(CCameraManager)->GetWorldMatrix();
	SetWorldTransform(_hdc, &xfWorld);

	GET_SINGLE(CMapManager)->Render();
	GET_SINGLE(CPlayerManager)->Render(_hdc);

	D3DXVECTOR3 v[2]
		=
	{
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(300, 300, 0)
	};

	D3DXMATRIX matTotal, matView, matPro, matTrans;
	GET_SINGLE(CGraphicDevice)->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	GET_SINGLE(CGraphicDevice)->GetDevice()->GetTransform(D3DTS_PROJECTION, &matPro);
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 1.f);
	matTotal = matView * matPro /** matTrans*/;

	LPD3DXLINE pLine = nullptr;
	D3DXCreateLine(GET_SINGLE(CGraphicDevice)->GetDevice(), &pLine);
	pLine->Begin();
	pLine->DrawTransform(v, 2, &matTotal, D3DCOLOR_ARGB(255, 255, 0, 0));
	pLine->End();
	pLine->Release();


	// 카메라 움직임 결과
	D3DXMATRIX matWorld = GET_SINGLE(CCameraManager)->GetWorldD3DMatrix();
	GET_SINGLE(CGraphicDevice)->GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
	
	GET_SINGLE(CGraphicDevice)->RenderEnd();

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
	CMapManager::Destroy_Instance();
}


