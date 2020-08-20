#pragma once
#include "CObj.h"

class CObjManager
{
public:
	DECLARE_SINGLETON(CObjManager)

public:
	bool Ready(void);
	void Update(void);
	void LateUpdate(void);
	void Render(const HDC& _hdc);
	void Release(void);

public:
	list<shared_ptr<CObj>>& GetBullets(void)  { return m_listBullets;  }
	list<shared_ptr<CObj>>& GetGrenades(void) { return m_listGrenades; }
	list<shared_ptr<CObj>>& GetCasings(void) { return m_listCasings; }
	list<shared_ptr<CObj>>& GetMonsters(void) { return m_listMonsters; }

private:
	CObjManager();
	~CObjManager();

private:
	list<shared_ptr<CObj>> m_listBullets;
	list<shared_ptr<CObj>> m_listGrenades;
	list<shared_ptr<CObj>> m_listCasings;
	list<shared_ptr<CObj>> m_listMonsters;


};

