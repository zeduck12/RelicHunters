#include "pch.h"
#include "CBombParticle.h"
#include "CTimeManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CMonster.h"
#include "CMonsterState.h"
#include "CObjManager.h"
#include "CCollisionManager.h"
#include "CBoss.h"
#include "CBossState.h"
#include "CNumberParticle.h"
#include "CHitParticle.h"

CBombParticle::CBombParticle(float _fX, float _fY)
{
	m_pOwner = nullptr;
	m_eID = CParticle::FIRE;

	m_iDrawID = 0;
	m_iDrawMax = 11;
	m_fDeltaX = GetNumberMinBetweenMax(-400.f, 400.f);
	m_fDeltaY = GetNumberMinBetweenMax(-300.f, 300.f);
	m_fStackTime = 0.f;

	m_tInfo.vPos = { _fX + m_fDeltaX, _fY + m_fDeltaY, 0.f };
	m_tInfo.vSize = { 200.f, 200.f, 1.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vImageSize = { 0.f, 0.f, 0.f };

	m_fSpeed = 0.f;
	m_iAlpha = 255;
	m_fDegree = 0.f;
}

void CBombParticle::Ready(void)
{
}

int CBombParticle::Update(float _fDeltaTime)
{
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 0.1f)
	{
		m_iDrawID++;
		m_fStackTime = 0.f;
	}

	if (m_iDrawID >= 11)
		this->SetIsValid(false);

    return 0;
}

void CBombParticle::LateUpdate(void)
{
	// 적들만 충돌처리
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
					pBoss->SetState(new BossAttackedState());
					pBoss->SetHp(pBoss->GetHp() - 200.f);

					shared_ptr<CObj> pParticle = make_shared<CNumberParticle>(pMonster->GetX(), pMonster->GetY() - 30.f, 200.f);
					pParticle->Ready();
					GET_SINGLE(CObjManager)->GetParticles().emplace_back(pParticle);
				}
			}
			else
			{
				CMonster* pMonst = dynamic_cast<CMonster*>(pMonster.get());
				if (pMonst->IsDead() == false)
				{
					pMonst->SetState(new AttackedState());
					pMonst->SetHp(pMonst->GetHp() - 200.f);

					shared_ptr<CObj> pParticle = make_shared<CNumberParticle>(pMonster->GetX(), pMonster->GetY() - 30.f, 200.f);
					pParticle->Ready();
					GET_SINGLE(CObjManager)->GetParticles().emplace_back(pParticle);
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

void CBombParticle::Release(void)
{
}

void CBombParticle::Render(const HDC& _hdc)
{
	const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Special", L"Explosion", m_iDrawID);

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, this->GetX(), this->GetY(), 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}
