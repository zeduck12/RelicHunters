#include "pch.h"
#include "CSelButton.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CStatBox.h"
#include "CMenu.h"

CSelButton::CSelButton(CMenu& _rMenu, float _fX, float _fY, float _fWidth, float _fHeight, const wstring& _strName, const wstring& _strClickedName, BUTTON::SEL_ID _eID)
	:
	m_rMenuScene{ _rMenu }
{
	m_pStatBox = nullptr;
	m_bIsActive = false;
	m_bIsClicked = false;
	// 초기화
	m_tInfo.vPos = { _fX , _fY, 0.f };
	m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
	m_tInfo.vImageSize = { 1.f, 1.f, 1.f };
	m_tInfo.vDir = { 1.f ,0.f , 0.f };
	m_tInfo.vLook = { 1.f ,0.f , 0.f };
	D3DXMatrixIdentity(&m_tInfo.mat);

	m_iDrawID = 0;
	m_iDrawClickedID = 0;

	m_strName = _strName;
	m_strClickedName = _strClickedName;
	m_eID = _eID;

}

CSelButton::~CSelButton()
{
	Release();
}

void CSelButton::Ready(void)
{
}

int CSelButton::Update(float _fDeltaTime)
{
	if (m_pStatBox)
		m_pStatBox->Update();

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	if (pt.x > m_tInfo.vPos.x - (m_tInfo.vSize.x * 0.5f) && m_tInfo.vPos.x + (m_tInfo.vSize.x * 0.5f) > pt.x &&
		pt.y > m_tInfo.vPos.y - (m_tInfo.vSize.y * 0.5f) && m_tInfo.vPos.y + (m_tInfo.vSize.y * 0.5f) > pt.y)
	{
		m_bIsActive = true;
		m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fStackTime >= 0.1f)
		{
			m_iDrawID++;
			m_iDrawClickedID++;
			m_fStackTime = 0.f;
		}

		if (m_iDrawID >= 11)
			m_iDrawID = 0;

		if (m_iDrawClickedID >= 4)
			m_iDrawClickedID = 4;

		// 캐릭터 선택
		if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_LBUTTON))
		{
			if (m_eID == BUTTON::BIU || m_eID == BUTTON::PUNNY || m_eID == BUTTON::ASS)
				return 0;

			if(m_eID == BUTTON::JIMMY)
				GET_SINGLE(CSceneManager)->SetPlayerID(PLAYER::JIMMY);
			else if(m_eID == BUTTON::PINKY)
				GET_SINGLE(CSceneManager)->SetPlayerID(PLAYER::PINKY);
			else if(m_eID == BUTTON::RAFF)
				GET_SINGLE(CSceneManager)->SetPlayerID(PLAYER::RAFF);
			else if (m_eID == BUTTON::RANDOM) // 랜덤으로 캐릭선택되게
			{
				int iRandNum = rand() % 3 + 1;
				if(iRandNum == 1)
					GET_SINGLE(CSceneManager)->SetPlayerID(PLAYER::JIMMY);
				else if(iRandNum == 2)
					GET_SINGLE(CSceneManager)->SetPlayerID(PLAYER::PINKY);
				else if(iRandNum == 3)
					GET_SINGLE(CSceneManager)->SetPlayerID(PLAYER::RAFF);
			}
			else
				GET_SINGLE(CSceneManager)->SetPlayerID(PLAYER::JIMMY);

			// 캐릭터 클릭
			m_bIsClicked = true;
			GET_SINGLE(CSoundManager)->PlaySound((TCHAR*)L"sfx_click.wav", CSoundManager::BUTTON);
			// 버튼들을 가져와서
			for (auto pBtn : m_rMenuScene.GetButtons())
			{
				// 자기 자신을 제외하고 클릭된 아이들이 있으면 해제하고 그 버튼 소유의 스탯박스 delete
				if (pBtn == this)
					continue;

				CSelButton* pSelBtn = dynamic_cast<CSelButton*>(pBtn);
				if (pSelBtn->IsClicked() == true)
				{
					pSelBtn->SetIsClicked(false);
					pSelBtn->Release();
				}
				
			}

			if(m_pStatBox == nullptr)
				m_pStatBox = new CStatBox(this);
			return 0;
		}
	}
	else
	{
		m_iDrawID = 0;
		m_fStackTime = 0.f;
		m_bIsActive = false;
	}

    return 0;
}

void CSelButton::LateUpdate(void)
{
}

void CSelButton::Release(void)
{
	Safe_Delete(m_pStatBox);
}

void CSelButton::Render(const HDC& _hdc)
{
	if (m_pStatBox)
		m_pStatBox->Render(_hdc);

	if (m_eID == BUTTON::BIU || m_eID == BUTTON::PUNNY || m_eID == BUTTON::ASS)
	{
		DrawIdle();
	}
	else
	{
		if (m_bIsClicked == true)
		{
			DrawClicked();
			DrawName();
			return;
		}

		if(m_bIsActive == false)
			DrawIdle();
		else
			DrawActive();
	}

	DrawName();
}

void CSelButton::DrawIdle(void)
{
	if (m_eID == BUTTON::JIMMY)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"JimmyCardIdle");
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	else if (m_eID == BUTTON::PINKY)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"PinkyCardIdle");
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	}

	else if (m_eID == BUTTON::RAFF)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"RaffCardIdle");
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	else if (m_eID == BUTTON::BIU)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"BiuCardDeath");
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
	else if (m_eID == BUTTON::PUNNY)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"PunnyCardDeath");
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}
	else if (m_eID == BUTTON::ASS)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"AssCardDeath");
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	else if (m_eID == BUTTON::RANDOM)
	{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"RadomCardIdle");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


}

void CSelButton::DrawActive(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", m_strName, m_iDrawID);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.85f, 0.85f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CSelButton::DrawClicked(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", m_strClickedName, m_iDrawClickedID);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CSelButton::DrawName(void)
{
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 3.f, m_tInfo.vPos.y + 5.f, 0.f);

	if (m_eID == BUTTON::JIMMY)
	{
		TCHAR szBuf[MAX_PATH] = L"JIMMY";
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	else if (m_eID == BUTTON::PINKY)
	{
		TCHAR szBuf[MAX_PATH] = L"PINKY";
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));

	}

	else if (m_eID == BUTTON::RAFF)
	{
		TCHAR szBuf[MAX_PATH] = L" RAFF";
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	else if (m_eID == BUTTON::BIU)
	{
		TCHAR szBuf[MAX_PATH] = L"  BIU";
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
	else if (m_eID == BUTTON::PUNNY)
	{
		TCHAR szBuf[MAX_PATH] = L"PUNNY";
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
	else if (m_eID == BUTTON::ASS)
	{
		TCHAR szBuf[MAX_PATH] = L"  ASS";
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));

	}

	
}
