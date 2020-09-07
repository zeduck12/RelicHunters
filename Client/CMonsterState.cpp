#include "pch.h"
#include "CMonsterState.h"
#include "CTimeManager.h"
#include "CPlayerManager.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CAnimation.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CShadow.h"
#include "CObjManager.h"
#include "CItem.h"
#include "CInteractionManager.h"


CMonsterState* IdleState::Update(CMonster* _pMonster)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Idle");

	if (_pMonster->GetImageID() == IMAGE::KAMIKAZE && _pMonster->IsDash() == true)
	{
		m_fCoolTime = 2.f;
		m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fStackTime >= m_fCoolTime)
		{
			m_fStackTime = 0.f;
			_pMonster->SetIsDash(false);
			_pMonster->SetAddSpeed(0.f);
		}
		else
			return nullptr;
	}

	// 만약에 플레이어를 감지했다면
	if (_pMonster->IsDetectPlayer() == true)
		return new TrackingState;

	// 상태 유지
	return nullptr;
}

void IdleState::Render(CMonster* _pMonster)
{
	// 애니메이션 출력
	int iFrame = 0;
	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pMonster->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pMonster->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pMonster->GetInfo()->vPos.x, _pMonster->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pMonster);
	
}

CMonsterState* TrackingState::Update(CMonster* _pMonster)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Move");

	// 초에 한번 확률적으로 
	if (_pMonster->GetImageID() == IMAGE::KAMIKAZE_FLY)
	{
		m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fStackTime > 1.f)
		{
			m_fStackTime = 0.f;
			int iRandNum = rand() % 100 + 1;
			if (iRandNum > 98)
			{
				dynamic_cast<CMonster*>(_pMonster)->SetOldPos(_pMonster->GetInfo()->vPos);
				return new FlyState;
			}
		}
	}

	// 만약에 플레이어가 감지 범위를 벗어나면 다시 Idle 상태로
	if (_pMonster->IsDetectPlayer() == false)
		return new IdleState;

	// 만약에 공격범위 안에 들어온다면 AttackState로
	if (_pMonster->IsInAttackRangePlayer() == true)
	{
		// 플레이어 방향으로 방향벡터 구함
		D3DXVECTOR3 vDir = pPlayer->GetInfo()->vPos - _pMonster->GetInfo()->vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		_pMonster->SetDirectionVector(vDir);
		return new AttackState;
	}

	// 방향벡터 구함.
	D3DXVECTOR3 vDir = pPlayer->GetInfo()->vPos - _pMonster->GetInfo()->vPos;
	D3DXVec3Normalize(&vDir, &vDir);
	_pMonster->SetDirectionVector(vDir);
	// 이동
	D3DXVECTOR3 vMoveResult = _pMonster->GetPosVector() + _pMonster->GetDirectionVector();
	_pMonster->SetPosVector(vMoveResult);

	// 상태 유지
	return nullptr;
}

void TrackingState::Render(CMonster* _pMonster)
{
	// 애니메이션 출력
	int iFrame = 0;
	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pMonster->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pMonster->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pMonster->GetInfo()->vPos.x, _pMonster->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	CShadow::RenderShadow(_pMonster);
	CInteractionManager::RenderActivation(_pMonster);
}

CMonsterState* AttackState::Update(CMonster* _pMonster)
{
	// 공격 종류 (Shoot Type, Crawl Type, Fly Type)
	// Shoot Type

	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Idle");


	// 만약에 공격범위를 벗어났다면 다시 IdleState로
	if (_pMonster->IsInAttackRangePlayer() == false)
		return new IdleState;
	
	// 공격범위에 있다면 Shoot

	if (_pMonster->GetImageID() != IMAGE::KAMIKAZE)
	{
		m_fCoolTime = 0.2f;
		m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fStackTime >= m_fCoolTime)
		{

			m_fStackTime = 0.f;
			_pMonster->Shoot();

			// 확률적으로
			if (_pMonster->GetImageID() == IMAGE::KAMIKAZE_FLY)
			{
				int iRandNum = rand() % 100 + 1;
				if (iRandNum > 98)
				{
					dynamic_cast<CMonster*>(_pMonster)->SetOldPos(_pMonster->GetInfo()->vPos);
					return new FlyState;
				}
			}
		}
	}
	
	if (_pMonster->GetImageID() == IMAGE::KAMIKAZE)
	{
		m_fCoolTime = 0.5f;
		m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fStackTime >= m_fCoolTime)
		{
			m_fStackTime = 0.f;
			return new IdleState;
		}

		_pMonster->SetIsDash(true);
	}



	// 상태 유지
	return nullptr;
}

void AttackState::Render(CMonster* _pMonster)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pMonster->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pMonster->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pMonster->GetInfo()->vPos.x, _pMonster->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	CShadow::RenderShadow(_pMonster);
}

CMonsterState* AttackedState::Update(CMonster* _pMonster)
{
	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Attacked");

	if (_pMonster->GetHp() <= 0.f)
	{
		_pMonster->SetHp(0.f);
		_pMonster->DropItems();
		_pMonster->SetIsDead(true);
		_pMonster->KnockBack();
		return new DeathState;
	}

	m_fCoolTime = 0.4f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= m_fCoolTime)
		return new IdleState;

	return nullptr;
}

void AttackedState::Render(CMonster* _pMonster)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pMonster->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pMonster->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pMonster->GetInfo()->vPos.x, _pMonster->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	CShadow::RenderShadow(_pMonster);
}

CMonsterState* FlyState::Update(CMonster* _pMonster)
{
	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Fly");

	m_fCoolTime = 2.f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= m_fCoolTime)
	{
		dynamic_cast<CMonster*>(_pMonster)->SetGap(dynamic_cast<CMonster*>(_pMonster)->GetOldPos() - _pMonster->GetInfo()->vPos);
		return new PatrolState;
	}

	// 공격을 당하지 않고, 그 자리에서 일정 수치 올라간다음 주위를 몇초동안 순회하다 다시 내려감.
	_pMonster->Fly();
	_pMonster->SetIsFlying(true);

	return nullptr;
}

void FlyState::Render(CMonster* _pMonster)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pMonster->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pMonster->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pMonster->GetInfo()->vPos.x, _pMonster->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	CShadow::RenderFlyShadow(_pMonster);
}

CMonsterState* LandingState::Update(CMonster* _pMonster)
{
	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Fly");

	m_fCoolTime = 1.6f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= m_fCoolTime)
	{
		m_fStackTime = 0.f;
		_pMonster->SetIsFlying(false);
		return new IdleState;
	}

	//착지하기
	_pMonster->Landing();

	return nullptr;
}

void LandingState::Render(CMonster* _pMonster)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pMonster->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pMonster->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pMonster->GetInfo()->vPos.x, _pMonster->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderLandingShadow(_pMonster);
}

CMonsterState* PatrolState::Update(CMonster* _pMonster)
{

	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Fly");

	m_fCoolTime = 10.f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= m_fCoolTime)
	{
		m_fStackTime = 0.f;
		_pMonster->SetStackTime(0.f);
		dynamic_cast<CMonster*>(_pMonster)->SetOldPos(_pMonster->GetInfo()->vPos);
		return new LandingState;
	}

	//착지하기
	_pMonster->Patrol();

	return nullptr;
}

void PatrolState::Render(CMonster* _pMonster)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pMonster->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pMonster->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pMonster->GetInfo()->vPos.x, _pMonster->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	CShadow::RenderPatrolShadow(_pMonster);
}

CMonsterState* DeathState::Update(CMonster* _pMonster)
{
	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Death");

	m_fCoolTime = 1.2f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= m_fCoolTime)
	{
		m_fStackTime = 0.f;
		_pMonster->SetIsValid(false);
	}

	return nullptr;
}

void DeathState::Render(CMonster* _pMonster)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pMonster->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pMonster->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pMonster->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pMonster->GetInfo()->vPos.x, _pMonster->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pMonster);
}
