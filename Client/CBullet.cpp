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
#include "CBoss.h"
#include "CBossState.h"
#include "CStructure.h"
#include "CShield.h"
#include "CParticle.h"
#include "CTimeManager.h"
#include "CHitParticle.h"

CBullet::CBullet(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed, float _fShootingDegree,
	OBJ::ID _eID, const wstring& _strBulletName /*= L"Small"*/,float _fDamage/* = 10.f*/)
	:
	m_eObjID{ _eID },
	m_strBulletName{ _strBulletName },
	m_fDamage{ _fDamage }
{
	m_fDegree = _fShootingDegree;
	m_fSpeed = _fSpeed;
	m_fStacktime = 0.f;

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
	{
		if(m_eObjID == OBJ::MONSTER)
			fAddAngle = GetNumberMinBetweenMax(-10.f,10.f);
			//보스는 40으로 고려해보기
		else
			fAddAngle = GetNumberMinBetweenMax(-7.f, 7.f);

	}

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
	if (m_eObjID == OBJ::MONSTER)
	{
		m_fStacktime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fStacktime >= 2.f)
			this->SetIsValid(false);

		if (m_fStacktime >= 1.5f)
			m_iDelta += 10;
	}

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
				if (pMonster->GetImageID() == IMAGE::BOSS)
				{
					CBoss* pBoss = dynamic_cast<CBoss*>(pMonster.get());
					if (pBoss->IsDead() == false && pBoss->IsCrack() == true && pBoss->IsInvicible() == false)
					{
						pBoss->SetState(new BossAttackedState);
						pBoss->SetHp(pBoss->GetHp() - m_fDamage);
					}
				}
				else
				{
					CMonster* pMonst = dynamic_cast<CMonster*>(pMonster.get());
					if (pMonst->IsDead() == false)
					{
						pMonst->SetState(new AttackedState());
						pMonst->SetHp(pMonst->GetHp() - m_fDamage);

					}
				}

				shared_ptr<CObj> pParticle = make_shared<CParticle>(pMonster->GetX(), pMonster->GetY(), CParticle::HIT);
				pParticle->Ready();
				GET_SINGLE(CObjManager)->GetParticles().emplace_back(pParticle);


				int iRandNum = int(GetNumberMinBetweenMax(5.f, 10.f));
				for (int i = 0; i < iRandNum; i++)
				{
					shared_ptr<CObj> pParticle = make_shared<CHitParticle>(pMonster->GetX(), pMonster->GetY() + 10.f);
					pParticle->Ready();
					GET_SINGLE(CObjManager)->GetParticles().emplace_back(pParticle);
				}
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

			if (pPlayer->IsDead() == false)
			{
				pPlayer->SetState(GET_SINGLE(PlayerAttacked));
				pPlayer->SetIsAttacked(true);
				pPlayer->TakeDamage(m_fDamage);
			}
		}
		
	}

	// 공통
	for (auto& pObj : GET_SINGLE(CMapManager)->GetStructures())
	{
		if (m_strBulletName == L"SonicBoom")
			continue;

		CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
		if (pObj->GetInfo()->vPos.x < pPlayer->GetX() - ((WINCX >> 1) + 200.f) || pPlayer->GetX() + ((WINCX >> 1) + 200.f) < pObj->GetInfo()->vPos.x ||
			pObj->GetInfo()->vPos.y < pPlayer->GetY() - ((WINCY >> 1) + 200.f) || pPlayer->GetY() + ((WINCY >> 1) + 200.f) < pObj->GetInfo()->vPos.y)
			continue;

		if (CCollisionManager::CollideBullet(pObj.get(), this) == true)
		{
			CStructure* pStructure = dynamic_cast<CStructure*>(pObj.get());
			pStructure->SetCurHp(pStructure->GetCurHp() - 10);
			if (pStructure->GetCurDrawID() >= pStructure->GetMaxDrawID())
				continue;

			pStructure->SetCurDrawID(pStructure->GetCurDrawID() + 1);

			shared_ptr<CObj> pParticle = make_shared<CParticle>(pObj->GetX(), pObj->GetY(), CParticle::HIT);
			pParticle->Ready();
			GET_SINGLE(CObjManager)->GetParticles().emplace_back(pParticle);

		}

	}

	for(auto& pTile : GET_SINGLE(CMapManager)->GetWalls())
		CCollisionManager::CollideTileBullet(pTile, this);


}

void CBullet::Render(const HDC& _hdc)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

	// Bullet 그리기
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Bullet", m_strBulletName, 1);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matWorld, matScale, matRev, matParent;

	if(pPlayer->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, 1.f, -1.f, 1.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRev, D3DXToRadian(m_fDegree));
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	matWorld = matScale * matRev * matParent;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	
	if(m_eObjID == OBJ::MONSTER)
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255 - m_iDelta, 255, 255, 255));
	else
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBullet::Release()
{
}
