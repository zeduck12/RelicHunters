#include "pch.h"
#include "CBossHpBar.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CObjManager.h"
#include "CTimeManager.h"

CBossHpBar::CBossHpBar()
{
	m_tInfo.vPos = { 400.f, 500.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 1.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vImageSize = { 0.f, 0.f, 0.f };
	m_fValue = 0.f;
	m_fStackTime = 0.f;
	m_bIsFinish = false;
}

CBossHpBar::CBossHpBar(CBoss* _pOwner)
{
	// HP Bar 주인 셋팅
	m_pOwner = _pOwner;

	m_tInfo.vPos = { 400.f, 500.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 1.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vImageSize = { 0.f, 0.f, 0.f };

	m_fValue = 0.f;
	m_fStackTime = 0.f;
	m_bIsFinish = false;

}

void CBossHpBar::Ready(void)
{
}

int CBossHpBar::Update(float _fDeltaTime)
{
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 0.01f)
	{
		m_fValue += 10.f;
		m_fStackTime = 0.f;
	}

	if (m_fValue >= 1000.f)
	{
		m_fValue = 1000.f;
		m_bIsFinish = true;
	}

	if (m_pOwner == nullptr || m_pOwner->GetHp() <= 0.f)
		this->SetIsValid(false);

	return 0;
}

void CBossHpBar::LateUpdate(void)
{
}

void CBossHpBar::Release(void)
{
}

void CBossHpBar::StartDrawHpBar()
{
	RECT rc = { 0, 0, LONG(81 * (m_fValue / 1000.f)), 18 };

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"DashBar", L"DashBar", 0);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 5.f, 2.3f, 0.f);
	D3DXMatrixTranslation(&matTrans, 555.f, 465.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, &rc, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(100, 0, 0, 0));

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"DashBar", L"DashBar", 0);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);


	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 5.f, 2.3f, 0.f);
	D3DXMatrixTranslation(&matTrans, 550.f, 470.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, &rc, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 0, 70));

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Emoticon", L"Boss", 0);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.3f, 1.3f, 0.f);
	D3DXMatrixTranslation(&matTrans, 330.f, 460.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CBossHpBar::Render_HpBar(void)
{
	float fCurHp = 0.f;
	fCurHp = m_pOwner->GetHp();

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"DashBar", L"DashBar", 0);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 5.f, 2.3f, 0.f);
	D3DXMatrixTranslation(&matTrans, 555.f, 465.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(100, 0, 0, 0));

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"DashBar", L"DashBar", 0);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);


	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 5.f, 2.3f, 0.f);
	D3DXMatrixTranslation(&matTrans, 550.f, 470.f, 0.f);
	matWorld = matScale * matTrans;


	RECT rc = { 0, 0, LONG(81 * (fCurHp / m_pOwner->GetMaxHp())), 18 };

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, &rc, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 0, 70));

	int iDrawID = 0;
	if (fCurHp < m_pOwner->GetMaxHp() * 0.5f)
		iDrawID = 1;
	else 
		iDrawID = 0;

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Emoticon", L"Boss", iDrawID);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.3f, 1.3f, 0.f);
	D3DXMatrixTranslation(&matTrans, 330.f, 460.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}
