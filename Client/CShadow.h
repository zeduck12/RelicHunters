#pragma once
#include "CObj.h"

// �׸��ڸ� �׷��ִ� Ŭ����
class CShadow
{
public:
	explicit CShadow() = default;
	virtual ~CShadow() = default;

	// Kamikaze Fly ���� �׸��� �Լ�
	static void RenderFlyShadow(CObj* _pOwner);
	static void RenderPatrolShadow(CObj* _pOwner);
	static void RenderLandingShadow(CObj* _pOwner);

	static void RenderShadow(CObj* _pOwner);
	static void RenderSheetShadow(CObj* _pOwner, const TEXINFO*& _pTexInfo);

private:
	static D3DXVECTOR3 vOldPos;
	static float	   fAddY;
};

