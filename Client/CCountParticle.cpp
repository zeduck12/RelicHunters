#include "pch.h"
#include "CCountParticle.h"
#include "CTimeManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CPlayerManager.h"

CCountParticle::CCountParticle()
{
	m_pOwner = nullptr;
	m_eID = CParticle::COUNT;

	m_iDrawID = 0;
	m_iDrawMax = 1;
	m_fStackTime = 0.f;

	m_tInfo.vPos  = { 400.f, 300.f , 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 1.f };
	m_tInfo.vDir  = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vImageSize = { 0.f, 0.f, 0.f };

	m_fSpeed = 0.f;
	m_iAlpha = 255;
	m_fDegree = 0.f;
}

void CCountParticle::Ready(void)
{
}

int CCountParticle::Update(float _fDeltaTime)
{
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	m_fCoolTime  += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 0.5f)
		this->SetIsValid(false);

	if (m_fCoolTime >= 0.01f)
	{
		m_fCoolTime = 0.f;
		m_fDeltaY -= 5.f;
		m_iAlpha  -= 20;

		if (m_iAlpha <= 0)
			m_iAlpha = 0;
	}

    return 0;
}

void CCountParticle::LateUpdate(void)
{
}

void CCountParticle::Release(void)
{
}

void CCountParticle::Render(const HDC& _hdc)
{
	float fX = GET_SINGLE(CPlayerManager)->GetPlayer()->GetX();
	float fY = GET_SINGLE(CPlayerManager)->GetPlayer()->GetY();

	D3DXMATRIX matScale, matTrans, matWorld;
	TCHAR szBuf[MAX_PATH] = L"";
	wsprintf(szBuf, L"+%d", 10);
	D3DXMatrixScaling(&matScale, 1.3f, 1.3f, 1.f);
	D3DXMatrixTranslation(&matTrans, fX - 25.f, fY - 70.f + m_fDeltaY, 0.f);
	matWorld = matScale * matTrans;
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

}
