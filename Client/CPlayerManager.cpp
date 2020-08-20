#include "pch.h"
#include "CObj.h"
#include "CPlayer.h"
#include "CPlayerManager.h"
#include "CInventory.h"

DEFINITION_SINGLETON(CPlayerManager)

bool CPlayerManager::Ready(void)
{
	if (!CreateInventory())
		return false;

	// 플레이어 관련 객체들 셋팅
	if (!CreatePlayer())
		return false;

	return true;
}

void CPlayerManager::Update(void)
{
	DO_IF_IS_VALID_OBJ(m_pPlayer) { m_pPlayer->Update(); }
}

void CPlayerManager::LateUpdate(void)
{
	DO_IF_IS_VALID_OBJ(m_pPlayer) { m_pPlayer->LateUpdate(); }

	// 플레이어가 무효하다면 객체지워주기
	DO_IF_IS_NOT_VALID_OBJ(m_pPlayer) { m_pPlayer.reset(); }
}

void CPlayerManager::Render(const HDC& _hdc)
{
	DO_IF_IS_VALID_OBJ(m_pPlayer) { m_pPlayer->Render(_hdc); }
}

void CPlayerManager::Release(void)
{
}

bool CPlayerManager::CreateInventory(void)
{
	// 인벤토리가 있다면 지우고 생성
	if(m_pInventory) { m_pInventory.reset(); }
	m_pInventory = make_unique<CInventory>();
	m_pInventory->Ready();

	return true;
}

bool CPlayerManager::CreatePlayer(void)
{
	// 플레이어가 있다면 지우고 생성
	DO_IF_IS_VALID_OBJ(m_pPlayer) { m_pPlayer.reset(); }
	m_pPlayer = CreateUnique<CPlayer>();

	return true;
}

CPlayerManager::CPlayerManager()
	:
	m_pPlayer{ nullptr },
	m_pInventory{ nullptr }
{
}

CPlayerManager::~CPlayerManager()
{
	Release();
}
