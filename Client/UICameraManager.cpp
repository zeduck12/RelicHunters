#include "pch.h"
#include "UICameraManager.h"
#include "CSceneManager.h"
#include "CPlayerInfo.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CTimeManager.h"

DEFINITION_SINGLETON(UICameraManager)

bool UICameraManager::Ready(void)
{
	m_pPlayerInfo = make_unique<CPlayerInfo>(L"Jimmy");
	if (!m_pPlayerInfo->Ready())
		return false;

	m_bIsFinish = false;
	m_fStartTime = 0.f;
	m_fStackTime = 0.f;

	return true;
}

void UICameraManager::Update(void)
{
	m_pPlayerInfo->Update();

	m_fStartTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStartTime >= 1.f)
	{
		m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fStackTime >= 0.1f)
		{
			m_iDrawID++;
			m_fStackTime = 0.f;
		}

		if (m_iDrawID >= 13)
		{
			m_iDrawID = 0;
			m_fStackTime = 0.f;
			m_bIsFinish = true;
		}
	}
	
}

void UICameraManager::LateUpdate(void)
{
}

void UICameraManager::Render(void)
{
	m_pPlayerInfo->Render();
	if (m_fStartTime >= 1.f)
		DrawStageTitle();
}

void UICameraManager::DrawStageTitle(void)
{
	if (m_bIsFinish == true)
		return;

	CSceneManager::ID eSceneID = GET_SINGLE(CSceneManager)->GetCurSceneID();
	switch (eSceneID)
	{
	case CSceneManager::SCENE_GAME:
		m_strName = L"Stage1";
		break;
	case CSceneManager::SCENE_GAME2:
		m_strName = L"Stage2";
		break;
	case CSceneManager::SCENE_GAME3:
		m_strName = L"Stage3";
		break;
	case CSceneManager::SCENE_GAME4:
		m_strName = L"Stage4";
		break;
	}

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Stage", m_strName, m_iDrawID);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}
