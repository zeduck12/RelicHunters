#include "pch.h"
#include "CIntro.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CTimeManager.h"

CIntro::CIntro()
{
	m_iDrawID = 0;
	m_fStackTime = 0.f;
}

CIntro::~CIntro()
{
	Release();
}

bool CIntro::Ready(void)
{
	//배경음악
	CSoundManager::Get_Instance()->PlayBGM((TCHAR*)L"bgm_intro.wav");
    return true;
}

void CIntro::Update(void)
{
	
	if (m_iDrawID >= 11)
		m_iDrawID = 0;

	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 0.1f)
	{
		m_iDrawID++;
		m_fStackTime = 0.f;
	}


	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_SPACE))
	{
		// 로비씬으로 
		// 지금은 게임씬으로
		GET_SINGLE(CSceneManager)->ChangeScene(CSceneManager::SCENE_LOBBY);
	}

}

void CIntro::LateUpdate(void)
{
}

void CIntro::Render(const HDC& _hdc)
{
	//GET_SINGLE(CGraphicDevice)->RenderBegin();

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Scene", L"Intro", m_iDrawID);
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

	//GET_SINGLE(CGraphicDevice)->GetSprite()->End();
	//GET_SINGLE(CGraphicDevice)->RenderEnd();
}

void CIntro::Release(void)
{
}
