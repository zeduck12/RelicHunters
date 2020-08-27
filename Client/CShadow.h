#pragma once
#include "CObj.h"

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
	static void RenderSheetShadow(CObj* _pOwner, const TEXINFO*& _pTexInfo);

private:
	static D3DXVECTOR3 vOldPos;
	static float	   fAddY;
};

