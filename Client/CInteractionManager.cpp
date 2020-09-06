#include "pch.h"
#include "CInteractionManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CTimeManager.h"
#include "CItem.h"
#include "CMonster.h"
#include "CHologram.h"

float CInteractionManager::m_fX = 0.f;
float CInteractionManager::m_fY = 0.f;
int   CInteractionManager::m_iDrawID = 0;
int   CInteractionManager::m_iMaxDrawID = 7;
float CInteractionManager::m_fStackTime = 0.f;
float CInteractionManager::m_fCoolTime = 0.2f;

void CInteractionManager::Render(CObj* _pItem)
{
	CItem* pItem = dynamic_cast<CItem*>(_pItem);

	if (pItem->GetDrawID() >= m_iMaxDrawID)
	{
		pItem->SetDrawID(0);
		return;
	}

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Prompt", L"PromptE", pItem->GetDrawID());
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_fX, m_fY - 30.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CInteractionManager::RenderHologramVer(CObj* _pHologram)
{
	CHologram* pHologram= dynamic_cast<CHologram*>(_pHologram);

	if (pHologram->GetInteractDrawID() >= m_iMaxDrawID)
	{
		pHologram->SetInteractDrawID(0);
		return;
	}

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Prompt", L"PromptE", pHologram->GetInteractDrawID());
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_fX, m_fY - 30.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CInteractionManager::RenderActivation(CObj* _pMonster)
{
	DO_IF_IS_NOT_VALID_OBJ(_pMonster)
		return;

	CMonster* pMonster= dynamic_cast<CMonster*>(_pMonster);
	int iDrawID = pMonster->GetActivationDrawID();
	if (iDrawID >= 7)
		return;

	float fMonsterX = pMonster->GetX();
	float fMonsterY = pMonster->GetY();
	float fMonsterWidth = pMonster->GetWidth();
	float fMonsterHeight = pMonster->GetHeight();

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Activation", L"Activation", iDrawID);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, fMonsterX, fMonsterY - 50.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pMonster->SetActivationDrawID(iDrawID + 1);
}


// 아이템 범위안에 플레이어가 들어온다면 상호작용 표시뜨게 하기
bool CInteractionManager::InteractPlayerItem(CObj* _pPlayer, CObj* _pItem)
{
	DO_IF_IS_NOT_VALID_OBJ(_pPlayer)
		return false;
	DO_IF_IS_NOT_VALID_OBJ(_pItem)
		return false;

	CItem* pItem = dynamic_cast<CItem*>(_pItem);

	float fPlayerX    = _pPlayer->GetX();
	float fPlayerY    = _pPlayer->GetY();
	float fItemX	  = _pItem->GetX();
	float fItemY	  = _pItem->GetY();
	float fItemWidth  = _pItem->GetWidth();
	float fItemHeight = _pItem->GetHeight();

	m_fX = fItemX;
	m_fY = fItemY;

	if (fItemX - (fItemWidth  * 2.f) > fPlayerX || fPlayerX > fItemX + (fItemWidth  * 2.f) ||
		fItemY - (fItemHeight * 2.5f) > fPlayerY || fPlayerY > fItemY + (fItemHeight * 2.5f))
	{
		pItem->SetDrawID(0);
		return false;
	}

	// 아이템 범위내에 있으면 상호작용 TRUE !
	return true;
}

bool CInteractionManager::InteractPlayerHologram(CObj* _pPlayer, CObj* _pHologram)
{
	DO_IF_IS_NOT_VALID_OBJ(_pPlayer)
		return false;
	DO_IF_IS_NOT_VALID_OBJ(_pHologram)
		return false;

	CHologram* pItem = dynamic_cast<CHologram*>(_pHologram);

	float fPlayerX = _pPlayer->GetX();
	float fPlayerY = _pPlayer->GetY();
	float fItemX = _pHologram->GetX();
	float fItemY = _pHologram->GetY();
	float fItemWidth = _pHologram->GetWidth();
	float fItemHeight = _pHologram->GetHeight();

	m_fX = fItemX;
	m_fY = fItemY;

	if (fItemX - (fItemWidth * 2.f) > fPlayerX || fPlayerX > fItemX + (fItemWidth * 2.f) ||
		fItemY - (fItemHeight * 2.5f) > fPlayerY || fPlayerY > fItemY + (fItemHeight * 2.5f))
	{
		pItem->SetInteractDrawID(0);
		return false;
	}

	return true;
}
