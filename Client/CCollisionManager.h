#pragma once
class CCollisionManager
{
public:
	DECLARE_SINGLETON(CCollisionManager)

public:
	static bool CollideBullet(CObj* _pDstObj, CObj* _SrcObj);				 // Object와 총알충돌 
	static bool CollidePlayerMonster(CObj* _pDstObj, CObj* _SrcObj);		 // 플레이어와 몬스터 충돌
	static bool CollideWallGrenade(CObj* _pDstObj, CObj* _SrcObj);			 // 벽과 수류탄
	static bool CollideLineToLine(LINEINFO& _DstLine, LINEINFO& _SrcLine);	 // 선과 선 충돌
	static bool CollideLineToLineReturn(LINEINFO& _DstLine, LINEINFO& _SrcLine, LINEPOS* _pOut = nullptr);	 // 선과 선 충돌

private:
	CCollisionManager() = default;
	~CCollisionManager();

};

