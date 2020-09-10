#include "pch.h"
#include "CShotGun.h"
#include "CObjManager.h"
#include "CPlayerManager.h"
#include "CCollisionManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CMonster.h"
#include "CBoss.h"
#include "CBossState.h"
#include "CMonsterState.h"
#include "CPlayer.h"
#include "CPlayerState.h"
#include "CMapManager.h"
#include "CStructure.h"
#include "CParticle.h"
#include "CTimeManager.h"

CShotGun::CShotGun(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fAddDegree, float _fSpeed, float _fShootingDegree, OBJ::ID _eID /*= OBJ::PLAYER*/, const wstring& _strBulletName /*= L"Small"*/)
	:
	m_fAddDegree{ _fAddDegree }
{
	m_fDegree = _fShootingDegree;
	m_fSpeed = _fSpeed;

	m_tInfo.vPos = { _fX , _fY , 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 20.f, 10.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_vDir = _vDir;

	// 회전할 버텍스 좌표
	m_vRotVertex[0].x = -(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[0].y = -(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[1].x = +(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[1].y = -(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[2].x = +(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[2].y = +(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[3].x = -(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[3].y = +(m_tInfo.vSize.y * 0.5f);

	m_eObjID = _eID;
	m_strBulletName = _strBulletName;
	m_fStackTime = 0.f;
	m_fDamage = 10.f;
}

CShotGun::~CShotGun()
{
	Release();
}

int CShotGun::Update(float _fDeltaTime)
{
	CheckValidTime();
	// 현재 방향벡터를 기준으로 3발 발사하기.
	// 총알이 발사될 각도 구함.
	D3DXMATRIX matWorld, matRev, matParent;

	D3DXMatrixRotationZ(&matRev, D3DXToRadian(m_fDegree + m_fAddDegree));
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	matWorld = matRev * matParent;

	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&m_vRealVertex[i], &m_vRotVertex[i], &matWorld);

	if (m_fAddDegree != 0.f)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + m_fAddDegree)) * m_fSpeed;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + m_fAddDegree)) * m_fSpeed;
	}
	else
		m_tInfo.vPos += m_vDir * m_fSpeed;

	return 0;
}

void CShotGun::LateUpdate(void)
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
					if (pBoss->IsDead() == false && pBoss->IsCrack() == true)
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
	{
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

	for (auto& pTile : GET_SINGLE(CMapManager)->GetWalls())
		CCollisionManager::CollideTileBullet(pTile, this);
}

void CShotGun::Render(const HDC& _hdc)
{
	//MoveToEx(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y, nullptr);

	//for (int i = 1; i < 4; i++)
	//	LineTo(_hdc, (int)m_vRealVertex[i].x, (int)m_vRealVertex[i].y);
	//LineTo(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y);

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Bullet", m_strBulletName, 1);
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
}

void CShotGun::CheckValidTime(void)
{
	float fCoolTime = 0.f;
	if (m_eObjID == OBJ::MONSTER)
		fCoolTime = 1.f;
	else
		fCoolTime = 0.25f;

	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= fCoolTime)
	{
		m_fStackTime = 0.f;
		this->SetIsValid(false);
	}
}
