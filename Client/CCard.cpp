#include "pch.h"
#include "CCard.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "UICameraManager.h"
#include "CCameraManager.h"
#include "CKeyManager.h"
#include "CPlayerManager.h"
#include "CTimeManager.h"

CCard::CCard(float _fX, float _fY, CARD::ID _eID)
{
	m_tInfo.vPos = { _fX , _fY, 0.f };
	m_tInfo.vSize = { 117.f, 106.f, 0.f };
	m_tInfo.vImageSize = { 1.f, 1.f, 1.f };
	m_tInfo.vDir = { 1.f ,0.f , 0.f };
	m_tInfo.vLook = { 1.f ,0.f , 0.f };
	D3DXMatrixIdentity(&m_tInfo.mat);

	// JIMMY, PINKY, RAFF, BIU, ASS, PUNNY
	m_eID = _eID;
}

CCard::~CCard()
{
}

void CCard::Ready(void)
{
}

int CCard::Update(float _fDeltaTime)
{
	if (m_bIsStart == false)
	{
		m_bIsClicked = true;
		m_fStartTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fStartTime >= 2.f)
		{
			m_bIsStart = true;
			m_bIsClicked = false;
		}
	}
	else
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWND, &pt);

		if (pt.x > m_tInfo.vPos.x - (m_tInfo.vSize.x * 0.5f) && m_tInfo.vPos.x + (m_tInfo.vSize.x * 0.5f) > pt.x &&
			pt.y > m_tInfo.vPos.y - (m_tInfo.vSize.y * 0.5f) && m_tInfo.vPos.y + (m_tInfo.vSize.y * 0.5f) > pt.y)
		{
			if (GET_SINGLE(CKeyManager)->Key_UP(KEY_LBUTTON))
			{
				GET_SINGLE(CSoundManager)->StopSound(CSoundManager::EFFECT);
				GET_SINGLE(CSoundManager)->PlaySound((TCHAR*)L"sfx_menu_mouse.wav", CSoundManager::EFFECT);
				if (m_bIsClicked == false)
				{
					m_bIsClicked = true;
				}
			}
		}
	}

	

	return 0;
}

void CCard::LateUpdate(void)
{
	if (m_bIsStart == true)
	{

		if (m_bIsClicked == true && this->IsActive() == false)
		{
			m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
			if (m_fStackTime >= 0.4f)
			{
				m_fStackTime = 0.f;
				for (auto& pCard : GET_SINGLE(UICameraManager)->GetCards())
				{
					if (pCard.get() == this || pCard->IsActive() == true)
						continue;

					if (pCard->IsClicked() == true && pCard->IsActive() == false)
					{
						if (pCard->GetCardID() != this->GetCardID())
						{
							pCard->SetIsClicked(false);
							this->SetIsClicked(false);
							GET_SINGLE(CSoundManager)->StopSound(CSoundManager::EFFECT);
							GET_SINGLE(CSoundManager)->PlaySound((TCHAR*)L"sfx_duck_death.wav", CSoundManager::EFFECT);
						}
						else if (pCard->GetCardID() == this->GetCardID())
						{
							pCard->SetIsClicked(true);
							this->SetIsClicked(true);

							pCard->SetIsActive(true);
							this->SetIsActive(true);
						}
					}
				}

			}
		}
	}
}

void CCard::Release(void)
{
}

void CCard::Render(const HDC& _hdc)
{
}

void CCard::Render_Card(void)
{
	switch (m_eID)
	{
	case CARD::JIMMY:
		m_iDrawID = 0;
		break;
	case CARD::PINKY:
		m_iDrawID = 1;
		break;
	case CARD::RAFF:
		m_iDrawID = 2;
		break;
	case CARD::BIU:
		m_iDrawID = 3;
		break;
	case CARD::ASS:
		m_iDrawID = 4;
		break;
	case CARD::PUNNY:
		m_iDrawID = 5;
		break;
	}
	const TEXINFO* pTexInfo = nullptr;
	if (m_bIsClicked == true)
	{
		pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Card", L"Card", m_iDrawID);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, this->GetX(), this->GetY(), 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Card", L"Card", 6);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, this->GetX(), this->GetY(), 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


}
