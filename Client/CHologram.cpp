#include "pch.h"
#include "CHologram.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CPlayerManager.h"
#include "CInteractionManager.h"

CHologram::CHologram(float _fX, float _fY, float _fWidth, float _fHeight, const wstring& _strName)
{
	m_iDrawID = 0;
	m_fStackTime = 0.f;
	m_strName = _strName;

	m_tInfo.vPos = { _fX, _fY, 0.f };
	m_tInfo.vSize = { _fWidth, _fHeight, 1.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vImageSize = { 0.f, 0.f, 0.f };

	if (m_strName == L"Jimmy")
		m_ePlayerID = PLAYER::JIMMY;
	else if(m_strName == L"Pinky")
		m_ePlayerID = PLAYER::PINKY;
	else if (m_strName == L"Raff")
		m_ePlayerID = PLAYER::RAFF;

}

CHologram::~CHologram()
{
	Release();
}

void CHologram::Ready(void)
{
}

int CHologram::Update(float _fDeltaTime)
{
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 0.1f)
	{
		m_iDrawID++;
		m_fStackTime = 0.f;
	}

	if (m_iDrawID >= 26)
		m_iDrawID = 0;

	// 캐릭터 바꾸게

	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	if (CInteractionManager::InteractPlayerHologram(pPlayer, this) == true)
	{
		if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_E))
		{
			switch (m_ePlayerID)
			{
			case PLAYER::JIMMY:
				GET_SINGLE(CSceneManager)->SetPlayerID(PLAYER::JIMMY);
				break;
			case PLAYER::PINKY:
				GET_SINGLE(CSceneManager)->SetPlayerID(PLAYER::PINKY);
				break;
			case PLAYER::RAFF:
				GET_SINGLE(CSceneManager)->SetPlayerID(PLAYER::RAFF);
				break;
			}
			
		}
	}


    return 0;
}

void CHologram::LateUpdate(void)
{
	// 테스트 씬이 아니면 삭제
	if (GET_SINGLE(CSceneManager)->GetCurSceneID() != CSceneManager::SCENE_TEST)
		this->SetIsValid(false);
}

void CHologram::Release(void)
{
}

void CHologram::Render(const HDC& _hdc)
{
	DrawAltar();
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Hologram", m_strName, m_iDrawID);
	if (pTexInfo == nullptr)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 상호작용
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	if (CInteractionManager::InteractPlayerHologram(pPlayer, this) == true)
	{
		m_fHoloStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fHoloStackTime >= 0.2f)
		{
			m_fHoloStackTime = 0.f;
			this->SetInteractDrawID(this->GetInteractDrawID() + 1);
		}

		CInteractionManager::RenderHologramVer(this);
	}

}

void CHologram::DrawAltar(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Altar");
	if (pTexInfo == nullptr)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y + 50.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}
