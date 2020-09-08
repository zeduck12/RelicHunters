#include "pch.h"
#include "CButton.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CKeyManager.h"
#include "CSceneManager.h"

CButton::CButton(float _fX, float _fY, float _fWidth, float _fHeight, const wstring& _strName, BUTTON::ID _eID)
{
	m_bIsPress = false;
	// 초기화
	m_tInfo.vPos = { _fX , _fY, 0.f };
	m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
	m_tInfo.vImageSize = { 1.f, 1.f, 1.f };
	m_tInfo.vDir = { 1.f ,0.f , 0.f };
	m_tInfo.vLook = { 1.f ,0.f , 0.f };
	D3DXMatrixIdentity(&m_tInfo.mat);

	m_iDrawID = 0;
	m_strName = _strName;
	m_eID = _eID;
}

CButton::~CButton()
{
    Release();
}

void CButton::Ready(void)
{
}

int CButton::Update(float _fDeltaTime)
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	if (pt.x > m_tInfo.vPos.x - (m_tInfo.vSize.x * 0.5f) && m_tInfo.vPos.x + (m_tInfo.vSize.x * 0.5f) > pt.x &&
		pt.y > m_tInfo.vPos.y - (m_tInfo.vSize.y * 0.5f) && m_tInfo.vPos.y + (m_tInfo.vSize.y * 0.5f) > pt.y)
	{
		if (m_bIsPress == false)
			m_tInfo.vPos.x += 50;

		m_iDrawID = 0;
		m_bIsPress = true;

		// 시작버튼
		if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_LBUTTON))
		{
			GET_SINGLE(CSoundManager)->PlaySound((TCHAR*)L"sfx_click.wav", CSoundManager::BUTTON);
			if(m_eID == BUTTON::START)
				GET_SINGLE(CSceneManager)->SetIsChangeScene(true);
			else if(m_eID == BUTTON::CLOSE)
				DestroyWindow(g_hWND);

			return 0;
		}

	}
	else
	{
		if (m_bIsPress == true)
			m_tInfo.vPos.x -= 50;

		m_bIsPress = false;
		m_iDrawID = 1;
	}

    return 0;
}

void CButton::LateUpdate(void)
{
}

void CButton::Release(void)
{
}

void CButton::Render(const HDC& _hdc)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Lobby", m_strName, m_iDrawID);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_bIsPress == true)
	{
		pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Deco");
		if (nullptr == pTexInfo)
			return;
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 220.f, m_tInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}

	if (m_eID == BUTTON::START)
	{
		TCHAR szBuf[MAX_PATH] = L"PLAY ADVENTURE";
		if (m_bIsPress)
		{
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y - 10.f, 0.f);
			CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 0, 0));
		}
		else
		{
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y - 10.f, 0.f);
			CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	if (m_eID == BUTTON::SETTING)
	{
		TCHAR szBuf[MAX_PATH] = L"SETTING";
		if (m_bIsPress)
		{
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 60.f, m_tInfo.vPos.y - 10.f, 0.f);
			CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 0, 0));
		}
		else
		{
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 60.f, m_tInfo.vPos.y - 10.f, 0.f);
			CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	if (m_eID == BUTTON::CREDIT)
	{
		TCHAR szBuf[MAX_PATH] = L"CREDIT";
		if (m_bIsPress)
		{
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 80.f, m_tInfo.vPos.y - 10.f, 0.f);
			CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 0, 0));
		}
		else
		{
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 80.f, m_tInfo.vPos.y - 10.f, 0.f);
			CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	if (m_eID == BUTTON::CLOSE)
	{
		TCHAR szBuf[MAX_PATH] = L"EXIT";
		if (m_bIsPress)
		{
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 120.f, m_tInfo.vPos.y - 10.f, 0.f);
			CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 0, 0));
		}
		else
		{
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 120.f, m_tInfo.vPos.y - 10.f, 0.f);
			CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

}
