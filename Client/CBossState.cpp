#include "pch.h"
#include "CBossState.h"
#include "CAnimation.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CTimeManager.h"
#include "CShadow.h"
#include "CPlayerManager.h"

CBossState* EggIdleState::Update(CBoss* _pBoss)
{
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("EggIdle");

	if (_pBoss->IsDetectPlayer() == true)
		return new EggStartState;

	return nullptr;
}

void EggIdleState::Render(CBoss* _pBoss)
{
	// 애니메이션 출력
	int iFrame = 0;
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pBoss->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pBoss->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pBoss->GetInfo()->vPos.x, _pBoss->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pBoss);
}

CBossState* EggStartState::Update(CBoss* _pBoss)
{
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("EggStart");

	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 10.f)
		return new EggCrackState;

	return nullptr;
}

void EggStartState::Render(CBoss* _pBoss)
{
	// 애니메이션 출력
	int iFrame = 0;
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pBoss->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pBoss->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pBoss->GetInfo()->vPos.x, _pBoss->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pBoss);
}

CBossState* EggCrackState::Update(CBoss* _pBoss)
{
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("EggCrack");

	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 4.1f)
	{
		_pBoss->SetIsCrack(true);
		return new BossIdleState;
	}

	return nullptr;
}

void EggCrackState::Render(CBoss* _pBoss)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pBoss->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pBoss->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pBoss->GetInfo()->vPos.x, _pBoss->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pBoss);
}

//알
////////////////////////////

CBossState* BossIdleState::Update(CBoss* _pBoss)
{
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Idle");

	_pBoss->SetIsDash(false);
	_pBoss->SetAddSpeed(0.f);

	// 만약에 플레이어를 감지했다면
	if (_pBoss->IsDetectPlayerBossVersion() == true)
		return new BossMoveState;

	return nullptr;
}

void BossIdleState::Render(CBoss* _pBoss)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pBoss->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pBoss->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pBoss->GetInfo()->vPos.x, _pBoss->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pBoss);
}

CBossState* BossMoveState::Update(CBoss* _pBoss)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Move");

	// 만약에 플레이어가 감지 범위를 벗어나면 다시 Idle 상태로
	if (_pBoss->IsDetectPlayerBossVersion() == false)
		return new BossIdleState;

	// 만약에 공격범위 안에 들어온다면 AttackState로
	if (_pBoss->IsInAttackRangePlayerBossVersion() == true)
	{
		// 플레이어 방향으로 방향벡터 구함
		D3DXVECTOR3 vDir = pPlayer->GetInfo()->vPos - _pBoss->GetInfo()->vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		_pBoss->SetDirectionVector(vDir);

		if (_pBoss->IsPhase2() == false)
		{
			int iRandNum = rand() % 3 + 1;
			if (iRandNum > 2)
				return new BossDashAttack;
			else
				return new BossAttackState;
		}
		else
		{
			int iRandNum = rand() % 3 + 1;
			if (iRandNum == 1)
				return new BossRapidAttack;
			else if(iRandNum == 2)
				return new BossRocketAttackState;
			else
				return new BossRandomAttackState;
		}

	}

	// 방향벡터 구함.
	D3DXVECTOR3 vDir = pPlayer->GetInfo()->vPos - _pBoss->GetInfo()->vPos;
	D3DXVec3Normalize(&vDir, &vDir);
	_pBoss->SetDirectionVector(vDir);
	// 이동
	D3DXVECTOR3 vMoveResult = _pBoss->GetPosVector() + _pBoss->GetDirectionVector();
	_pBoss->SetPosVector(vMoveResult);

	return nullptr;
}

void BossMoveState::Render(CBoss* _pBoss)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pBoss->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pBoss->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pBoss->GetInfo()->vPos.x, _pBoss->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pBoss);
}

CBossState* BossAttackedState::Update(CBoss* _pBoss)
{
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Attacked");

	if (_pBoss->GetHp() <= 0.f)
	{
		_pBoss->SetHp(0.f);
		_pBoss->SetIsDead(true);
		return new BossDeathState;
	}

	m_fCoolTime = 0.4f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= m_fCoolTime)
		return new BossIdleState;

	return nullptr;
}

void BossAttackedState::Render(CBoss* _pBoss)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pBoss->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pBoss->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pBoss->GetInfo()->vPos.x, _pBoss->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pBoss);
}

CBossState* BossDeathState::Update(CBoss* _pBoss)
{
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Death");

	m_fCoolTime = 1.3f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= m_fCoolTime)
	{
		m_fStackTime = 0.f;
		_pBoss->SetIsValid(false);
	}

	return nullptr;
}

void BossDeathState::Render(CBoss* _pBoss)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pBoss->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pBoss->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pBoss->GetInfo()->vPos.x, _pBoss->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pBoss);
}

CBossState* BossAttackState::Update(CBoss* _pBoss)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Idle");

	// 만약에 공격범위를 벗어났다면 다시 IdleState로
	if (_pBoss->IsInAttackRangePlayerBossVersion() == false)
		return new BossIdleState;

	
	int iRandNum = rand() % 2 + 1;
	if (m_iCount >= 4)
	{
		if (iRandNum == 1)
		{
			m_iCount = 0;
			m_fStackTime = 0.f;
			_pBoss->ShootShotgun();
			return new BossShotGunState;
		}
		if (iRandNum == 2)
		{
			m_iCount = 0;
			m_fStackTime = 0.f;
			D3DXVECTOR3 vDir = pPlayer->GetInfo()->vPos - _pBoss->GetInfo()->vPos;
			D3DXVec3Normalize(&vDir, &vDir);
			_pBoss->SetDirectionVector(vDir);
			return new BossDashAttack;
		}
	}

	m_fCoolTime = 0.3f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= m_fCoolTime)
	{
		m_iCount++;
		m_fStackTime = 0.f;
		_pBoss->Shoot();
	}

	return nullptr;
}

void BossAttackState::Render(CBoss* _pBoss)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pBoss->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pBoss->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pBoss->GetInfo()->vPos.x, _pBoss->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pBoss);
}

CBossState* BossRocketAttackState::Update(CBoss* _pBoss)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Idle");

	// 만약에 공격범위를 벗어났다면 다시 IdleState로
	if (_pBoss->IsInAttackRangePlayerBossVersion() == false)
		return new BossIdleState;

	m_fCoolTime = 1.f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= m_fCoolTime)
	{
		if (m_iCount >= 5)
		{
			m_iCount = 0;
			m_fStackTime = 0.f;
			return new BossIdleState;
		}

		m_iCount++;
		m_fStackTime = 0.f;
		_pBoss->ShootRocket();
	}


	return nullptr;
}

void BossRocketAttackState::Render(CBoss* _pBoss)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pBoss->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pBoss->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pBoss->GetInfo()->vPos.x, _pBoss->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pBoss);
}

CBossState* BossShotGunState::Update(CBoss* _pBoss)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Idle");

	// 만약에 공격범위를 벗어났다면 다시 IdleState로
	if (_pBoss->IsInAttackRangePlayerBossVersion() == false)
		return new BossIdleState;

	m_fCoolTime = 1.5f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= m_fCoolTime)
	{
		if (m_iCount >= 5)
		{
			m_iCount = 0;
			m_fStackTime = 0.f;
			return new BossAttackState;
		}

		m_iCount++;
		m_fStackTime = 0.f;
		_pBoss->ShootShotgun();
	}

	return nullptr;
}

void BossShotGunState::Render(CBoss* _pBoss)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pBoss->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pBoss->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pBoss->GetInfo()->vPos.x, _pBoss->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pBoss);
}

CBossState* BossRapidAttack::Update(CBoss* _pBoss)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Idle");

	m_fCoolTime = 0.1f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= m_fCoolTime)
	{
		if (m_iCount >= 20)
		{
			m_iCount = 0;
			m_fStackTime = 0.f;
			return new BossDashAttack;
		}

		m_iCount++;
		m_fStackTime = 0.f;
		_pBoss->Shoot();
	}

	return nullptr;
}

void BossRapidAttack::Render(CBoss* _pBoss)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pBoss->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pBoss->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pBoss->GetInfo()->vPos.x, _pBoss->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pBoss);
}

CBossState* BossDashAttack::Update(CBoss* _pBoss)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Idle");

	m_fCoolTime = 1.f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= m_fCoolTime)
	{
		m_fStackTime = 0.f;
		return new BossFullRangeAttack;
	}

	_pBoss->SetIsDash(true);

	return nullptr;
}

void BossDashAttack::Render(CBoss* _pBoss)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pBoss->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pBoss->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pBoss->GetInfo()->vPos.x, _pBoss->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pBoss);
}

CBossState* BossRandomAttackState::Update(CBoss* _pBoss)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Idle");

	// 만약에 공격범위를 벗어났다면 다시 IdleState로
	if (_pBoss->IsInAttackRangePlayerBossVersion() == false)
		return new BossIdleState;

	m_fCoolTime  += GET_SINGLE(CTimeManager)->GetElapsedTime();
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if(m_fCoolTime >= 3.f)
		return new BossFullRangeAttack;

	if (m_fStackTime >= 0.12f)
	{
		m_iCount++;
		m_fStackTime = 0.f;
		_pBoss->Shoot();
		_pBoss->Shoot();
	}

	return nullptr;
}

void BossRandomAttackState::Render(CBoss* _pBoss)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pBoss->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pBoss->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pBoss->GetInfo()->vPos.x, _pBoss->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pBoss);
}

CBossState* BossFullRangeAttack::Update(CBoss* _pBoss)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Idle");

	m_fCoolTime  += GET_SINGLE(CTimeManager)->GetElapsedTime();
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();

	if(m_fCoolTime >= 1.f)
		return  new BossIdleState;

	if (m_fStackTime >= 0.5f)
	{
		m_fStackTime = 0.f;
		_pBoss->FullRangeAttack();
	}

	return  nullptr;
}

void BossFullRangeAttack::Render(CBoss* _pBoss)
{
	int iFrame = 0;
	CAnimation* pAnimation = _pBoss->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pBoss->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (_pBoss->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, _pBoss->GetInfo()->vPos.x, _pBoss->GetInfo()->vPos.y - 20, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderShadow(_pBoss);
}
