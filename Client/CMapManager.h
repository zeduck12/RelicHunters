#pragma once

class CStructure;
class CMapManager
{
public:
	DECLARE_SINGLETON(CMapManager)

private:
	CMapManager() = default;
	~CMapManager();

public:
	bool Ready(void);
	void Update(void);
	void LateUpdate(void);
	void Render(const HDC& _hdc);
	void Release(void);

public:
	bool LoadFile(void);
	void CheckPlayerShadow(void);

public:
	list<TILE*>& GetTiles(void) { return m_vecTile; }
	list<TILE*>& GetWalls(void) { return m_vecCreateTile; }
	list<shared_ptr<CObj>>& GetStructures(void) { return m_listStructure; }
	list<shared_ptr<CObj>>& GetItems(void) { return m_listItems; }

private:
	list<TILE*>	m_vecTile;			  // �⺻Ÿ��
	list<TILE*>	m_vecCreateTile;	  // �⺻Ÿ�� ���� �򸮴� Ÿ��
	list<shared_ptr<CObj>> m_listStructure;
	list<shared_ptr<CObj>> m_listItems;

};

