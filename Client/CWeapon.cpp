#include "pch.h"
#include "CWeapon.h"
#include "CPlayer.h"
#include "CPlayerManager.h"
#include "CBoomerang.h"
#include "CShotGun.h"
#include "CGrenade.h"
#include "CObjManager.h"
#include "CKeyManager.h"
#include "CMonster.h"
#include "CCasing.h"
#include "CInventory.h"
#include "CCameraManager.h"

CWeapon::~CWeapon()
{
    Release();
}

void CWeapon::Ready(void)
{
    m_eCurWeaponID = GUN::DEFAULT;

	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 1.0f, 0.f, 0.f };
	m_tInfo.vSize = { 100.f, 50.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	// 회전할 버텍스 좌표
	m_vRotVertex[0].x = -(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[0].y = -(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[1].x = +(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[1].y = -(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[2].x = +(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[2].y = +(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[3].x = -(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[3].y = +(m_tInfo.vSize.y * 0.5f);

}

int CWeapon::Update(float _fDeltaTime)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
	DO_IF_IS_NOT_VALID_OBJ(pObj) { return 0; }
	
	D3DXMATRIX matWorld, matScale, matRotz, matMove, matRev, matParent, matOldScale;
	
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	// 마우스 좌표
	D3DXVECTOR3 vMouse = { float(pt.x) + pPlayer->GetX() - (WINCX >> 1), float(pt.y) + pPlayer->GetY() - (WINCY >> 1), 0.f };

	// 방향에 따라 스케일 반전
	if (vMouse.x < pPlayer->GetX())
	{
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matMove, 50.f, -25.f, 0.f);
	}
	else if (vMouse.x > pPlayer->GetX())
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matMove, 50.f, 25.f, 0.f);
	}

	D3DXMatrixRotationZ(&matRev, D3DXToRadian(pPlayer->GetShootingDegree()));
	D3DXMatrixTranslation(&matParent, pPlayer->GetX(), pPlayer->GetY(), 0.f);

	matWorld = matScale * matMove * matRev * matParent;

	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&m_vRealVertex[i], &m_vRotVertex[i], &matWorld);

    return 0;
}

void CWeapon::LateUpdate(void)
{
}

void CWeapon::Release(void)
{
}

void CWeapon::Render(const HDC& _hdc)
{
	// 총 아이디에 따라 총 이미지 달라지게
	m_eCurWeaponID = GET_SINGLE(CPlayerManager)->GetInventory()->GetCurWeapon();
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer) { return; }


	MoveToEx(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y, nullptr);

	for (int i = 1; i < 4; i++)
		LineTo(_hdc, (int)m_vRealVertex[i].x, (int)m_vRealVertex[i].y);
	LineTo(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y);
}

void CWeapon::Shoot(void)
{
	m_eCurWeaponID = GET_SINGLE(CPlayerManager)->GetInventory()->GetCurWeapon();
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer) { return; }

	switch (m_eCurWeaponID)
	{
	case GUN::DEFAULT:
		ShootDefault();
		break;
	case GUN::SHOTGUN:
		ShootShotGun();
		break;
	case GUN::BOOMERANG:
		ShootBoomerang();
		break;
	}
}

void CWeapon::ShootDefault(void)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pObj) { return; }
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

	// Bullet 발사
	shared_ptr<CBullet> pBullet 
		= make_shared<CBullet>(pPlayer->GetX() + pPlayer->GetDirectionVector().x * 100.f,
		pPlayer->GetY() + pPlayer->GetDirectionVector().y * 100.f,
		pPlayer->GetDirectionVector(), cfDefaultBulletSpeed, pPlayer->GetShootingDegree());
	pBullet->Ready();
	GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBullet);
	GET_SINGLE(CCameraManager)->SetIsShooting(true);

	// 탄피 생성
	shared_ptr<CCasing> pCasing = make_shared<CCasing>(pPlayer->GetX(),
		pPlayer->GetY(), pPlayer->GetDirectionVector(), cfCasingSpeed, pPlayer->GetShootingDegree());
	pCasing->Ready();
	GET_SINGLE(CObjManager)->GetCasings().emplace_back(pCasing);
}

void CWeapon::ShootShotGun(void)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pObj) { return; }
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

	// Shot Gun 발사
	shared_ptr<CBullet> pShotGun = nullptr;
	for (int i = 0; i < 3; i++)
	{
		pShotGun = make_shared<CShotGun>(pPlayer->GetX() + pPlayer->GetDirectionVector().x * 100.f,
			pPlayer->GetY() + pPlayer->GetDirectionVector().y * 100.f,
			pPlayer->GetDirectionVector(), -7.f * (i - 1), cfDefaultBulletSpeed, pPlayer->GetShootingDegree());
		pShotGun->Ready();
		GET_SINGLE(CObjManager)->GetBullets().emplace_back(pShotGun);
	}
	GET_SINGLE(CCameraManager)->SetIsShooting(true);


	// 탄피 생성
	shared_ptr<CCasing> pCasing = nullptr;
	for (int i = 0; i < 3; i++)
	{
		pCasing = make_shared<CCasing>(pPlayer->GetX(),
			pPlayer->GetY(), pPlayer->GetDirectionVector(), cfCasingSpeed,
			pPlayer->GetShootingDegree());
		pCasing->Ready();
		GET_SINGLE(CObjManager)->GetCasings().emplace_back(pCasing);
	}
}

void CWeapon::ShootBoomerang(void)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pObj) { return; }
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

	// 부메랑 발사
	shared_ptr<CBullet> pBoomerang = make_shared<CBoomerang>(pPlayer->GetX() + pPlayer->GetDirectionVector().x * 100.f,
		pPlayer->GetY() + pPlayer->GetDirectionVector().y * 100.f,
		pPlayer->GetDirectionVector());
	pBoomerang->Ready();
	GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBoomerang);
	GET_SINGLE(CCameraManager)->SetIsShooting(true);

	// 탄피 생성
	shared_ptr<CCasing> pCasing = make_shared<CCasing>(pPlayer->GetX(),
		pPlayer->GetY(), pPlayer->GetDirectionVector(), cfCasingSpeed, pPlayer->GetShootingDegree());
	pCasing->Ready();
	GET_SINGLE(CObjManager)->GetCasings().emplace_back(pCasing);
}
