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
	int  GetOwnBullets(void) const { return m_iOwnBullets; }
	void SetOwnBellets(int _iBullets) { m_iOwnBullets = _iBullets; }
	GUN::ID GetCurWeapon(void) const { return m_eCurWeaponID; }; // 현재 사용중인 무기 반환
	
public:
	void GainWeapon(GUN::ID _eID);		// 무기 얻기
	void ChangeWeapon(void);			// 무기 바꾸기

private:
	int m_iCapacity;					 // 인벤토리 크기
	GUN::ID m_eCurWeaponID;				 // 현재 사용중인 무기 ID
	std::vector<GUN::ID> m_vecInventory; // 무기 담는 인벤토리

	// 보유 총알
	int m_iOwnBullets;
};


// 나중에 인벤토리에 들어있는 총알 + 총 이미지 출력하기.
