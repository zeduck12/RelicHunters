#include "pch.h"
#include "CReflectBoard.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"

CReflectBoard::CReflectBoard(CObj* _pOwner)
{
    m_pOwner = _pOwner;
	m_tInfo.vPos = { m_pOwner->GetX(), m_pOwner->GetY(), 0.f };
	m_tInfo.vSize = { 125.f, 34.f, 1.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vImageSize = { 0.f, 0.f, 0.f };
}

CReflectBoard::~CReflectBoard()
{
    Release();
}

void CReflectBoard::Ready(void)
{
}

int CReflectBoard::Update(float _fDeltaTime)
{
	m_tInfo.vPos.x = m_pOwner->GetX();
	m_tInfo.vPos.y = m_pOwner->GetY() - 90.f;

    return 0;
}

void CReflectBoard::LateUpdate(void)
{
}

void CReflectBoard::Release(void)
{
}

void CReflectBoard::Render(const HDC& _hdc)
{
	const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"ReflectBoard");

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fDegree));
	D3DXMatrixTranslation(&matTrans, this->GetX(), this->GetY() - 25.f, 0.f);
	matWorld = matScale * matRotZ * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}
