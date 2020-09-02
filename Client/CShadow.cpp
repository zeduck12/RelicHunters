#include "pch.h"
#include "CShadow.h"
#include "CAnimation.h"
#include "CGraphicDevice.h"
#include "CTextureManager.h"
#include "CMonster.h"

D3DXVECTOR3 CShadow::vOldPos = { 0.f, 0.f, 0.f };
float CShadow::fAddY = 0.f;

void CShadow::RenderFlyShadow(CObj* _pOwner)
{
	CMonster* pMonster = dynamic_cast<CMonster*>(_pOwner);
	D3DXVECTOR3 vOldPos = pMonster->GetOldPos();

	float fSizeX = _pOwner->GetInfo()->vSize.x;
	float fSizeY = _pOwner->GetInfo()->vSize.y;

	pMonster->SetAddY(pMonster->GetAddY() + 0.8f);
	//fAddY += 1.5f;

	int iFrame = 0;
	CAnimation* pAnimation = _pOwner->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pOwner->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pOwner->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, -0.6f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, -0.6f, 0.f);

	D3DXMatrixTranslation(&matTrans, vOldPos.x, vOldPos.y + pMonster->GetAddY(), 0.f);
	vOldPos = { vOldPos.x , vOldPos.y + pMonster->GetAddY() , 0.f};

	matWorld = matScale * matTrans;

	matWorld._23 = 10.f;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 100, 100, 100));

}

void CShadow::RenderPatrolShadow(CObj* _pOwner)
{
	D3DXVECTOR3 vGap = dynamic_cast<CMonster*>(_pOwner)->GetGap();

	float fSizeX = _pOwner->GetInfo()->vSize.x;
	float fSizeY = _pOwner->GetInfo()->vSize.y;

	int iFrame = 0;
	CAnimation* pAnimation = _pOwner->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pOwner->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pOwner->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, -0.6f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, -0.6f, 0.f);

	D3DXVECTOR3 vResultPos = _pOwner->GetInfo()->vPos + vGap * 1.5;

	D3DXMatrixTranslation(&matTrans, vResultPos.x, vResultPos.y, 0.f);
	matWorld = matScale * matTrans;

	matWorld._23 = 10.f;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 100, 100, 100));

}

void CShadow::RenderLandingShadow(CObj* _pOwner)
{
	CMonster* pMonster = dynamic_cast<CMonster*>(_pOwner);
	D3DXVECTOR3 vOldPos = pMonster->GetOldPos();
	D3DXVECTOR3 vGap = dynamic_cast<CMonster*>(_pOwner)->GetGap();

	float fSizeX = _pOwner->GetInfo()->vSize.x;
	float fSizeY = _pOwner->GetInfo()->vSize.y;

	pMonster->SetAddY(pMonster->GetAddY() - 0.8f);
	//fAddY -= 1.5f;

	int iFrame = 0;
	CAnimation* pAnimation = _pOwner->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pOwner->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pOwner->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, -0.6f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, -0.6f, 0.f);

	D3DXVECTOR3 vResultPos = vOldPos + vGap;

	D3DXMatrixTranslation(&matTrans, vResultPos.x, vResultPos.y + pMonster->GetAddY(), 0.f);
	matWorld = matScale * matTrans;

	matWorld._23 = 10.f;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 100, 100, 100));

}

void CShadow::RenderShadow(CObj* _pOwner)
{
	float fSizeX = _pOwner->GetInfo()->vSize.x;
	float fSizeY = _pOwner->GetInfo()->vSize.y;

	int iFrame = 0;
	CAnimation* pAnimation = _pOwner->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pOwner->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pOwner->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, -0.6f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, -0.6f, 0.f);

	D3DXMatrixTranslation(&matTrans, _pOwner->GetInfo()->vPos.x, _pOwner->GetInfo()->vPos.y + fSizeY, 0.f);
	matWorld = matScale * matTrans;

	matWorld._23 = 10.f;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 100, 100, 100));


}

void CShadow::RenderSheetShadow(CObj* _pOwner, const TEXINFO*& _pTexInfo)
{
	float fSizeX = _pOwner->GetInfo()->vSize.x;
	float fSizeY = _pOwner->GetInfo()->vSize.y;

	float fCenterX = float(_pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(_pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pOwner->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, -0.6f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, -0.6f, 0.f);

	D3DXVECTOR3 vSpectrumPos = _pOwner->GetInfo()->vPos;
	vSpectrumPos.y -= 20.f;

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y + 20 + fSizeY, 0.f);
	matWorld = matScale * matTrans;
	
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(_pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 100, 100, 100));
}

void CShadow::RenderSheetProjectile(CObj* _pOwner, const TEXINFO*& _pTexInfo, const float& _fDegree)
{
	float fSizeX = _pOwner->GetInfo()->vSize.x;
	float fSizeY = _pOwner->GetInfo()->vSize.y;

	float fCenterX = float(_pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(_pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matRev, matTrans, matWorld;
	if (_pOwner->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, -0.6f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, -0.6f, 0.f);

	D3DXMatrixRotationZ(&matRev, D3DXToRadian(_fDegree));

	D3DXVECTOR3 vSpectrumPos = _pOwner->GetInfo()->vPos;
	vSpectrumPos.y -= 20.f;

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y + 50 + fSizeY, 0.f);
	matWorld = matScale * matRev * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(_pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 100, 100, 100));
}

