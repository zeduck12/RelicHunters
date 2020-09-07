#pragma once
class CInventory final
{
public:
	CInventory() = default;
	virtual ~CInventory();

public:
	bool Ready(void);
	void Release(void);

public:
	void SetBombsCount(int _iCount) { m_iOwnBombs = _iCount; if (m_iOwnBombs >= 5) { m_iOwnBombs = 5; } }
	int  GetBombsCount(void) const { return m_iOwnBombs; }
	void SetCoins(int _iCoins) { m_iCoins = _iCoins; }
	int  GetCoins(void) const { return m_iCoins; }
	int  GetOwnBullets(void) const { return m_iOwnBullets; }
	void SetOwnBellets(int _iBullets) { m_iOwnBullets = _iBullets; }
	GUN::ID GetCurWeapon(void) const { return m_eCurWeaponID; }; // ���� ������� ���� ��ȯ
	GUN::ID GetSubWeapon(void) const { return m_vecInventory[1]; }
	void GetSubWeapon(GUN::ID* _eID)
	{
		if (m_vecInventory.size() < 2)
			return;

		*_eID = m_vecInventory[1];
	}
	
public:
	void GainWeapon(GUN::ID _eID);		// ���� ���
	void ChangeWeapon(void);			// ���� �ٲٱ�

private:
	int m_iCapacity;					 // �κ��丮 ũ��
	GUN::ID m_eCurWeaponID;				 // ���� ������� ���� ID
	std::vector<GUN::ID> m_vecInventory; // ���� ��� �κ��丮

	// ���� �Ѿ�, ��ź, ��
	int m_iOwnBombs;
	int m_iOwnBullets;
	int m_iCoins;
};


// ���߿� �κ��丮�� ����ִ� �Ѿ� + �� �̹��� ����ϱ�.
