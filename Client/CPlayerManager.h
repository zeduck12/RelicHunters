#pragma once
#include "CInventory.h"

// 플레이어와 관련된 모든걸 관리하는 매니저 스크립트
class CPlayerManager
{
	DECLARE_SINGLETON(CPlayerManager)

public:
	bool Ready(void);
	void Update(void);
	void LateUpdate(void);
	void Render(const HDC& _hdc);
	void Release(void);

public:
	bool  CreatePlayer(void);
	bool  CreateInventory(void);

public:
	CObj* GetPlayer(void) const { return m_pPlayer.get(); }
	CInventory* GetInventory(void) const { return m_pInventory.get();}

private:
	CPlayerManager();
	~CPlayerManager();

private:
	unique_ptr<CObj> m_pPlayer;
	unique_ptr<CInventory> m_pInventory;
};

