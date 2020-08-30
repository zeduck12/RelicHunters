#include "pch.h"
#include "CDecoStar.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"

CDecoStar::CDecoStar(float _fX, float _fY, float _fWidth, float _fHeight, float _fGapX /*= 0.f*/, float _fGapY /*= 0.f*/)
{
	// √ ±‚»≠
	m_tInfo.vPos = { _fX , _fY, 0.f };
	m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
	m_tInfo.vImageSize = { 1.f, 1.f, 1.f };
	m_tInfo.vDir = { 1.f ,0.f , 0.f };
	m_tInfo.vLook = { 1.f ,0.f , 0.f };
	D3DXMatrixIdentity(&m_tInfo.mat);

	m_fSpawnX = _fX;
	m_fSpawnY = _fY;
	m_fGapX = _fGapX;
	m_fGapY = _fGapY;
}

CDecoStar::~CDecoStar()
{
    Release();
}

void CDecoStar::Ready(void)
{
}

int CDecoStar::Update(float _fDeltaTime)
{
	m_tInfo.vPos.x += 1.f;
	m_tInfo.vPos.y += 1.f;

	if (600.f + m_fGapY < m_tInfo.vPos.y)
	{
		float fDeltax = m_tInfo.vPos.x - m_fSpawnX ;
		m_tInfo.vPos.x -= fDeltax + 400.f + m_fGapX * 2.f;
		m_tInfo.vPos.y = -600.f + m_fGapY;
	}

    return 0;
}

void CDecoStar::LateUpdate(void)
{
}

void CDecoStar::Release(void)
{
}

void CDecoStar::Render(const HDC& _hdc)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Deco");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.6f, 0.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(100, 255, 255, 255));

}
