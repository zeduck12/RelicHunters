#include "pch.h"
#include "CParticle.h"
#include "CTimeManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"

CParticle::CParticle(float _fX, float _fY, CParticle::ID _eID, int _iDrawMax, const wstring& _strName, CObj* _pOwner)
{
	m_iDrawID = 0;
	m_iDrawMax = _iDrawMax;
	m_fStackTime = 0.f;

	m_tInfo.vPos = { _fX, _fY, 0.f};
	m_tInfo.vSize = { 1.f, 1.f, 1.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vImageSize = { 0.f, 0.f, 0.f };

	m_eID = _eID;
	m_strName = _strName;
	m_pOwner = _pOwner;
}

void CParticle::Ready(void)
{
}

int CParticle::Update(float _fDeltaTime)
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
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}
