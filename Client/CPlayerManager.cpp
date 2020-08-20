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

	// �÷��̾� ���� ��ü�� ����
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

	// �÷��̾ ��ȿ�ϴٸ� ��ü�����ֱ�
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
	// �κ��丮�� �ִٸ� ����� ����
	if(m_pInventory) { m_pInventory.reset(); }
	m_pInventory = make_unique<CInventory>();
	m_pInventory->Ready();

	return true;
}

bool CPlayerManager::CreatePlayer(void)
{
	// �÷��̾ �ִٸ� ����� ����
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
