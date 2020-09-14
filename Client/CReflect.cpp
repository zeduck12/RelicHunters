#include "pch.h"
#include "CReflect.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CTimeManager.h"
#include "CObjManager.h"
#include "CMonster.h"
#include "CMonsterState.h"
#include "CBoss.h"
#include "CBossState.h"
#include "CCollisionManager.h"
#include "CCameraManager.h"
#include "CMapManager.h"
#include "CStructure.h"
#include "CParticle.h"
#include "CHitParticle.h"
#include "CNumberParticle.h"

CReflect::CReflect(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed, float _fShootingDegree, OBJ::ID _eID, float _fDamage)
{
	m_iDrawID = 0;
	m_fDegree = _fShootingDegree;
	m_eObjID = _eID;
	m_fSpeed = _fSpeed;
	m_fDamage = _fDamage;

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
		fAddAngle = GetNumberMinBetweenMax(-7.f, 7.f);

	D3DXMATRIX matWorld, matRotZ;
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(fAddAngle));
	matWorld = matRotZ;
	D3DXVec3TransformCoord(&m_vDir, &m_vDir, &matWorld);

	m_fStackTime = 0.f;
	m_fCoolTime = 0.f;
	m_fDamage = 200.f;
}

CReflect::~CReflect()
{
	Release();
}

void CReflect::Ready(void)
{
}

int CReflect::Update(float _fDeltaTime)
{
	if (m_bIsCollide == true)
		Reflect();
	else
	{

		D3DXMATRIX matWorld, matRev, matParent;

		D3DXMatrixRotationZ(&matRev, D3DXToRadian(m_fDegree));
		D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

		matWorld = matRev * matParent;

		for (int i = 0; i < 4; i++)
			D3DXVec3TransformCoord(&m_vRealVertex[i], &m_vRotVertex[i], &matWorld);

		m_tInfo.vPos += m_vDir * m_fSpeed;


	}
	return 0;
}

void CReflect::LateUpdate(void)
{

	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 10.f)
		this->SetIsValid(false);

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

				pParticle = make_shared<CNumberParticle>(pMonster->GetX(), pMonster->GetY() - 30.f, m_fDamage);
				pParticle->Ready();
				GET_SINGLE(CObjManager)->GetParticles().emplace_back(pParticle);
			}
		}

	}

	for (auto& pObj : GET_SINGLE(CMapManager)->GetStructures())
	{
		if (CCollisionManager::CollideReflectStructure(pObj.get(), this) == true)
		{
			m_bIsCollide = true;
			CStructure* pStructure = dynamic_cast<CStructure*>(pObj.get());
			pStructure->SetCurHp(pStructure->GetCurHp() - 10);
			if (pStructure->GetCurDrawID() >= pStructure->GetMaxDrawID())
				continue;

			pStructure->SetCurDrawID(pStructure->GetCurDrawID() + 1);
		}
	}

	for (auto& pTile : GET_SINGLE(CMapManager)->GetWalls())
	{
		if(CCollisionManager::CollideReflectWall(pTile, this))
			m_bIsCollide = true;
	}
}

void CReflect::Render(const HDC& _hdc)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Bullet", L"Plasma", 0);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matWorld, matScale, matRev, matParent;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRev, m_fDegree);
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	matWorld = matScale * matRev * matParent;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CReflect::Release(void)
{
}

void CReflect::Reflect(void)
{
	m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree))  * 10.f;
	m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree))  * 10.f;
}
