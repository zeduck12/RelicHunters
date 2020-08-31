#include "pch.h"
#include "CStatBox.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CSelButton.h"
#include "CStatBoxButton.h"

CStatBox::CStatBox(CSelButton* _pOwner)
{
	m_pOwner = _pOwner;
	m_pStartBtn  = new CStatBoxButton(this, 330.f, 550.f, BUTTON::START);
	m_pChangeBtn = new CStatBoxButton(this, 510.f, 570.f, BUTTON::CHANGE);
}

CStatBox::~CStatBox()
{
	Release();
}

void CStatBox::Ready(void)
{
}

int CStatBox::Update(float _fDeltaTime)
{
	if (m_pStartBtn)
		m_pStartBtn->Update();

	if (m_pChangeBtn)
		m_pChangeBtn->Update();

	return 0;
}

void CStatBox::LateUpdate(void)
{
}

void CStatBox::Release(void)
{
	Safe_Delete(m_pStartBtn);
	Safe_Delete(m_pChangeBtn);
}

void CStatBox::Render(const HDC& _hdc)
{
	DrawBackground();
	DrawCharacterCard();
	DrawStatBackground();

	if (m_pStartBtn)
		m_pStartBtn->Render(_hdc);

	if (m_pChangeBtn)
		m_pChangeBtn->Render(_hdc);
}

void CStatBox::DrawBackground(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"PlayerCard");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1) + 130.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	if (m_pOwner->GetSelID() == BUTTON::JIMMY)
	{
		TCHAR szBuf[MAX_PATH] = L"P1            JIMMY";
		D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 130.f, float(WINCY >> 1) - 5.f , 0.f);
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	
	if (m_pOwner->GetSelID() == BUTTON::PINKY)
	{
		TCHAR szBuf[MAX_PATH] = L"P1            PINKY";
		D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 130.f, float(WINCY >> 1) - 5.f, 0.f);
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_pOwner->GetSelID() == BUTTON::RAFF)
	{
		TCHAR szBuf[MAX_PATH] = L"P1             RAFF";
		D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 130.f, float(WINCY >> 1) - 5.f, 0.f);
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_pOwner->GetSelID() == BUTTON::RANDOM)
	{
		TCHAR szBuf[MAX_PATH] = L"P1           RANDOM";
		D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 130.f, float(WINCY >> 1) - 5.f, 0.f);
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


}

void CStatBox::DrawStatBackground(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"PlayerStat");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 80.f, float(WINCY >> 1) + 150.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR szBuf[MAX_PATH] = L"HEALTH\nSHIELD";
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 32.f, float(WINCY >> 1) + 35.f, 0.f);
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	TCHAR szBuf2[MAX_PATH] = L"MELEE";
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 32.f, float(WINCY >> 1) + 87.f, 0.f);
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf2, lstrlen(szBuf2), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR szBuf3[MAX_PATH] = L"SPEED";
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 32.f, float(WINCY >> 1) + 113.f, 0.f);
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf3, lstrlen(szBuf3), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));

	DrawStarBar();
}

void CStatBox::DrawCharacterCard(void)
{
	if (m_pOwner->GetSelID() == BUTTON::JIMMY)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"CharacterCard", 2);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 145.f, float(WINCY >> 1) + 140.f, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_pOwner->GetSelID() == BUTTON::PINKY)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"CharacterCard", 3);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 145.f, float(WINCY >> 1) + 140.f, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_pOwner->GetSelID() == BUTTON::RAFF)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"CharacterCard", 5);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 145.f, float(WINCY >> 1) + 140.f, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CStatBox::DrawStarBar(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"StarBar", 0);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.65f, 0.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 130.f, float(WINCY >> 1) + 48.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"StarBar", 1);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.65f, 0.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 150.f, float(WINCY >> 1) + 48.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"StarBar", 2);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.65f, 0.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 170.f, float(WINCY >> 1) + 48.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"StarBar", 0);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.65f, 0.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 130.f, float(WINCY >> 1) + 74.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"StarBar", 1);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.65f, 0.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 150.f, float(WINCY >> 1) + 74.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"StarBar", 2);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.65f, 0.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 170.f, float(WINCY >> 1) + 74.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"StarBar", 0);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.65f, 0.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 130.f, float(WINCY >> 1) + 100.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"StarBar", 1);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.65f, 0.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 150.f, float(WINCY >> 1) + 100.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"StarBar", 2);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.65f, 0.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 170.f, float(WINCY >> 1) + 100.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"StarBar", 0);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.65f, 0.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 130.f, float(WINCY >> 1) + 126.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"StarBar", 1);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.65f, 0.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 150.f, float(WINCY >> 1) + 126.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"StarBar", 2);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.65f, 0.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 170.f, float(WINCY >> 1) + 126.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}
