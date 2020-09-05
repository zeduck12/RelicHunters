#pragma once
class CCollisionManager
{
public:
	DECLARE_SINGLETON(CCollisionManager)

public:
	static bool CollideBullet(CObj* _pDstObj, CObj* _SrcObj);				 // Object와 총알충돌 
	static bool CollideGrenade(CObj* _pDstObj, CObj* _SrcObj);				 // Object와 총알충돌 
	static bool CollideTileBullet(TILE* _pTile, CObj* _SrcObj);				 // Tile관 총알충돌
	static bool CollideTileCasing(TILE* _pTile, CObj* _SrcObj);				 // Tile과 탄피충돌
	static bool CollideTileGrenade(TILE* _pTile, CObj* _SrcObj);			 // Tile과 수류탄충돌
	static bool CollidePlayerMonster(CObj* _pDstObj, CObj* _SrcObj);		 // 플레이어와 몬스터 충돌
	static bool CollideWallGrenade(CObj* _pDstObj, CObj* _SrcObj);			 // 벽과 수류탄
	static bool CollideLineToLine(LINEINFO& _DstLine, LINEINFO& _SrcLine);	 // 선과 선 충돌
	static bool CollideLineToLineReturn(LINEINFO& _DstLine, LINEINFO& _SrcLine, LINEPOS* _pOut = nullptr);	 // 선과 선 충돌
	static bool CollideCharacterTile(CObj* _pDstObj, TILE* _pTile);			 // 캐릭터 타일 충돌
	static bool CollideCharacterTile(CObj* _pDstObj, TILE* _pTile, RECT* _pOut);
	static bool CollideCharacterStructure(CObj* _pDstObj, CObj* _pSrcObj);	 // 캐릭터 구조물 충돌


private:
	CCollisionManager() = default;
	~CCollisionManager();

};

