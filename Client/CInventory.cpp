#include "pch.h"
#include "CInventory.h"

CInventory::~CInventory()
{
}

bool CInventory::Ready(void)
{
	m_iOwnBombs = 5;
	m_iCoins = 0;
	m_iOwnBullets = 9999;
	m_iCapacity = 2;
	// 인벤토리 크기 2개 예약
	m_vecInventory.reserve(m_iCapacity);
	m_eCurWeaponID = GUN::DEFAULT;

	return  true;
}

void CInventory::Release(void)
{
}

void CInventory::GainWeapon(GUN::ID _eID)
{
	if (m_vecInventory.size() >= 2)
	{
		// 인벤토리가 꽉찬 상태에서 무기 획득하면
		// 현재 사용중인거 버리고 획득한 걸로 교체
		m_vecInventory[0] = _eID;
		m_eCurWeaponID = m_vecInventory.front();
	}
	else
	{
		// 획득한 무기 추가.
		m_vecInventory.emplace_back(_eID);
	}
	
}

void CInventory::ChangeWeapon(void)
{
	// 무기가 1개이하일 때는 무기 교체 안됨.
	if (m_vecInventory.size() <= 1)
		return;

	GUN::ID eTemp = m_vecInventory[0];
	GUN::ID eTemp2 = m_vecInventory[1];

	m_eCurWeaponID = eTemp2;

	m_vecInventory[0] = eTemp2;
	m_vecInventory[1] = eTemp;
}
