#pragma once
class CCollisionManager
{
public:
	DECLARE_SINGLETON(CCollisionManager)

public:
	static bool CollideBullet(CObj* _pDstObj, CObj* _SrcObj);				 // Object�� �Ѿ��浹 
	static bool CollidePlasma(CObj* _pDstObj, CObj* _SrcObj);				 // Object�� �Ѿ��浹 
	static bool CollideGrenade(CObj* _pDstObj, CObj* _SrcObj);				 // Object�� �Ѿ��浹 
	static bool CollideTileBullet(TILE* _pTile, CObj* _SrcObj);				 // Tile�� �Ѿ��浹
	static bool CollideTileCasing(TILE* _pTile, CObj* _SrcObj);				 // Tile�� ź���浹
	static bool CollidePlayerMonster(CObj* _pDstObj, CObj* _SrcObj);		 // �÷��̾�� ���� �浹
	static bool CollideWallGrenade(CObj* _pDstObj, CObj* _SrcObj);			 // ���� ����ź
	static bool CollideLineToLine(LINEINFO& _DstLine, LINEINFO& _SrcLine);	 // ���� �� �浹
	static bool CollideLineToLineReturn(LINEINFO& _DstLine, LINEINFO& _SrcLine, LINEPOS* _pOut = nullptr);	 // ���� �� �浹
	static bool CollideCharacterTile(CObj* _pDstObj, TILE* _pTile);			 // ĳ���� Ÿ�� �浹
	static bool CollideCharacterTile(CObj* _pDstObj, TILE* _pTile, RECT* _pOut);
	static bool CollideCharacterStructure(CObj* _pDstObj, CObj* _pSrcObj);	 // ĳ���� ������ �浹
	static bool CollideReflectStructure(CObj* _pDstObj, CObj* _pSrcObj);
	static bool CollideReflectWall(TILE* _pTile, CObj* _SrcObj);

private:
	CCollisionManager() = default;
	~CCollisionManager();

};

