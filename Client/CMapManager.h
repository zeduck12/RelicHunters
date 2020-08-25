#pragma once
class CMapManager
{
public:
	DECLARE_SINGLETON(CMapManager)

private:
	CMapManager() = default;
	~CMapManager();

public:
	bool Ready(void);
	void Render(void);
	void Render(D3DXMATRIX _rMat);
	void Release(void);

public:
	bool LoadFile(void);

private:
	vector<TILE*>	m_vecTile;		// 기본타일
	vector<TILE*>	m_vecCreateTile; // 기본타일 위에 깔리는 타일

};

