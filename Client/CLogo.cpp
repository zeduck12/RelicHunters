#include "pch.h"
#include "CLogo.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CTimeManager.h"

CLogo::CLogo()
{
	m_fStackTime = 0.f;
}

CLogo::~CLogo()
{
}

bool CLogo::Ready(void)
{
	return true;
}

void CLogo::Update(void)
{
	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_SPACE))
	{
		// ÀÎÆ®·Î
		GET_SINGLE(CSceneManager)->ChangeScene(CSceneManager::SCENE_INTRO);
	}

}

void CLogo::LateUpdate(void)
{
}

void CLogo::Render(const HDC& _hdc)
{
	GET_SINGLE(CGraphicDevice)->RenderBegin();

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Logo");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, float(1280 / WINCX), float(720 / WINCY), 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 0.5f)
	{
		pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"LogoText");
		if (nullptr == pTexInfo)
			return;

		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


	GET_SINGLE(CGraphicDevice)->GetSprite()->End();
	GET_SINGLE(CGraphicDevice)->RenderEnd();
}

void CLogo::Release(void)
{
}
