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
	GUN::ID GetCurWeapon(void) const { return m_eCurWeaponID; }; // 현재 사용중인 무기 반환
	GUN::ID GetSubWeapon(void) const { return m_vecInventory[1]; }
	void GetSubWeapon(GUN::ID* _eID)
	{
		if (m_vecInventory.size() < 2)
			return;

		*_eID = m_vecInventory[1];
	}
	
public:
	void GainWeapon(GUN::ID _eID);		// 무기 얻기
	void ChangeWeapon(void);			// 무기 바꾸기

private:
	int m_iCapacity;					 // 인벤토리 크기
	GUN::ID m_eCurWeaponID;				 // 현재 사용중인 무기 ID
	std::vector<GUN::ID> m_vecInventory; // 무기 담는 인벤토리

	// 보유 총알, 폭탄, 돈
	int m_iOwnBombs;
	int m_iOwnBullets;
	int m_iCoins;
};


// 나중에 인벤토리에 들어있는 총알 + 총 이미지 출력하기.
