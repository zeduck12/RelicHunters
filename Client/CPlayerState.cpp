#include "pch.h"
#include "CPlayerState.h"
#include "CAnimation.h"
#include "CTimeManager.h"
#include "CGraphicDevice.h"
#include "CPlayer.h"

DEFINITION_SINGLETON(PlayerIdleState)
DEFINITION_SINGLETON(PlayerMoveState)
DEFINITION_SINGLETON(PlayerAttacked)

void PlayerIdleState::Update(CPlayer* _pPlayer)
{
	CAnimation* pAnimation = _pPlayer->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Idle");

}

void PlayerIdleState::Render(CPlayer* _pPlayer)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pPlayer->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pPlayer->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pPlayer->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pPlayer->GetInfo()->vPos.x, _pPlayer->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void PlayerMoveState::Update(CPlayer* _pPlayer)
{
	CAnimation* pAnimation = _pPlayer->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Move");

	SetPlayerState(_pPlayer, GET_SINGLE(PlayerIdleState));
}

void PlayerMoveState::Render(CPlayer* _pPlayer)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pPlayer->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; 
	}

	TEXINFO* pTexInfo = _pPlayer->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pPlayer->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	D3DXMatrixTranslation(&matTrans, _pPlayer->GetInfo()->vPos.x, _pPlayer->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void PlayerAttacked::Update(CPlayer* _pPlayer)
{
	CAnimation* pAnimation = _pPlayer->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Attacked");

	m_fCoolTime = 0.4f;
	m_fStackTime += CTimeManager::Get_Instance()->GetElapsedTime();
	if (m_fStackTime > m_fCoolTime)
	{
		m_fStackTime = 0.f;
		SetPlayerState(_pPlayer, GET_SINGLE(PlayerIdleState));
		_pPlayer->SetIsAttacked(false);
	}

}

void PlayerAttacked::Render(CPlayer* _pPlayer)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pPlayer->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; 
	}

	TEXINFO* pTexInfo = _pPlayer->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pPlayer->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	D3DXMatrixTranslation(&matTrans, _pPlayer->GetInfo()->vPos.x, _pPlayer->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}
