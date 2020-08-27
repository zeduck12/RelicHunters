#include "pch.h"
#include "CBullet.h"
#include "CCollisionManager.h"
#include "CObjManager.h"
#include "CPlayerManager.h"
#include "CCameraManager.h"
#include "CMapManager.h"
#include "CStructure.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CPlayer.h"
#include "CPlayerState.h"
#include "CMonster.h"
#include "CMonsterState.h"
#include "CShadow.h"

CBullet::CBullet(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed, float _fShootingDegree, OBJ::ID _eID)
	:
	m_eObjID{ _eID }
{
	m_fDegree = _fShootingDegree;
	m_fSpeed = _fSpeed;

	m_tInfo.vPos = { _fX , _fY , 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { cfDefaultBulletSizeX, cfDefaultBulletSizeY, 0.f };

	// 회전할 버텍스 좌표
	m_vRotVertex[0].x = -(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[0].y = -(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[1].x = +(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[1].y = -(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[2].x = +(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[2].y = +(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[3].x = -(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[3].y = +(m_tInfo.vSize.y * 0.5f);

	m_vDir = _vDir;

	float fAddAngle = 0.f;
	if (GET_SINGLE(CCameraManager)->IsPressing() == false)
		fAddAngle = GetNumberMinBetweenMax(-7.f,7.f);

	D3DXMATRIX matWorld, matRotZ;
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(fAddAngle));
	matWorld = matRotZ;
	D3DXVec3TransformCoord(&m_vDir, &m_vDir, &matWorld);

}

CBullet::~CBullet()
{
	Release();
}

void CBullet::Ready()
{

}

int CBullet::Update(float _fDeltaTime)
{
	D3DXMATRIX matWorld, matRev, matParent;

	D3DXMatrixRotationZ(&matRev, D3DXToRadian(m_fDegree));
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	matWorld = matRev * matParent;

	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&m_vRealVertex[i], &m_vRotVertex[i], &matWorld);

	m_tInfo.vPos += m_vDir * m_fSpeed;

	return 0;
}

void CBullet::LateUpdate()
{
	if (m_eObjID == OBJ::PLAYER)
	{
		for (auto& pMonster : GET_SINGLE(CObjManager)->GetMonsters())
		{
			// 몬스터가 하늘을 나는 중이면 충돌판정 PASS !
			if (dynamic_cast<CMonster*>(pMonster.get())->IsFlying() == true)
				continue;

			if (CCollisionManager::CollideBullet(pMonster.get(), this) == true)
			{
				CMonster* pMonst = dynamic_cast<CMonster*>(pMonster.get());
				pMonst->SetState(new AttackedState());
			}
		}
	}
	
	if (m_eObjID == OBJ::MONSTER)
	{
		CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
		if (CCollisionManager::CollideBullet(pObj, this) == true)
		{
			// 피격 애니메이션
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
			pPlayer->SetState(GET_SINGLE(PlayerAttacked));
			pPlayer->SetIsAttacked(true);
		}
	}

	// 공통
	for (auto& pObj : GET_SINGLE(CMapManager)->GetStructures())
		CCollisionManager::CollideBullet(pObj, this);

	for(auto& pTile : GET_SINGLE(CMapManager)->GetWalls())
		CCollisionManager::CollideTileBullet(pTile, this);


}

void CBullet::Render(const HDC& _hdc)
{
	MoveToEx(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y, nullptr);

	for (int i = 1; i < 4; i++)
		LineTo(_hdc, (int)m_vRealVertex[i].x, (int)m_vRealVertex[i].y);
	LineTo(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y);


	// Bullet 그리기
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Bullet", L"Small", 1);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matWorld, matScale, matRev, matParent;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRev, D3DXToRadian(m_fDegree));
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	matWorld = matScale * matRev * matParent;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderSheetProjectile(this, pTexInfo, m_fDegree);
}

void CBullet::Release()
{
}
