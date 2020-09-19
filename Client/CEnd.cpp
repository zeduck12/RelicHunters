#include "pch.h"
#include "CEnd.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CTimeManager.h"
#include "CSceneManager.h"

CEnd::CEnd()
{
}

CEnd::~CEnd()
{
	Release();
}

bool CEnd::Ready(void)
{
    return true;
}

void CEnd::Update(void)
{
	if(GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		DestroyWindow(g_hWND);

	if (m_bIsPlayingBGM == false)
	{
		m_bIsPlayingBGM = true;
		CSoundManager::Get_Instance()->StopAll();
		CSoundManager::Get_Instance()->PlayBGM((TCHAR*)L"bgm_victory.wav");

	}

	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 0.05f)
	{
		m_fDeltaY += 6.f;
		m_fStackTime = 0.f;
	}

}

void CEnd::LateUpdate(void)
{
}

void CEnd::Render(const HDC& _hdc)
{
	DrawBackground();
	DrawEndingText();
}

void CEnd::Release(void)
{
}

void CEnd::DrawBackground(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"EndBackground");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	// 800  600
	// 1920 1080
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, float(WINCX / 1920.f), float(WINCY / 1080.f), 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CEnd::DrawEndingText(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"EndText");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	// 800  600
	// 1920 1080
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), 1400.f - m_fDeltaY, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}
