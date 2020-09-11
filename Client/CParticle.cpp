#include "pch.h"
#include "CParticle.h"
#include "CTimeManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CPlayerManager.h"
#include "CCollisionManager.h"

CParticle::CParticle(float _fX, float _fY, CParticle::ID _eID, int _iDrawMax, const wstring& _strName, CObj* _pOwner)
{
	m_iDrawID = 0;
	m_iDrawMax = _iDrawMax;
	m_fStackTime = 0.f;

	m_tInfo.vPos = { _fX, _fY, 0.f};
	m_tInfo.vSize = { 10.f, 10.f, 1.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vImageSize = { 0.f, 0.f, 0.f };

	m_eID = _eID;
	m_strName = _strName;
	m_pOwner = _pOwner;

	m_fDegree = GetNumberMinBetweenMax(0.f, 360.f);
	m_fSpeed  = GetNumberMinBetweenMax(5.f, 20.f);
}

void CParticle::Ready(void)
{
}

int CParticle::Update(float _fDeltaTime)
{
	if (m_eID == CParticle::FINISH)
	{
		m_fCoolTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fCoolTime >= 0.5f)
		{
			m_fFinishTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
			if (m_fFinishTime >= 1.5f)
				MoveToPlayer();
			return 0;
		}
		else
			Move();
	}
	else
	{
		m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fStackTime >= 0.1f)
		{
			m_iDrawID++;
			m_fStackTime = 0.f;
		}

		// 한 애니매이션 끝나면 사라지게
		if (m_iDrawID >= m_iDrawMax)
		{
			m_iDrawID = m_iDrawMax - 1;
			this->SetIsValid(false);
		}

	}

	return 0;
}

void CParticle::LateUpdate(void)
{
}

void CParticle::Release(void)
{
}

void CParticle::Render(const HDC& _hdc)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Particle", m_strName, m_iDrawID);
	if (pTexInfo == nullptr)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;

	if (m_pOwner != nullptr)
	{
		if(m_pOwner->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	}
	else
	{
		if (m_eID == CParticle::FINISH)
			D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	}
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	if (m_eID == CParticle::FINISH)
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(220, 255, 255, 255));
	else
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CParticle::Move(void)
{
	m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree)) * m_fSpeed;
	m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree)) * m_fSpeed;
}

void CParticle::MoveToPlayer()
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	
	m_fAddSpeed += 0.6f;
	// 플레이어 쪽으로 빨려가게
	D3DXVECTOR3 vDist = pPlayer->GetInfo()->vPos - this->GetInfo()->vPos;
	D3DXVec3Normalize(&vDist, &vDist);

	this->SetPosVector(this->GetPosVector() + vDist * m_fAddSpeed);

	// 플레이어와 부딫히면 사라지게
	if (CCollisionManager::CollideBullet(pPlayer, this) == true)
	{
		int iCurCoins = GET_SINGLE(CPlayerManager)->GetInventory()->GetCoins();
		GET_SINGLE(CPlayerManager)->GetInventory()->SetCoins(iCurCoins + 10);
		GET_SINGLE(CSoundManager)->PlaySound((TCHAR*)L"sfx_coin1.wav", CSoundManager::UI);
	}
}
