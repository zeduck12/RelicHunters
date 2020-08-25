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
	void LateUpdate(void);
	void Render(void);
	void Render(D3DXMATRIX _rMat);
	void Release(void);

public:
	bool LoadFile(void);

public:
	vector<TILE*>& GetTiles(void) { return m_vecTile; }
	vector<TILE*>& GetWalls(void) { return m_vecCreateTile; }
	vector<CStructure*>& GetStructures(void) { return m_vecStructure; }

private:
	vector<TILE*>	m_vecTile;			  // �⺻Ÿ��
	vector<TILE*>	m_vecCreateTile;	  // �⺻Ÿ�� ���� �򸮴� Ÿ��
	vector<CStructure*> m_vecStructure;	  // ������Ʈ

};

