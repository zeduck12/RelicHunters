#include "pch.h"
#include "CNumberParticle.h"
#include "CTimeManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"

CNumberParticle::CNumberParticle(float _fX, float _fY, float _fDamage/* = 0.f*/)
{
	m_pOwner = nullptr;
	m_eID = CParticle::NUMBER;

	m_iDrawID = 0;
	m_iDrawMax = 1;
	m_fStackTime = 0.f;

	m_tInfo.vPos = { _fX, _fY, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 1.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vImageSize = { 0.f, 0.f, 0.f };

	m_fDegree = 0.f;
	m_fDamage = _fDamage;
	m_fScale = GetNumberMinBetweenMax(1.f, 1.5f);
	m_fSpeed = GetNumberMinBetweenMax(150.f, 250.f);
	m_fDeltaX = GetNumberMinBetweenMax(-20.f, 20.f);

	m_iAlpha = 200;
	m_iRandNum = rand() % 3 + 1;
}

void CNumberParticle::Ready(void)
{
}

int CNumberParticle::Update(float _fDeltaTime)
{
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 0.5f)
		this->SetIsValid(false);

	// 생성 위치에서 위로 올라감.
	if(m_fStackTime <= 0.3f)
		m_tInfo.vPos.y -= m_fSpeed * GET_SINGLE(CTimeManager)->GetElapsedTime();
	else
	{
		m_fCoolTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fCoolTime >= 0.05f)
		{
			m_fCoolTime = 0.f;
			m_iAlpha -= 40;
		}
	}

    return 0;
}

void CNumberParticle::LateUpdate(void)
{
}

void CNumberParticle::Release(void)
{
}

void CNumberParticle::Render(const HDC& _hdc)
{
	// 노란색 255 255 0
	// 빨강색 255 0   0
	// 주황색 255 127 0

	D3DXMATRIX matScale, matTrans, matWorld;
	TCHAR szBuf[MAX_PATH] = L"";
	wsprintf(szBuf, L"%d", int(m_fDamage));
	D3DXMatrixScaling(&matScale, m_fScale * 1.1f, m_fScale * 1.1f, 1.f);
	D3DXMatrixTranslation(&matTrans, this->GetX() + m_fDeltaX, this->GetY(), 0.f);
	matWorld = matScale * matTrans;
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0));

	wsprintf(szBuf, L"%d", int(m_fDamage));
	D3DXMatrixScaling(&matScale, m_fScale, m_fScale, 1.f);
	D3DXMatrixTranslation(&matTrans, this->GetX() + m_fDeltaX, this->GetY(), 0.f);
	matWorld = matScale * matTrans;
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);

	if(m_iRandNum == 1)
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	else if(m_iRandNum == 2)
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(m_iAlpha, 255, 0, 0));
	else if(m_iRandNum == 3)
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 0));
}
