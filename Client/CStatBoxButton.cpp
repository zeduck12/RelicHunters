#include "pch.h"
#include "CStatBoxButton.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CStatBox.h"
#include "CSelButton.h"

CStatBoxButton::CStatBoxButton(CStatBox* _pOwner, float _fX, float _fY, BUTTON::ID _eID)
{
	m_eID = _eID;
	m_iDrawID = 0;
	// 초기화
	m_tInfo.vPos = { _fX , _fY, 0.f };
	m_tInfo.vSize = { 229.f, 43.f, 0.f };
	m_tInfo.vImageSize = { 1.f, 1.f, 1.f };
	m_tInfo.vDir = { 1.f ,0.f , 0.f };
	m_tInfo.vLook = { 1.f ,0.f , 0.f };
	D3DXMatrixIdentity(&m_tInfo.mat);

	m_pOwner = _pOwner;
}

void CStatBoxButton::Ready(void)
{
}

int CStatBoxButton::Update(float _fDeltaTime)
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	if (pt.x > m_tInfo.vPos.x - (m_tInfo.vSize.x * 0.5f) && m_tInfo.vPos.x + (m_tInfo.vSize.x * 0.5f) > pt.x &&
		pt.y > m_tInfo.vPos.y - (m_tInfo.vSize.y * 0.5f) && m_tInfo.vPos.y + (m_tInfo.vSize.y * 0.5f) > pt.y)
	{
		m_iDrawID = 1;
		// 시작버튼
		if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_LBUTTON))
		{
			GET_SINGLE(CSoundManager)->PlaySound((TCHAR*)L"sfx_click.wav", CSoundManager::BUTTON);
			if (m_eID == BUTTON::START)
				GET_SINGLE(CSceneManager)->SetIsChangeScene(true);
			else if (m_eID == BUTTON::CHANGE)
			{
				// 이 캐릭터 선택취소
				// 셀버튼 클릭상태 취소
				// 셀버튼 소유의 스탯박스 없애기.
				m_pOwner->GetOwner()->SetIsClicked(false);
				m_pOwner->GetOwner()->Release();
			}

			return 0;
		}

	}
	else
		m_iDrawID = 0;

    return 0;
}

void CStatBoxButton::LateUpdate(void)
{
}

void CStatBoxButton::Release(void)
{
}

void CStatBoxButton::Render(const HDC& _hdc)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Menu", L"Button", m_iDrawID);
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

	if (m_eID == BUTTON::START)
	{
		TCHAR szBuf[MAX_PATH] = L"START";
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 50.f, m_tInfo.vPos.y - 10.f, 0.f);
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 80, 188, 255));

	}
	else
	{
		TCHAR szBuf[MAX_PATH] = L"CHANGE";
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 50.f, m_tInfo.vPos.y - 10.f, 0.f);
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 80, 188, 255));
	}



}
