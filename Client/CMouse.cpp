#include "pch.h"
#include "CMouse.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CMonster.h"
#include "CObjManager.h"
#include "CKeyManager.h"
#include "CPlayerManager.h"
#include "CCameraManager.h"
#include "CPlayer.h"
#include "CTimeManager.h"

void CMouse::Ready(void)
{
	ShowCursor(false);
	//마우스 크기
	m_tInfo.vPos  = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 42.f, 42.f, 0.f };
	m_tInfo.vDir  = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vImageSize = { 0.f, 0.f, 0.f };
}

int CMouse::Update(float _fDeltaTime)
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	float fPlayerX = 0.f;
	float fPlayerY = 0.f;
	if (GET_SINGLE(CPlayerManager)->GetPlayer() != nullptr)
	{
		fPlayerX = GET_SINGLE(CPlayerManager)->GetPlayer()->GetX();
		fPlayerY = GET_SINGLE(CPlayerManager)->GetPlayer()->GetY();
		fPlayerX -= float(WINCX >> 1) + GET_SINGLE(CCameraManager)->GetCameraDeltaX();
		fPlayerY -= float(WINCY >> 1) + GET_SINGLE(CCameraManager)->GetCameraDeltaY();

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(GET_SINGLE(CPlayerManager)->GetPlayer());
		if (pPlayer->IsReloading() == true)
		{
			m_iDrawID = 2; // 장전하면 드로우 아이디 바까줌
			m_bIsReloading = true;
			m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
			if (m_fStackTime >= 0.1f)
			{
				m_iReloadID++;
				m_fStackTime = 0.f;
			}

			if (m_iReloadID >= 12)
			{
				// 장전끝
				m_iReloadID = 0;
				m_bIsReloading = false;
				pPlayer->SetIsReloading(false);
			}
		}

	}

	m_tInfo.vPos.x = float(pt.x) + fPlayerX;
	m_tInfo.vPos.y = float(pt.y) + fPlayerY;

	float fX = m_tInfo.vPos.x;
	float fY = m_tInfo.vPos.y;
	float fWidth  = m_tInfo.vSize.x;
	float fHeight = m_tInfo.vSize.y;

	if (GET_SINGLE(CKeyManager)->Key_Pressing(KEY_LBUTTON) && m_bIsReloading == false)
	{		
		m_bIsPressing = true;
	}

	for (auto& pMonster : GET_SINGLE(CObjManager)->GetMonsters())
	{
		if (m_bIsReloading == true)
			return 0;

		if (pMonster->GetX() < fX + (fWidth * 0.5f) + 10.f && fX - (fWidth * 0.5f) - 10.f < pMonster->GetX() &&
			pMonster->GetY() < fY + (fHeight * 0.5f) + 10.f && fY - (fHeight * 0.5f) - 10.f < pMonster->GetY())
		{
			m_iDrawID = 1;
			return 0;
		}
	}

	m_iDrawID = 0;

    return 0;
}

void CMouse::LateUpdate(void)
{
	if (m_bIsPressing == true)
		ToEnlargeScale();

	if (m_bIsReduceScale == true)
		ToReduceScale();
}

void CMouse::Release(void)
{
}

void CMouse::Render(const HDC& _hdc)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Mouse", L"CrossHair", m_iDrawID);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f * m_fScale, 1.f * m_fScale, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_bIsReloading == true)
	{
		pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Mouse", L"Reload", m_iReloadID);
		if (nullptr == pTexInfo)
			return;
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.6f * m_fScale , 1.6f * m_fScale, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		float fPlayerX = 0.f;
		float fPlayerY = 0.f;
		fPlayerX = GET_SINGLE(CPlayerManager)->GetPlayer()->GetX();
		fPlayerY = GET_SINGLE(CPlayerManager)->GetPlayer()->GetY();
		pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Mouse", L"ReloadChar", m_iReloadID);
		if (nullptr == pTexInfo)
			return;
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.f * m_fScale, 1.f * m_fScale, 0.f);
		D3DXMatrixTranslation(&matTrans, fPlayerX, fPlayerY + 35.f, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}

}

void CMouse::ToEnlargeScale(void)
{
	if (m_fScale >= 1.4f)
	{
		m_fScale = 1.4f;
		m_bIsReduceScale = true;
		return;
	}

	m_fScale += 0.1f;
}

void CMouse::ToReduceScale(void)
{
	m_bIsPressing = false;
	if (m_fScale <= 1.f)
	{
		m_fScale = 1.f;
		m_bIsReduceScale = false;
		return;
	}

	m_fScale -= 0.1f;
}
