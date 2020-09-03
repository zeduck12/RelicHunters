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
	// �κ��丮 ũ�� 2�� ����
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
		// �κ��丮�� ���� ���¿��� ���� ȹ���ϸ�
		// ���� ������ΰ� ������ ȹ���� �ɷ� ��ü
		m_vecInventory[0] = _eID;
		m_eCurWeaponID = m_vecInventory.front();
	}
	else
	{
		// ȹ���� ���� �߰�.
		m_vecInventory.emplace_back(_eID);
	}
	
}

void CInventory::ChangeWeapon(void)
{
	// ���Ⱑ 1�������� ���� ���� ��ü �ȵ�.
	if (m_vecInventory.size() <= 1)
		return;

	GUN::ID eTemp = m_vecInventory[0];
	GUN::ID eTemp2 = m_vecInventory[1];

	m_eCurWeaponID = eTemp2;

	m_vecInventory[0] = eTemp2;
	m_vecInventory[1] = eTemp;
}
