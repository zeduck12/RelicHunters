#include "pch.h"
#include "CPlayerState.h"
#include "CAnimation.h"
#include "CTimeManager.h"
#include "CGraphicDevice.h"
#include "CSceneManager.h"
#include "CPlayer.h"
#include "CShadow.h"
#include "CSceneManager.h"

DEFINITION_SINGLETON(PlayerIdleState)
DEFINITION_SINGLETON(PlayerMoveState)
DEFINITION_SINGLETON(PlayerAttacked)
DEFINITION_SINGLETON(PlayerDeath)

void PlayerIdleState::Update(CPlayer* _pPlayer)
{
	CAnimation* pAnimation = _pPlayer->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	PLAYER::ID ePlayerID = GET_SINGLE(CSceneManager)->GetPlayerID();
	switch (ePlayerID)
	{
	case PLAYER::JIMMY:
		pAnimation->ChangeClip("Idle");
		break;
	case PLAYER::PINKY:
		pAnimation->ChangeClip("Pinky_Idle");
		break;
	case PLAYER::RAFF:
		pAnimation->ChangeClip("Raff_Idle");
		break;
	}

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

	CShadow::RenderShadowPlayer(_pPlayer);
	CShadow::RenderShadow(_pPlayer);
}

void PlayerMoveState::Update(CPlayer* _pPlayer)
{
	CAnimation* pAnimation = _pPlayer->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());


	PLAYER::ID ePlayerID = GET_SINGLE(CSceneManager)->GetPlayerID();
	switch (ePlayerID)
	{
	case PLAYER::JIMMY:
		pAnimation->ChangeClip("Move");
		break;
	case PLAYER::PINKY:
		pAnimation->ChangeClip("Pinky_Move");
		break;
	case PLAYER::RAFF:
		pAnimation->ChangeClip("Raff_Move");
		break;
	}

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

	CShadow::RenderShadow(_pPlayer);
}

void PlayerAttacked::Update(CPlayer* _pPlayer)
{
	CAnimation* pAnimation = _pPlayer->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	
	PLAYER::ID ePlayerID = GET_SINGLE(CSceneManager)->GetPlayerID();
	switch (ePlayerID)
	{
	case PLAYER::JIMMY:
		pAnimation->ChangeClip("Attacked");
		break;
	case PLAYER::PINKY:
		pAnimation->ChangeClip("Pinky_Attacked");
		break;
	case PLAYER::RAFF:
		pAnimation->ChangeClip("Raff_Attacked");
		break;
	}

	if (_pPlayer->GetCurHp() <= 0.f && _pPlayer->IsInvicible() == false)
	{
		_pPlayer->SetCurHp(0.f);
		_pPlayer->SetIsDead(true);
		_pPlayer->SetIsAttacked(false);
		SetPlayerState(_pPlayer, GET_SINGLE(PlayerDeath));
		return;
	}

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
	
	CShadow::RenderShadow(_pPlayer);
}


void PlayerDeath::Update(CPlayer* _pPlayer)
{
	CAnimation* pAnimation = _pPlayer->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());


	PLAYER::ID ePlayerID = GET_SINGLE(CSceneManager)->GetPlayerID();
	switch (ePlayerID)
	{
	case PLAYER::JIMMY:
		pAnimation->ChangeClip("Death");
		break;
	case PLAYER::PINKY:
		pAnimation->ChangeClip("Pinky_Death");
		break;
	case PLAYER::RAFF:
		pAnimation->ChangeClip("Raff_Death");
		break;
	}


	m_fCoolTime = 1.1f;
	m_fStackTime += CTimeManager::Get_Instance()->GetElapsedTime();
	if (m_fStackTime > m_fCoolTime)
	{
		m_fStackTime = 0.f;
		GET_SINGLE(CSceneManager)->SetIsChangeScene(true);
	}

}


void PlayerDeath::Render(CPlayer* _pPlayer)
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

	CShadow::RenderShadow(_pPlayer);
}
