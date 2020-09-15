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
	list<shared_ptr<CObj>>& GetBullets(void)   { return m_listBullets;  }
	list<shared_ptr<CObj>>& GetGrenades(void)  { return m_listGrenades; }
	list<shared_ptr<CObj>>& GetCasings(void)   { return m_listCasings; }
	list<shared_ptr<CObj>>& GetMonsters(void)  { return m_listMonsters; }
	list<shared_ptr<CObj>>& GetItems(void)     { return m_listItems; }
	list<shared_ptr<CObj>>& GetParticles(void) { return m_listParticles; }

private:
	CObjManager();
	~CObjManager();

private:
	void DrawLine(void);
	void PlayBGM(void);
	void SceneChange(void);
	void InstallTeleporter(void);
	void SpawnKamikazeCage(void);

private:
	list<shared_ptr<CObj>> m_listBullets;
	list<shared_ptr<CObj>> m_listGrenades;
	list<shared_ptr<CObj>> m_listCasings;
	list<shared_ptr<CObj>> m_listMonsters;
	list<shared_ptr<CObj>> m_listItems;
	list<shared_ptr<CObj>> m_listParticles;
	list<shared_ptr<CObj>> m_listHolograms;

private:
	bool m_bIsInstall;
};

