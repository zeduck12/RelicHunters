#include "pch.h"
#include "CFireParticle.h"
#include "CTimeManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"

CFireParticle::CFireParticle(float _fX, float _fY)
{
	m_pOwner = nullptr;
	m_eID = CParticle::FIRE;

	m_iDrawID = int(GetNumberMinBetweenMax(0, 7));
	m_fDeltaX = GetNumberMinBetweenMax(-50.f, 50.f);
	m_fDeltaY = GetNumberMinBetweenMax(-50.f, 50.f);
	m_iDrawMax = 1;
	m_fStackTime = 0.f;

	m_tInfo.vPos = { _fX, _fY , 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 1.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vImageSize = { 0.f, 0.f, 0.f };

	m_fSpeed = 0.f;
	m_iAlpha = 255;
	m_fDegree = 0.f;
}

void CFireParticle::Ready(void)
{
}

int CFireParticle::Update(float _fDeltaTime)
{
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 0.5f)
	{
		m_fStackTime = 0.f;
		this->SetIsValid(false);
	}

	m_tInfo.vPos.y -= 10.f * GET_SINGLE(CTimeManager)->GetElapsedTime();
    return 0;
}

void CFireParticle::LateUpdate(void)
{
}

void CFireParticle::Release(void)
{
}

void CFireParticle::Render(const HDC& _hdc)
{
	const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Special", L"Particle", m_iDrawID);

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, this->GetX() + m_fDeltaX, this->GetY() + m_fDeltaY, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
}
