#include "pch.h"
#include "CMenu.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CSelButton.h"
#include "CDecoStar.h"

CMenu::CMenu()
{
}

CMenu::~CMenu()
{
	Release();
}

bool CMenu::Ready(void)
{
	CObj* pButton = new CSelButton(*this, 170.f, 120.f, 320.f, 72.f, L"JimmySel", L"JimmyClicked", BUTTON::JIMMY);
	pButton->Ready();
	m_listButtons.emplace_back(pButton);

	pButton = new CSelButton(*this, 400.f, 120.f, 320.f, 72.f, L"PinkySel", L"PinkyClicked",BUTTON::PINKY);
	pButton->Ready();
	m_listButtons.emplace_back(pButton);

	pButton = new CSelButton(*this, 630.f, 120.f, 320.f, 72.f, L"RaffSel", L"RaffClicked",BUTTON::RAFF);
	pButton->Ready();
	m_listButtons.emplace_back(pButton);

	pButton = new CSelButton(*this, 170.f, 180.f, 320.f, 72.f, L"Button", L"Default", BUTTON::BIU);
	pButton->Ready();
	m_listButtons.emplace_back(pButton);

	pButton = new CSelButton(*this, 400.f, 180.f, 320.f, 72.f, L"Button", L"Default", BUTTON::PUNNY);
	pButton->Ready();
	m_listButtons.emplace_back(pButton);

	pButton = new CSelButton(*this, 630.f, 180.f, 320.f, 72.f, L"Button", L"Default", BUTTON::ASS);
	pButton->Ready();
	m_listButtons.emplace_back(pButton);


	pButton = new CSelButton(*this, 400.f, 240.f, 320.f, 72.f, L"RandomSel", L"RandomClicked", BUTTON::RANDOM);
	pButton->Ready();
	m_listButtons.emplace_back(pButton);

	//40 40
	shared_ptr<CDecoStar> pStar = nullptr;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			pStar = make_shared<CDecoStar>(-1000.f + j * 80.f, -600.f + i * 80.f, 80.f, 80.f);
			pStar->Ready();
			m_listStars.emplace_back(pStar);
		}
	}

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			pStar = make_shared<CDecoStar>(-1000.f + j * 80.f + 40.f, -600.f + i * 80.f + 40.f, 80.f, 80.f, 40.f, 40.f);
			pStar->Ready();
			m_listStars.emplace_back(pStar);
		}
	}


	return true;
}

void CMenu::Update(void)
{
	m_fCheckTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fCheckTime >= 0.2f)
	{
		m_iOSDrawID++;
		m_fCheckTime = 0.f;
	}

	if (m_iOSDrawID >= 2)
		m_iOSDrawID = 0;

	for (auto& pButton : m_listButtons) { if (pButton) { pButton->Update(); } }
	for (auto& pStar : m_listStars) { DO_IF_IS_VALID_OBJ(pStar) { pStar->Update(); } }

	// Scene 체인지
	if (GET_SINGLE(CSceneManager)->IsChangeScene() == true)
	{
		GET_SINGLE(CSceneManager)->SetIsChangeScene(false);
		GET_SINGLE(CSceneManager)->ChangeScene(CSceneManager::SCENE_TEST);
	}
}

void CMenu::LateUpdate(void)
{
	CollectGarbageObjects(m_listStars);
	CollectGarbageObjects(m_listButtons);
}

void CMenu::Render(const HDC& _hdc)
{
	//GET_SINGLE(CGraphicDevice)->RenderBegin();

	DrawBackgorund();
	DrawSelBackground();

	for (auto& pStar : m_listStars) { DO_IF_IS_VALID_OBJ(pStar) { pStar->Render(_hdc); } }
	for (auto& pButton : m_listButtons) { if (pButton) { pButton->Render(_hdc); } }

	DrawSceneTitle();
	DrawOpenSource();

	//GET_SINGLE(CGraphicDevice)->GetSprite()->End();
	//GET_SINGLE(CGraphicDevice)->RenderEnd();
}

void CMenu::Release(void)
{
	// 메뉴씬 지워질때 버튼들 메모리 해제

	DeleteListSafe(m_listButtons);
}

void CMenu::DrawBackgorund(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"MenuBackground");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, float(WINCX), float(WINCY), 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CMenu::DrawSelBackground(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"StatsBox");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 2.5f, 0.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), 150.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CMenu::DrawSceneTitle(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Lobby", L"Button", 1);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.2f, 0.8f, 0.f);
	D3DXMatrixTranslation(&matTrans, 50.f, 50.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR szBuf[MAX_PATH] = L"CHARACTER SELECTION";
	D3DXMatrixTranslation(&matTrans, 15.f, 40.f, 0.f);
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CMenu::DrawOpenSource(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"OpenSource", m_iOSDrawID);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 95.f, 530.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}
