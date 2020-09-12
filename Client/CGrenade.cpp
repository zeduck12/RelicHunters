#include "pch.h"
#include "CGrenade.h"
#include "CPlayerManager.h"
#include "CPlayer.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CObjManager.h"
#include "CCollisionManager.h"
#include "CTimeManager.h"
#include "CMonster.h"
#include "CMonsterState.h"
#include "CStructure.h"
#include "CMapManager.h"
#include "CShadow.h"
#include "CBoss.h"
#include "CBossState.h"
#include "CSmoke.h"

CGrenade::CGrenade(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed, float _fShootingDegree, float _fShootingDist, bool _bIsReverse /*= false*/)
	:
	m_fBombX{ 0.f },
	m_fBombY{ 0.f },
	m_fRotZAngle{ 0.f },
	m_bIsReverse{ _bIsReverse },
	m_iCollideCount{ 0 }

{
	m_iDrawID = 0;
	m_fStackTime = 0.f;
	m_fBombStackTime = 0.f;
	m_bIsCollide = false;
	m_vShadowPos = { _fX, _fY, 0 };
	m_tInfo.vPos = { _fX , _fY , 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 10.f, 10.f, 0.f };

	m_vDir = _vDir;

	m_fDegree = _fShootingDegree;
	m_fSaveDegree = _fShootingDegree;
	m_fSpeed = 0.f;
	m_fShootingDist = _fShootingDist;

	m_fGravity = 0.f;
	m_fJumpPower = 0.f;
	m_fJumpPowerOrigin = 4.f;

	m_eDir = DIRECTION::END;

	m_fMiniGravity = 0.f;
	m_fMiniJumpPower = 0.f;
	m_fMiniJumpPowerOrigin = 1.5f;
	m_eImageID = IMAGE::PLAYER;

}

void CGrenade::Ready(void)
{
	m_fJumpPower = m_fJumpPowerOrigin;
	m_fMiniJumpPower = m_fMiniJumpPowerOrigin;
	// 생성될때 방향 설정.
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	m_eDir = dynamic_cast<CPlayer*>(pPlayer)->GetDirection();
}

int CGrenade::Update(float _fDeltaTime)
{
	if (m_iCollideCount <= 3)
	{
		if (m_fRotZAngle <= 300.f)
			m_fRotZAngle += 4.6f;

		if (m_bIsCollide == true)
			MiniJump();
		else
			ShootGrenade();

		if (m_bIsReverse == false)
		{
			m_fCoolTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
			if (m_fCoolTime >= 0.05f)
			{
				m_fCoolTime = 0.f;

				float fRandNum = 0;
				for (int i = 0; i < 2; i++)
				{
					fRandNum = GetNumberMinBetweenMax(-5.f, 5.f);
					shared_ptr<CObj> pParticle = make_shared<CSmoke>(this->GetX() + fRandNum, this->GetY() + fRandNum);
					pParticle->Ready();
					GET_SINGLE(CObjManager)->GetParticles().emplace_back(pParticle);
				}
			}

		}
	}


	return 0;
}

void CGrenade::LateUpdate(void)
{
	for (auto& pGrenade : CObjManager::Get_Instance()->GetGrenades())
	{
		DO_IF_IS_NOT_VALID_OBJ(pGrenade)
			continue;

		if (this->m_bIsReverse == false && dynamic_cast<CGrenade*>(pGrenade.get())->IsReverse() == true)
		{
			if (CCollisionManager::CollideGrenade(this, pGrenade.get()) == true)
			{
				m_bIsCollide = true;
				dynamic_cast<CGrenade*>(pGrenade.get())->SetIsCollide(true);
				m_iCollideCount++;
				dynamic_cast<CGrenade*>(pGrenade.get())->SetCollideCount(1);

				if (m_iCollideCount <= 3)
				{
					GET_SINGLE(CSoundManager)->StopSound(CSoundManager::EFFECT);
					GET_SINGLE(CSoundManager)->PlaySound((TCHAR*)L"sfx_grenade_bounce.wav", CSoundManager::EFFECT);
				}
			}

		}
	}

	if (m_iCollideCount >= 3)
	{
		if (m_bIsPlayingSFX == false)
		{
			m_bIsPlayingSFX = true;
			GET_SINGLE(CSoundManager)->StopSound(CSoundManager::EFFECT);
			GET_SINGLE(CSoundManager)->PlaySound((TCHAR*)L"sfx_grenade_explosion.wav", CSoundManager::EFFECT);
		}

		// 이안에서의 기간동안 Object들 Damage
		m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		m_fBombStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fBombStackTime > 0.6f)
		{
			if (m_bIsAttack == false)
			{
				TakeDamageToObejcts();
				m_bIsAttack = true;
			}
		}

		if (m_fStackTime > 0.1f)
		{
			m_iDrawID++;
			m_fStackTime = 0.f;
		}

		if (m_iDrawID >= 11)
			this->SetIsValid(false);
	}
}

void CGrenade::Release(void)
{
}

void CGrenade::Render(const HDC& _hdc)
{
	if (m_iCollideCount >= 3)
	{
		DrawBombParticle();
		return;
	}

	const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Grenade");

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	if (this->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fRotZAngle));
	D3DXMatrixTranslation(&matTrans, this->GetInfo()->vPos.x, this->GetInfo()->vPos.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);


	if (m_bIsReverse == true)
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 100, 100, 100));
	else
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CGrenade::TakeDamageToObejcts(void)
{
	// 일단 몬스터
	for (auto& pMonster : GET_SINGLE(CObjManager)->GetMonsters())
	{
		float fWidth = 360.f;
		float fHeight = 350.f;
		if (pMonster->GetX() < this->GetX() + (fWidth * 0.5f) && this->GetX() - (fWidth * 0.5f) < pMonster->GetX() &&
			pMonster->GetY() < this->GetY() + (fHeight * 0.5f) && this->GetY() - (fHeight * 0.5f) < pMonster->GetY())
		{
			if (pMonster->GetImageID() == IMAGE::BOSS)
			{
				CBoss* pBoss = dynamic_cast<CBoss*>(pMonster.get());
				if (pBoss->IsDead() == false && pBoss->IsCrack() == true && pBoss->IsInvicible() == false)
				{
					pBoss->SetState(new BossAttackedState);
					pBoss->SetHp(pBoss->GetHp() - 50.f);
				}
			}
			else
			{
				CMonster* pMonst = dynamic_cast<CMonster*>(pMonster.get());
				if (pMonst->IsDead() == false)
				{
					pMonst->SetState(new AttackedState());
					pMonst->SetHp(pMonst->GetHp() - 100.f);
				}
			}
		}
	}

	// 구조물
	for (auto& pObj : GET_SINGLE(CMapManager)->GetStructures())
	{
		float fWidth = 350.f;
		float fHeight = 350.f;
		if (pObj->GetX() < this->GetX() + (fWidth * 0.5f) && this->GetX() - (fWidth * 0.5f) < pObj->GetX() &&
			pObj->GetY() < this->GetY() + (fHeight * 0.5f) && this->GetY() - (fHeight * 0.5f) < pObj->GetY())
		{
			CStructure* pStructure = dynamic_cast<CStructure*>(pObj.get());
			pStructure->SetCurHp(pStructure->GetCurHp() - 90);
			if (pStructure->GetCurDrawID() >= pStructure->GetMaxDrawID())
				continue;
			pStructure->SetCurDrawID(pStructure->GetCurDrawID() + 2);
		}
	}

}

void CGrenade::DrawBombParticle(void)
{
	const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Particle", L"Bomb", m_iDrawID);

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}


void CGrenade::ShootGrenade(void)
{
	m_fJumpPower -= 0.1f;
	if (m_fShootingDist > 120.f)
		m_fShootingDist = 120.f;

	if (m_bIsReverse == false)
	{
		if (m_eDir == DIRECTION::LEFT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 20.f)) * 2.f;
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 20.f)) * 2.f;

			m_tInfo.vPos -= D3DXVECTOR3(m_vDir.y, -m_vDir.x, 0.f) * m_fJumpPower * 2.f;
			m_tInfo.vPos += m_vDir * 3.8f * (m_fShootingDist / 100);
		}
		else if (m_eDir == DIRECTION::RIGHT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 20.f)) * 2.f;
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 20.f)) * 2.f;

			m_tInfo.vPos += D3DXVECTOR3(m_vDir.y, -m_vDir.x, 0.f) * m_fJumpPower * 2.f;
			m_tInfo.vPos += m_vDir * 3.8f * (m_fShootingDist / 100);

		}

	}
	else
	{
		if (m_eDir == DIRECTION::LEFT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 20.f)) * 2.f;
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 20.f)) * 2.f;

			m_tInfo.vPos -= D3DXVECTOR3(-m_vDir.y, m_vDir.x, 0.f) * m_fJumpPower * 2.f;
			m_tInfo.vPos += m_vDir * 3.8f * (m_fShootingDist / 100);
		}
		else if (m_eDir == DIRECTION::RIGHT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 20.f)) * 2.f;
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 20.f)) * 2.f;

			m_tInfo.vPos += D3DXVECTOR3(-m_vDir.y, m_vDir.x, 0.f) * m_fJumpPower * 2.f;
			m_tInfo.vPos += m_vDir * 3.8f * (m_fShootingDist / 100);
		}

	}
}

void CGrenade::MiniJump(void)
{
	m_fMiniJumpPower -= 0.1f;
	if (m_fShootingDist > 170.f)
		m_fShootingDist = 170.f;

	if (m_bIsReverse == false)
	{
		if (m_eDir == DIRECTION::LEFT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fSaveDegree + 40.f)) * 2.f;
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fSaveDegree + 40.f)) * 2.f;

			m_tInfo.vPos += D3DXVECTOR3(-m_vDir.y, m_vDir.x, 0.f) * m_fMiniJumpPower * 3.f;
			m_tInfo.vPos += m_vDir * 3.8f * (m_fShootingDist / 100);
		}
		else if (m_eDir == DIRECTION::RIGHT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 40.f)) * 2.f;
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 40.f)) * 2.f;

			m_tInfo.vPos += D3DXVECTOR3(m_vDir.y, -m_vDir.x, 0.f) * m_fMiniJumpPower * 3.f;
			m_tInfo.vPos += m_vDir * 3.8f * (m_fShootingDist / 100);
		}
	}
	else
	{
		if (m_eDir == DIRECTION::LEFT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fSaveDegree - 40.f)) * 2.f;
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fSaveDegree - 40.f)) * 2.f;

			m_tInfo.vPos += D3DXVECTOR3(m_vDir.y, -m_vDir.x, 0.f) * m_fMiniJumpPower * 3.f;
			m_tInfo.vPos += m_vDir * 3.8f * (m_fShootingDist / 100);
		}
		else if (m_eDir == DIRECTION::RIGHT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 40.f)) * 2.f;
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 40.f)) * 2.f;


			m_tInfo.vPos += D3DXVECTOR3(-m_vDir.y, m_vDir.x, 0.f) * m_fMiniJumpPower * 3.f;
			m_tInfo.vPos += m_vDir * 3.8f * (m_fShootingDist / 100);
		}
	}
}




