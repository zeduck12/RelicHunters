#include "pch.h"
#include "CItemState.h"
#include "CItem.h"
#include "CPlayer.h"
#include "CPlayerManager.h"
#include "CAnimation.h"
#include "CTimeManager.h"
#include "CGraphicDevice.h"
#include "CCollisionManager.h"


CItemState* CItemIdleState::Update(CItem* _pItem)
{
	CAnimation* pAnimation = _pItem->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Idle");

	if (_pItem->GetImageID() == IMAGE::COIN)
	{
		float fWidth  = 160.f;
		float fHeight = 160.f;

		CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
		if (pPlayer->GetX() < _pItem->GetX() + fWidth && _pItem->GetX() - fWidth < pPlayer->GetX() &&
			pPlayer->GetY() < _pItem->GetY() + fHeight && _pItem->GetY() - fHeight < pPlayer->GetY())
		{
			m_fAddSpeed += 0.6f;
			// 플레이어 쪽으로 빨려가게
			D3DXVECTOR3 vDist = pPlayer->GetInfo()->vPos - _pItem->GetInfo()->vPos;
			D3DXVec3Normalize(&vDist, &vDist);

			_pItem->SetPosVector(_pItem->GetPosVector() + vDist * m_fAddSpeed);

			// 플레이어와 부딫히면 사라지게
			CCollisionManager::CollideBullet(pPlayer, _pItem);
		}
		
	}

	return nullptr;
}

void CItemIdleState::Render(CItem* _pItem)
{
	// 애니메이션 출력
	int iFrame = 0;
	CAnimation* pAnimation = _pItem->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pItem->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, _pItem->GetInfo()->vPos.x, _pItem->GetInfo()->vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

CItemState* CItemStartState::Update(CItem* _pItem)
{
	CAnimation* pAnimation = _pItem->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Start");

	m_fCoolTime = 2.f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime > m_fCoolTime)
	{
		m_fStackTime = 0.f;
		_pItem->SetIsValid(false);
		return nullptr;
	}

	return nullptr;
}

void CItemStartState::Render(CItem* _pItem)
{
	// 애니메이션 출력
	int iFrame = 0;
	CAnimation* pAnimation = _pItem->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pItem->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, _pItem->GetInfo()->vPos.x, _pItem->GetInfo()->vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

CItemState* CItemDropState::Update(CItem* _pItem)
{
	CAnimation* pAnimation = _pItem->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Drop");

	CStarCoin* pCoin = dynamic_cast<CStarCoin*>(_pItem);
	_pItem->SetX(_pItem->GetX() + cosf(D3DXToRadian(pCoin->GetDropDegree())) * 2.f );
	_pItem->SetY(_pItem->GetY() + sinf(D3DXToRadian(pCoin->GetDropDegree())) * 2.f);

	// 중력작용
	ActiveGravity();
	_pItem->SetY(_pItem->GetY() + m_fGravity);

	if (m_fGravity >= 4.f)
		return new CItemIdleState;

	return nullptr;
}

void CItemDropState::Render(CItem* _pItem)
{
	// 애니메이션 출력
	int iFrame = 0;
	CAnimation* pAnimation = _pItem->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pItem->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, _pItem->GetInfo()->vPos.x, _pItem->GetInfo()->vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CItemDropState::ActiveGravity(void)
{
	if (m_fGravity >= 10.f)
		return;
	m_fGravity += 0.1f;
}

CItemState* CItemSpawnState::Update(CItem* _pItem)
{
	CAnimation* pAnimation = _pItem->GetAnimation();
	if (pAnimation)
		pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	pAnimation->ChangeClip("Spawn");

	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 1.1f)
		return new CItemIdleState;

	return nullptr;
}

void CItemSpawnState::Render(CItem* _pItem)
{
	// 애니메이션 출력
	int iFrame = 0;
	CAnimation* pAnimation = _pItem->GetAnimation();
	if (pAnimation)
	{
		ANIMATION_CLIP* pClip = pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // 인덱스
	}

	TEXINFO* pTexInfo = _pItem->GetTextureInfo()[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, _pItem->GetInfo()->vPos.x, _pItem->GetInfo()->vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}
