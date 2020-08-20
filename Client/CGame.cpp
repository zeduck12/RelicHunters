#include "pch.h"
#include "CGame.h"
#include "CCollisionManager.h"
#include "CObjManager.h"
#include "CBullet.h"
#include "CPlayer.h"
#include "CMonster.h"

CGame::CGame()
{
}

CGame::~CGame()
{
}

bool CGame::Ready(void)
{
	if (!GET_SINGLE(CObjManager)->Ready())
		return false;

    return true;
}

void CGame::Update(void)
{
	GET_SINGLE(CObjManager)->Update();
}

void CGame::LateUpdate(void)
{
	GET_SINGLE(CObjManager)->LateUpdate();
}

void CGame::Render(const HDC& _hdc)
{
	GET_SINGLE(CObjManager)->Render(_hdc);
}

void CGame::Release(void)
{
	GET_SINGLE(CObjManager)->Release();
	GET_SINGLE(CObjManager)->Destroy_Instance();
	GET_SINGLE(CCollisionManager)->Destroy_Instance();
}
