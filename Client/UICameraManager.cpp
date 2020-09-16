#include "pch.h"
#include "UICameraManager.h"
#include "CSceneManager.h"
#include "CPlayerInfo.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CTimeManager.h"
#include "CBossHpBar.h"
#include "CMapManager.h"
#include "CPlayerManager.h"
#include "CPlayer.h"

DEFINITION_SINGLETON(UICameraManager)

bool UICameraManager::Ready(void)
{
	PLAYER::ID ePlayerID = GET_SINGLE(CSceneManager)->GetPlayerID();
	switch (ePlayerID)
	{
	case PLAYER::JIMMY:
		m_pPlayerInfo = make_unique<CPlayerInfo>(L"Jimmy");
		break;
	case PLAYER::PINKY:
		m_pPlayerInfo = make_unique<CPlayerInfo>(L"Pinky");
		break;
	case PLAYER::RAFF:
		m_pPlayerInfo = make_unique<CPlayerInfo>(L"Raff");
		break;
	}

	if (!m_pPlayerInfo->Ready())
		return false;

	m_bIsFinish = false;
	m_fStartTime = 0.f;
	m_fStackTime = 0.f;
	m_fDeltaY = 0.f;

	return true;
}

void UICameraManager::Update(void)
{
	m_pPlayerInfo->Update();
	if (m_pBossHpBar != nullptr && m_pBossHpBar->IsValid() == true)
		m_pBossHpBar->Update();

	m_fStartTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStartTime >= 1.f)
	{
		m_fDeltaY += 120.f * GET_SINGLE(CTimeManager)->GetElapsedTime();
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
	if(m_bIsShowMiniMap)
		RenderMiniMap();

	m_pPlayerInfo->Render();
	if (m_pBossHpBar != nullptr && m_pBossHpBar->IsValid() == true)
	{
		if(m_pBossHpBar->IsFinish() == true)
			m_pBossHpBar->Render_HpBar();
		else 
			m_pBossHpBar->StartDrawHpBar();
	}

	CSceneManager::ID eSceneID = GET_SINGLE(CSceneManager)->GetCurSceneID();
	if (eSceneID != CSceneManager::SCENE_TEST)
	{
		if (m_fStartTime >= 1.f)
			DrawStageTitle();

		DrawFocusTitle();
	}

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(GET_SINGLE(CPlayerManager)->GetPlayer());
	if (pPlayer->IsSpecialMode() == true)
	{
		// 캐릭터 카드 지나가게
		DrawSpecialFocus();
		DrawSpecialCard();
		DrawSpecialMidCard();
		DrawSpecialText();

		m_fSpecialCheckTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		m_fSpecialCoolTime  += GET_SINGLE(CTimeManager)->GetElapsedTime();

		if (m_fSpecialCoolTime >= 0.5f && m_fSpecialCoolTime <= 0.9f)
		{
			if (m_fSpecialCheckTime > 0.01f)
			{
				m_fSpecialCheckTime = 0.f;
				m_fDeltaX -= 350.f * GET_SINGLE(CTimeManager)->GetElapsedTime();
			}
		}
		else
		{
			if (m_fSpecialCheckTime > 0.01f)
			{
				m_fSpecialCheckTime = 0.f;
				m_fDeltaX -= 1050.f * GET_SINGLE(CTimeManager)->GetElapsedTime();
			}
		}

	}
	else
	{
		m_fDeltaX = 0.f;
		m_fSpecialCheckTime = 0.f;
		m_fSpecialCoolTime  = 0.f;
	}

}

void UICameraManager::RenderMiniMap()
{
	const TEXINFO* pTexInfo = nullptr;

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"WhiteParticle");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 200.f, 300.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 17200.f , 3300.f , 0.f);
	matWorld = matScale * matTrans;

	Set_Ratio(matWorld, 0.04f, 0.03f);
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(100, 50, 50, 50));

	for (auto& pTile : GET_SINGLE(CMapManager)->GetTiles())
	{
		pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Terrain", L"Tile", pTile->iDrawID);
		if (nullptr == pTexInfo)
			return;

		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		// 크 자 이 공 부
		D3DXMatrixScaling(&matScale, pTile->vSize.x, pTile->vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, pTile->vPos.x + 16000.f, pTile->vPos.y + 1500.f, 0.f);
		matWorld = matScale * matTrans;

		Set_Ratio(matWorld, 0.04f, 0.03f);
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(150, 255, 255, 255));
	}

	// 새로 생성한 타일
	for (auto& pTile : GET_SINGLE(CMapManager)->GetWalls())
	{
		pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Terrain", L"Tileset", pTile->iDrawID);
		if (nullptr == pTexInfo)
			return;
		RECT rc = { pTile->iFrameX * 72, pTile->iFrameY * 72, pTile->iFrameX * 72 + 72, pTile->iFrameY * 72 + 72 };
		float fCenterX = float(rc.right - rc.left) * 0.5f;
		float fCenterY = float(rc.bottom - rc.top) * 0.5f;

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, pTile->vSize.x, pTile->vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, pTile->vPos.x + 16000.f, pTile->vPos.y + 1500.f, 0.f);
		matWorld = matScale * matTrans;

		Set_Ratio(matWorld, 0.04f, 0.03f);
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, &rc,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(150, 255, 255, 255));
	}

	DrawPlayer();
}

void UICameraManager::DrawPlayer(void)
{
	CObj* pPlayer = dynamic_cast<CPlayer*>(GET_SINGLE(CPlayerManager)->GetPlayer());

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Emoticon", L"Jimmy", 0);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans,16000.f + pPlayer->GetX(), 1500.f + pPlayer->GetY(), 0.f);
	matWorld = matScale * matTrans;

	Set_Ratio(matWorld, 0.04f, 0.03f);
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void UICameraManager::Set_Ratio(D3DXMATRIX& matWorld, const float& fRatioX, const float& fRatioY)
{
	matWorld._11 *= fRatioX;
	matWorld._21 *= fRatioX;
	matWorld._31 *= fRatioX;
	matWorld._41 *= fRatioX;

	matWorld._12 *= fRatioY;
	matWorld._22 *= fRatioY;
	matWorld._32 *= fRatioY;
	matWorld._42 *= fRatioY;
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
	case CSceneManager::SCENE_EVENT:
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

void UICameraManager::DrawFocusTitle(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"DashBar", L"DashBar", 0);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 10.f, 10.f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1) - 260.f - m_fDeltaY, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 0, 0, 0));


	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"DashBar", L"DashBar", 0);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 10.f, 10.f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1) + 260.f + m_fDeltaY, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 0, 0, 0));

}

void UICameraManager::DrawSpecialFocus(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"DashBar", L"DashBar", 0);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 10.f, 10.f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1) - 260.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 0, 0, 0));


	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"DashBar", L"DashBar", 0);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 10.f, 10.f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1) + 260.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 0, 0, 0));

}

void UICameraManager::DrawSpecialCard(void)
{
	int iDrawID = 0;
	PLAYER::ID ePlayerID = GET_SINGLE(CSceneManager)->GetPlayerID();
	switch (ePlayerID)
	{
	case PLAYER::JIMMY:
		iDrawID = 2;
		break;
	case PLAYER::PINKY:
		iDrawID = 3;
		break;
	case PLAYER::RAFF:
		iDrawID = 5;
		break;
	}

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"CharacterCard", iDrawID);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.2f, 1.2f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 240.f, float(WINCY >> 1) + 160.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void UICameraManager::DrawSpecialMidCard(void)
{
	wstring strName = L"";
	PLAYER::ID ePlayerID = GET_SINGLE(CSceneManager)->GetPlayerID();
	switch (ePlayerID)
	{
	case PLAYER::JIMMY:
		strName = L"JimmySel";
		break;
	case PLAYER::PINKY:
		strName = L"PinkySel";
		break;
	case PLAYER::RAFF:
		strName = L"RaffSel";
		break;
	}

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", strName, 0);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.2f, 1.2f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) + 500.f +  m_fDeltaX, float(WINCY >> 1), 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void UICameraManager::DrawSpecialText(void)
{
	TCHAR szBuf[MAX_PATH] = L"";
	wsprintf(szBuf, L"SPECIAL TIME");

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.6f, 1.6f, 1.6f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 200.f, float(WINCY >> 1) + 200.f, 0.f);

	matWorld = matScale * matTrans;
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
}
