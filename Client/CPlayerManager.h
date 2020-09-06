#pragma once
#include "CInventory.h"

// �÷��̾�� ���õ� ���� �����ϴ� �Ŵ��� ��ũ��Ʈ
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

