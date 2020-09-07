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
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CPlasma.h"
#include "CReflect.h"

CWeapon::~CWeapon()
{
    Release();
}

void CWeapon::Ready(void)
{
    m_eCurWeaponID = GUN::DEFAULT;
	m_iCurCapacity	   = 50;
	m_iReloadedBullets = 50;

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

bool CWeapon::GetWorldMatrix(D3DXMATRIX* _pOutMatrix)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer) { return false; }

	CPlayer* pRealPlayer = dynamic_cast<CPlayer*>(pPlayer);
	DO_IF_IS_NOT_VALID_OBJ(pPlayer) { return false; }

	D3DXMATRIX matWorld, matScale, matRotz, matMove, matRev, matParent, matOldScale;

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	// 마우스 좌표
	D3DXVECTOR3 vMouse = { float(pt.x) + pPlayer->GetX() - (WINCX >> 1), float(pt.y) + pPlayer->GetY() - (WINCY >> 1), 0.f };

	// 방향에 따라 스케일 반전
	if (vMouse.x < pPlayer->GetX())
	{
		D3DXMatrixScaling(&matScale, 1.f, -1.f, 1.f);
		D3DXMatrixTranslation(&matMove, 15.f, -5.f, 0.f);
	}
	else if (vMouse.x > pPlayer->GetX())
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matMove, 15.f, 5.f, 0.f);
	}

	D3DXMatrixRotationZ(&matRev, D3DXToRadian(pRealPlayer->GetShootingDegree()));
	D3DXMatrixTranslation(&matParent, pPlayer->GetX(), pPlayer->GetY(), 0.f);

	matWorld = matScale * matMove * matRev * matParent;
	*_pOutMatrix = matWorld;

	return true;
}

void CWeapon::Render(const HDC& _hdc)
{
	// 총 아이디에 따라 총 이미지 달라지게
	m_eCurWeaponID = GET_SINGLE(CPlayerManager)->GetInventory()->GetCurWeapon();
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer) { return; }


	//MoveToEx(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y, nullptr);

	//for (int i = 1; i < 4; i++)
	//	LineTo(_hdc, (int)m_vRealVertex[i].x, (int)m_vRealVertex[i].y);
	//LineTo(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y);

	// 현재 장착총 그리기
	DrawCurGun();

}

//FLAME,				// 몬스터   기본총
//ASSAULT,			    // 3발 연속으로 나가는 총
//KEYTAR,				// 2발 연속으로 나가는 총
//PISTOL_ASSUALT,		// 연속발사 되는 총 (총알만 다름
//PISTOL_HEAVY,		    // 데미지 쌘 연속발사 총
//PLASMA,				// 플라즈마 볼 총

void CWeapon::Shoot(void)
{

	// 현재 장전된 총알이 없으면 발사 X Return
	if (m_iReloadedBullets <= 0)
	{
		m_iReloadedBullets = 0;
		return;
	}

	m_iReloadedBullets--;  // 총쏘면 장전된 총알  -1
	m_eCurWeaponID = GET_SINGLE(CPlayerManager)->GetInventory()->GetCurWeapon();
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer) { return; }

	CPlayer* pRealPlayer = dynamic_cast<CPlayer*>(pPlayer);
	if (pRealPlayer->IsReloading() == true)  // 장전 중이면 총 못쏘게
		return;

	switch (m_eCurWeaponID)
	{
	case GUN::DEFAULT:
		ShootDefault();
		break;
	case GUN::SHOTGUN:
		ShootShotGun();
		break;
	case GUN::HIGH_MAG:
		ShootBoomerang();
		break;
	case GUN::FLAME:
		ShootFlame();
		break;
	case GUN::ASSAULT:
		ShootAssault();
		break;
	case GUN::KEYTAR: // 나중에 음표나가게
		ShootKeytar();
		break;
	case GUN::PISTOL_ASSUALT:
		ShootPistolAssault();
		break;
	case GUN::PISTOL_HEAVY:
		ShootPistolHeavy();
		break;
	case GUN::PLASMA:
		ShootPlasma();
		break;
	case GUN::SNIPER:
		ShootSniper();
		break;
	case GUN::MACHINEGUN:
		ShootDefault();
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
		= make_shared<CBullet>(pPlayer->GetX() + pPlayer->GetDirectionVector().x * 10.f,
		pPlayer->GetY() + pPlayer->GetDirectionVector().y * 10.f,
		pPlayer->GetDirectionVector(), cfDefaultBulletSpeed, pPlayer->GetShootingDegree(), OBJ::PLAYER, L"Small", 200.f);
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
		pShotGun = make_shared<CShotGun>(pPlayer->GetX() + pPlayer->GetDirectionVector().x * 10.f,
			pPlayer->GetY() + pPlayer->GetDirectionVector().y * 10.f,
			pPlayer->GetDirectionVector(), -7.f * (i - 1), cfDefaultBulletSpeed, pPlayer->GetShootingDegree(), OBJ::PLAYER, L"Blue");
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
			pPlayer->GetShootingDegree(), L"CasingHeavy");
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
	shared_ptr<CBullet> pBoomerang = make_shared<CBoomerang>(pPlayer->GetX() + pPlayer->GetDirectionVector().x * 20.f,
		pPlayer->GetY() + pPlayer->GetDirectionVector().y * 20.f,
		pPlayer->GetDirectionVector(), cfBoomerangBulletSpeed, OBJ::PLAYER, L"Plasma");
	pBoomerang->Ready();
	GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBoomerang);
	GET_SINGLE(CCameraManager)->SetIsShooting(true);

	// 탄피 생성
	shared_ptr<CCasing> pCasing = make_shared<CCasing>(pPlayer->GetX(),
		pPlayer->GetY(), pPlayer->GetDirectionVector(), cfCasingSpeed, pPlayer->GetShootingDegree());
	pCasing->Ready();
	GET_SINGLE(CObjManager)->GetCasings().emplace_back(pCasing);
}

void CWeapon::ShootSniper(void)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pObj) { return; }
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

	// Bullet 발사
	shared_ptr<CBullet> pBullet
		= make_shared<CBullet>(pPlayer->GetX() + pPlayer->GetDirectionVector().x * 10.f,
			pPlayer->GetY() + pPlayer->GetDirectionVector().y * 10.f,
			pPlayer->GetDirectionVector(), 60.f, pPlayer->GetShootingDegree(), OBJ::PLAYER, L"Small", 9999.f);
	pBullet->Ready();
	GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBullet);
	GET_SINGLE(CCameraManager)->SetIsShooting(true);

	// 탄피 생성
	shared_ptr<CCasing> pCasing = make_shared<CCasing>(pPlayer->GetX(),
		pPlayer->GetY(), pPlayer->GetDirectionVector(), cfCasingSpeed, pPlayer->GetShootingDegree(), L"CasingHeavy");
	pCasing->Ready();
	GET_SINGLE(CObjManager)->GetCasings().emplace_back(pCasing);
}

void CWeapon::ShootFlame(void)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pObj) { return; }
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

	// Bullet 발사
	shared_ptr<CBullet> pBullet
		= make_shared<CBullet>(pPlayer->GetX() + pPlayer->GetDirectionVector().x * 10.f,
			pPlayer->GetY() + pPlayer->GetDirectionVector().y * 10.f,
			pPlayer->GetDirectionVector(), 40.f, pPlayer->GetShootingDegree(), OBJ::PLAYER, L"Blue", 15.f);
	pBullet->Ready();
	GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBullet);
	GET_SINGLE(CCameraManager)->SetIsShooting(true);

	// 탄피 생성
	shared_ptr<CCasing> pCasing = make_shared<CCasing>(pPlayer->GetX(),
		pPlayer->GetY(), pPlayer->GetDirectionVector(), cfCasingSpeed, pPlayer->GetShootingDegree());
	pCasing->Ready();
	GET_SINGLE(CObjManager)->GetCasings().emplace_back(pCasing);
}

void CWeapon::ShootAssault(void)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pObj) { return; }
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

	// Bullet 발사
	shared_ptr<CBullet> pBullet
		= make_shared<CReflect>(pPlayer->GetX() + pPlayer->GetDirectionVector().x * 10.f,
			pPlayer->GetY() + pPlayer->GetDirectionVector().y * 10.f,
			pPlayer->GetDirectionVector(), 5.f, pPlayer->GetShootingDegree(), OBJ::PLAYER, 1.f);
	pBullet->Ready();
	GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBullet);
	GET_SINGLE(CCameraManager)->SetIsShooting(true);

	// 탄피 생성
	shared_ptr<CCasing> pCasing = make_shared<CCasing>(pPlayer->GetX(),
		pPlayer->GetY(), pPlayer->GetDirectionVector(), cfCasingSpeed, pPlayer->GetShootingDegree());
	pCasing->Ready();
	GET_SINGLE(CObjManager)->GetCasings().emplace_back(pCasing);
}

void CWeapon::ShootPistolAssault(void)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pObj) { return; }
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

	// Bullet 발사
	shared_ptr<CBullet> pBullet
		= make_shared<CBullet>(pPlayer->GetX() + pPlayer->GetDirectionVector().x * 10.f,
			pPlayer->GetY() + pPlayer->GetDirectionVector().y * 10.f,
			pPlayer->GetDirectionVector(), 35.f, pPlayer->GetShootingDegree(), OBJ::PLAYER, L"Plasma", 9.f);
	pBullet->Ready();
	GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBullet);
	GET_SINGLE(CCameraManager)->SetIsShooting(true);

	// 탄피 생성
	shared_ptr<CCasing> pCasing = make_shared<CCasing>(pPlayer->GetX(),
		pPlayer->GetY(), pPlayer->GetDirectionVector(), cfCasingSpeed, pPlayer->GetShootingDegree());
	pCasing->Ready();
	GET_SINGLE(CObjManager)->GetCasings().emplace_back(pCasing);
}

void CWeapon::ShootPistolHeavy(void)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pObj) { return; }
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

	// Bullet 발사
	shared_ptr<CBullet> pBullet
		= make_shared<CBullet>(pPlayer->GetX() + pPlayer->GetDirectionVector().x * 10.f,
			pPlayer->GetY() + pPlayer->GetDirectionVector().y * 10.f,
			pPlayer->GetDirectionVector(), 15.f, pPlayer->GetShootingDegree(), OBJ::PLAYER, L"Plasma", 20.f);
	pBullet->Ready();
	GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBullet);
	GET_SINGLE(CCameraManager)->SetIsShooting(true);

	// 탄피 생성
	shared_ptr<CCasing> pCasing = make_shared<CCasing>(pPlayer->GetX(),
		pPlayer->GetY(), pPlayer->GetDirectionVector(), cfCasingSpeed, pPlayer->GetShootingDegree(), L"CasingMiddle");
	pCasing->Ready();
	GET_SINGLE(CObjManager)->GetCasings().emplace_back(pCasing);
}

void CWeapon::ShootKeytar(void)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pObj) { return; }
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

	// Bullet 발사
	shared_ptr<CBullet> pBullet
		= make_shared<CBullet>(pPlayer->GetX() + pPlayer->GetDirectionVector().x * 10.f,
			pPlayer->GetY() + pPlayer->GetDirectionVector().y * 10.f,
			pPlayer->GetDirectionVector(), 15.f, pPlayer->GetShootingDegree(), OBJ::PLAYER, L"SonicBoom", 40.f);
	pBullet->Ready();
	GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBullet);
	GET_SINGLE(CCameraManager)->SetIsShooting(true);

	// 탄피 생성
	shared_ptr<CCasing> pCasing = make_shared<CCasing>(pPlayer->GetX(),
		pPlayer->GetY(), pPlayer->GetDirectionVector(), cfCasingSpeed, pPlayer->GetShootingDegree(), L"CasingMiddle");
	pCasing->Ready();
	GET_SINGLE(CObjManager)->GetCasings().emplace_back(pCasing);
}

void CWeapon::ShootPlasma(void)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pObj) { return; }
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

	// Bullet 발사
	shared_ptr<CBullet> pBullet
		= make_shared<CPlasma>(pPlayer->GetX() + pPlayer->GetDirectionVector().x * 10.f,
			pPlayer->GetY() + pPlayer->GetDirectionVector().y * 10.f,
			pPlayer->GetDirectionVector(), 5.f, pPlayer->GetShootingDegree(), OBJ::PLAYER,  1.f);
	pBullet->Ready();
	GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBullet);
	GET_SINGLE(CCameraManager)->SetIsShooting(true);

	// 탄피 생성
	shared_ptr<CCasing> pCasing = make_shared<CCasing>(pPlayer->GetX(),
		pPlayer->GetY(), pPlayer->GetDirectionVector(), cfCasingSpeed, pPlayer->GetShootingDegree());
	pCasing->Ready();
	GET_SINGLE(CObjManager)->GetCasings().emplace_back(pCasing);
}

void CWeapon::DrawCurGun(void)
{
	// 총 그림 회전
	if (m_eCurWeaponID == GUN::DEFAULT)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Pistol", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		if (!GetWorldMatrix(&matWorld))
			return;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_eCurWeaponID == GUN::SHOTGUN)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Shotgun", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		if (!GetWorldMatrix(&matWorld))
			return;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_eCurWeaponID == GUN::HIGH_MAG)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"HighMag", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		if (!GetWorldMatrix(&matWorld))
			return;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_eCurWeaponID == GUN::SNIPER)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Sniper", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		if (!GetWorldMatrix(&matWorld))
			return;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_eCurWeaponID == GUN::MACHINEGUN)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"MachineGun", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		if (!GetWorldMatrix(&matWorld))
			return;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_eCurWeaponID == GUN::FLAME)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Flame", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		if (!GetWorldMatrix(&matWorld))
			return;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_eCurWeaponID == GUN::ASSAULT)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Assault", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		if (!GetWorldMatrix(&matWorld))
			return;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_eCurWeaponID == GUN::KEYTAR)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Keytar", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		if (!GetWorldMatrix(&matWorld))
			return;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_eCurWeaponID == GUN::PISTOL_ASSUALT)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Pistol_Assualt", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		if (!GetWorldMatrix(&matWorld))
			return;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	
	if (m_eCurWeaponID == GUN::PISTOL_HEAVY)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Pistol_Heavy", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		if (!GetWorldMatrix(&matWorld))
			return;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	
	if (m_eCurWeaponID == GUN::PLASMA)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Plasma", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		if (!GetWorldMatrix(&matWorld))
			return;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CWeapon::DrawSubGun(void)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pObj) { return ; }

	if (GET_SINGLE(CPlayerManager)->GetInventory()->GetSubWeapon() == GUN::DEFAULT)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Pistol", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld, matScale, matRotZ, matTrans;
		
		if (pObj->GetDirection() == DIRECTION::LEFT)
		{
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(130.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() + 20.f, pObj->GetY() - 20.f, 0.f);
		}
		else
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(230.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() - 20.f, pObj->GetY() - 20.f, 0.f);
		}

		matWorld = matScale * matRotZ * matTrans;
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (GET_SINGLE(CPlayerManager)->GetInventory()->GetSubWeapon() == GUN::SHOTGUN)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Shotgun", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld, matScale, matRotZ, matTrans;

		if (pObj->GetDirection() == DIRECTION::LEFT)
		{
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(130.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() + 20.f, pObj->GetY() - 20.f, 0.f);
		}
		else
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(230.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() - 20.f, pObj->GetY() - 20.f, 0.f);
		}

		matWorld = matScale * matRotZ * matTrans;
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (GET_SINGLE(CPlayerManager)->GetInventory()->GetSubWeapon() == GUN::HIGH_MAG)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"HighMag", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld, matScale, matRotZ, matTrans;

		if (pObj->GetDirection() == DIRECTION::LEFT)
		{
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(130.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() + 20.f, pObj->GetY() - 20.f, 0.f);
		}
		else
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(230.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() - 20.f, pObj->GetY() - 20.f, 0.f);
		}

		matWorld = matScale * matRotZ * matTrans;
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (GET_SINGLE(CPlayerManager)->GetInventory()->GetSubWeapon() == GUN::SNIPER)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Sniper", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld, matScale, matRotZ, matTrans;

		if (pObj->GetDirection() == DIRECTION::LEFT)
		{
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(130.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() + 20.f, pObj->GetY() - 20.f, 0.f);
		}
		else
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(230.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() - 20.f, pObj->GetY() - 20.f, 0.f);
		}

		matWorld = matScale * matRotZ * matTrans;
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (GET_SINGLE(CPlayerManager)->GetInventory()->GetSubWeapon() == GUN::MACHINEGUN)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"MachineGun", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld, matScale, matRotZ, matTrans;

		if (pObj->GetDirection() == DIRECTION::LEFT)
		{
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(130.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() + 20.f, pObj->GetY() - 20.f, 0.f);
		}
		else
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(230.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() - 20.f, pObj->GetY() - 20.f, 0.f);
		}

		matWorld = matScale * matRotZ * matTrans;
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (GET_SINGLE(CPlayerManager)->GetInventory()->GetSubWeapon() == GUN::FLAME)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Flame", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld, matScale, matRotZ, matTrans;

		if (pObj->GetDirection() == DIRECTION::LEFT)
		{
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(130.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() + 20.f, pObj->GetY() - 20.f, 0.f);
		}
		else
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(230.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() - 20.f, pObj->GetY() - 20.f, 0.f);
		}

		matWorld = matScale * matRotZ * matTrans;
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (GET_SINGLE(CPlayerManager)->GetInventory()->GetSubWeapon() == GUN::ASSAULT)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Assault", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld, matScale, matRotZ, matTrans;

		if (pObj->GetDirection() == DIRECTION::LEFT)
		{
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(130.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() + 20.f, pObj->GetY() - 20.f, 0.f);
		}
		else
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(230.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() - 20.f, pObj->GetY() - 20.f, 0.f);
		}

		matWorld = matScale * matRotZ * matTrans;
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (GET_SINGLE(CPlayerManager)->GetInventory()->GetSubWeapon() == GUN::KEYTAR)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Keytar", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld, matScale, matRotZ, matTrans;

		if (pObj->GetDirection() == DIRECTION::LEFT)
		{
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(130.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() + 20.f, pObj->GetY() - 20.f, 0.f);
		}
		else
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(230.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() - 20.f, pObj->GetY() - 20.f, 0.f);
		}

		matWorld = matScale * matRotZ * matTrans;
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (GET_SINGLE(CPlayerManager)->GetInventory()->GetSubWeapon() == GUN::PISTOL_ASSUALT)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Pistol_Assualt", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld, matScale, matRotZ, matTrans;

		if (pObj->GetDirection() == DIRECTION::LEFT)
		{
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(130.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() + 20.f, pObj->GetY() - 20.f, 0.f);
		}
		else
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(230.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() - 20.f, pObj->GetY() - 20.f, 0.f);
		}

		matWorld = matScale * matRotZ * matTrans;
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (GET_SINGLE(CPlayerManager)->GetInventory()->GetSubWeapon() == GUN::PISTOL_HEAVY)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Pistol_Heavy", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld, matScale, matRotZ, matTrans;

		if (pObj->GetDirection() == DIRECTION::LEFT)
		{
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(130.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() + 20.f, pObj->GetY() - 20.f, 0.f);
		}
		else
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(230.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() - 20.f, pObj->GetY() - 20.f, 0.f);
		}

		matWorld = matScale * matRotZ * matTrans;
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (GET_SINGLE(CPlayerManager)->GetInventory()->GetSubWeapon() == GUN::PLASMA)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Plasma", 0);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matWorld, matScale, matRotZ, matTrans;

		if (pObj->GetDirection() == DIRECTION::LEFT)
		{
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(130.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() + 20.f, pObj->GetY() - 20.f, 0.f);
		}
		else
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(230.f));
			D3DXMatrixTranslation(&matTrans, pObj->GetX() - 20.f, pObj->GetY() - 20.f, 0.f);
		}

		matWorld = matScale * matRotZ * matTrans;
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

bool CWeapon::ReloadBullets(void)
{
	// 인벤토리에서 필요한 총알만큼 가져와서 장전할 수 있는 만큼해준다.
	// m_iCurCapacity;     // 현재 총의 탄창크기    
	// m_iReloadedBullets; // 현재 장전된 총알 갯수

	// 소유하고 있는 총알 갯수
	int iOwnBullets = GET_SINGLE(CPlayerManager)->GetInventory()->GetOwnBullets();
	int iDelta = m_iCurCapacity - m_iReloadedBullets; // 장전해야할 총알의 갯수
	if (iDelta <= 0)  // 장전해야할 총알 수가 0이면 return;
		return false;

	// 만약에 소유하고 있는 총알보다 장전해야할 총알의 갯수가 크다면
	if (m_iCurCapacity > iOwnBullets)
	{
		m_iReloadedBullets += iOwnBullets;
		iDelta = iOwnBullets;
	}
	else
		m_iReloadedBullets += iDelta;

	// 사용한 총알 갯수 빼주기
	GET_SINGLE(CPlayerManager)->GetInventory()->SetOwnBellets(iOwnBullets - iDelta);


	return true;
}

