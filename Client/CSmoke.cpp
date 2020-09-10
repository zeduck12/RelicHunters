#include "pch.h"
#include "CSmoke.h"
#include "CTimeManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CRocket.h"

CSmoke::CSmoke(float _fX, float _fY, CObj* _pOwner)
{
	m_pOwner = _pOwner;

	m_iDrawID = 0;
	m_iDrawMax = 1;
	m_fStackTime = 0.f;

	m_tInfo.vPos = { _fX, _fY, 0.f };
	m_tInfo.vSize = { 10.f, 10.f, 1.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vImageSize = { 0.f, 0.f, 0.f };

	m_eID = CParticle::SMOKE;

	m_fDegree = 0.f;
	m_fSpeed = 0.f;
}

void CSmoke::Ready(void)
{
}

int CSmoke::Update(float _fDeltaTime)
{
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 0.1f)
	{
		m_iDelta -= 15;
		m_fScale -= 0.4f;
		m_fStackTime = 0.f;
	}

	if (m_iDelta <= 0)
		this->SetIsValid(false);

	return 0;
}

void CSmoke::LateUpdate(void)
{
}

void CSmoke::Release(void)
{
}

void CSmoke::Render(const HDC& _hdc)
{
	const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"AttackedParticle");

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 3.f + m_fScale, 3.f + m_fScale, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fDegree));
	D3DXMatrixTranslation(&matTrans, this->GetInfo()->vPos.x, this->GetInfo()->vPos.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iDelta, 255, 255, 255));
}
