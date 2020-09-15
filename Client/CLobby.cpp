#include "pch.h"
#include "CLobby.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CButton.h"
#include "CDecoStar.h"

CLobby::CLobby()
{
	m_iLogoBoxID = 0;
	m_iZueiraID  = 0;
	m_fStackTime = 0;
}

CLobby::~CLobby()
{
}

bool CLobby::Ready(void)
{
	//배경음악
	CSoundManager::Get_Instance()->StopAll();
	CSoundManager::Get_Instance()->PlayBGM((TCHAR*)L"bgm_menu.wav");

	// 나중에 버튼 옵션줘서 기능 추가.
	shared_ptr<CButton> pButton = make_shared<CButton>(50.f, 300.f, 600.f, 52.f, L"Button", BUTTON::START);
	pButton->Ready();
	m_listButtons.emplace_back(pButton);

	pButton = make_shared<CButton>(0.f, 370.f, 600.f, 52.f, L"Button", BUTTON::MINI_GAME);
	pButton->Ready();
	m_listButtons.emplace_back(pButton);

	pButton = make_shared<CButton>(-50.f, 440.f, 600.f, 52.f, L"Button", BUTTON::CREDIT);
	pButton->Ready();
	m_listButtons.emplace_back(pButton);

	pButton = make_shared<CButton>(-100.f, 510.f, 600.f, 52.f, L"Button", BUTTON::CLOSE);
	pButton->Ready();
	m_listButtons.emplace_back(pButton);

	//40 40
	shared_ptr<CDecoStar> pStar = nullptr;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			pStar = make_shared<CDecoStar>(-1000.f + j * 80.f, -600.f +i * 80.f, 80.f, 80.f);
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

void CLobby::Update(void)
{
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 0.1f)
	{
		m_iLogoBoxID++;
		m_iZueiraID++;
		m_fStackTime = 0.f;
	}

	if (m_iZueiraID >= 23)
		m_iZueiraID = 0;

	if (m_iLogoBoxID >= 41)
		m_iLogoBoxID = 41;

	for (auto& pButton : m_listButtons) { pButton->Update(); }
	for (auto& pStar : m_listStars) { DO_IF_IS_VALID_OBJ(pStar) { pStar->Update(); } }

	// Scene 체인지
	if (GET_SINGLE(CSceneManager)->IsChangeScene() == true)
	{
		GET_SINGLE(CSceneManager)->SetIsChangeScene(false);
		if (GET_SINGLE(CSceneManager)->IsChangeMiniGame() == true)
		{
			GET_SINGLE(CSceneManager)->SetIsChangeMiniGame(false);
			GET_SINGLE(CSceneManager)->ChangeScene(CSceneManager::SCENE_EVENT);
		}
		else
			GET_SINGLE(CSceneManager)->ChangeScene(CSceneManager::SCENE_MENU);
	}
}

void CLobby::LateUpdate(void)
{
	CollectGarbageObjects(m_listStars);
}

void CLobby::Render(const HDC& _hdc)
{
	DrawMainBackGround();
	DrawMainBackGround2();
	for (auto& pStar : m_listStars) { DO_IF_IS_VALID_OBJ(pStar) { pStar->Render(_hdc); } }
	for (auto& pButton : m_listButtons) { pButton->Render(_hdc); }

	DrawLogoBox();
	DrawLogo();
	DrawZueira();
}

void CLobby::Release(void)
{
	m_listButtons.clear();
}

void CLobby::DrawMainBackGround(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"MainBackground");
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

}

void CLobby::DrawMainBackGround2(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"MainBackground2");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, float(1280 / WINCX), float(720 / WINCY), 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(50, 255, 255, 255));

}

void CLobby::DrawLogoBox(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Lobby", L"LogoBox", m_iLogoBoxID);
	if (nullptr == pTexInfo)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	// 545 243
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, 200.f, 200.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CLobby::DrawLogo(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"LogoText");
	if (nullptr == pTexInfo)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	// 545 243
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.35f, 0.35f, 0.f);
	D3DXMatrixTranslation(&matTrans, 500.f, 550.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CLobby::DrawZueira(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Lobby", L"Zueira", m_iZueiraID);
	if (nullptr == pTexInfo)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	// 545 243
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 700.f, 550.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}
