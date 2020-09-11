#pragma once
#include "CObj.h"
#include "CGrenade.h"

// 그림자만 그려주는 클래스
class CShadow
{
public:
	explicit CShadow() = default;
	virtual ~CShadow() = default;

	// Kamikaze Fly 전용 그림자 함수
	static void RenderFlyShadow(CObj* _pOwner);
	static void RenderPatrolShadow(CObj* _pOwner);
	static void RenderLandingShadow(CObj* _pOwner);

	static void RenderShadow(CObj* _pOwner);
	// 낱장 이미지 그림자
	static void RenderSheetShadow(CObj* _pOwner, const TEXINFO*& _pTexInfo);
	static void RenderSheetProjectile(CObj* _pOwner, const TEXINFO*& _pTexInfo, const float& _fDegree);
	// 탄피 그림자
	static void RenderCasing(CObj* _pOwner);
	static void RenderParticle(CObj* _pOwner);
	static void RenderShadowPlayer(CObj* _pOwner);
private:
	static D3DXVECTOR3 vOldPos;
	static float	   fAddY;
};

