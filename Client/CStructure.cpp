#include "pch.h"
#include "CStructure.h"
#include "CGraphicDevice.h"
#include "CTextureManager.h"
#include "CTexture.h"
#include "CMapManager.h"
#include "CItem.h"

CStructure::CStructure(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vSize, D3DXVECTOR3 _vImageSize, int _iDrawID)
	:
	m_iDrawID{ _iDrawID },
	m_strStateKey{ L"" }
{
	m_iMaxHp = 200;
	m_iCurHp = m_iMaxHp;
	m_iCurDrawID = 0;

	// √ ±‚»≠
	m_tInfo.vPos = _vPos;
	m_tInfo.vSize = _vSize;
	m_tInfo.vImageSize = _vImageSize;
	m_tInfo.vDir = { 1.f ,0.f , 0.f };
	m_tInfo.vLook = { 1.f ,0.f , 0.f };
	D3DXMatrixIdentity(&m_tInfo.mat);

	m_vRealVertex[0].x = m_tInfo.vPos.x - (m_tInfo.vSize.x * 0.5f);
	m_vRealVertex[0].y = m_tInfo.vPos.y - (m_tInfo.vSize.y * 0.5f);
	m_vRealVertex[1].x = m_tInfo.vPos.x + (m_tInfo.vSize.x * 0.5f);
	m_vRealVertex[1].y = m_tInfo.vPos.y - (m_tInfo.vSize.y * 0.5f);
	m_vRealVertex[2].x = m_tInfo.vPos.x + (m_tInfo.vSize.x * 0.5f);
	m_vRealVertex[2].y = m_tInfo.vPos.y + (m_tInfo.vSize.y * 0.5f);
	m_vRealVertex[3].x = m_tInfo.vPos.x - (m_tInfo.vSize.x * 0.5f);
	m_vRealVertex[3].y = m_tInfo.vPos.y + (m_tInfo.vSize.y * 0.5f);

	// Draw ID 
	// 0 - ≈Î
	// 1 - ¿‹µ1
	// 2 - ¿‹µ2
	// 3 - ¿‹µ3
	// 4 - ±‰≈Î

	if (m_iDrawID == 0)
	{
		m_strStateKey = L"Barrel";
		m_iMaxDrawID = 13;
	}
	else if (m_iDrawID == 1)
	{
		m_strStateKey = L"Root1";
		m_iMaxDrawID = 10;
	}
	else if (m_iDrawID == 2)
	{
		m_strStateKey = L"Root2";
		m_iMaxDrawID = 14;
	}
	else if (m_iDrawID == 3)
	{
		m_strStateKey = L"Root3";
		m_iMaxDrawID = 11;
	}
	else if (m_iDrawID == 4)
	{
		m_strStateKey = L"LongBarrel";
		m_iMaxDrawID = 13;
	}
}

CStructure::~CStructure()
{
    Release();
}

void CStructure::Ready(void)
{
	m_iMaxHp = 200;
    m_iCurHp  = m_iMaxHp;
}

int CStructure::Update(float _fDeltaTime)
{
    return 0;
}

void CStructure::LateUpdate(void)
{
	if (m_iCurHp <= 0 || m_iCurDrawID >= m_iMaxDrawID)
	{
		m_iCurHp = 0;
		m_iCurDrawID = m_iMaxDrawID;
		this->SetIsValid(false);
		for (auto& pItem : GET_SINGLE(CMapManager)->GetItems())
		{
			if (pItem->GetX() > m_tInfo.vPos.x - (m_tInfo.vSize.x * 0.5f) && m_tInfo.vPos.x + (m_tInfo.vSize.x * 0.5f) > pItem->GetX() &&
				pItem->GetY() > m_tInfo.vPos.y - (m_tInfo.vSize.y * 0.5f) && m_tInfo.vPos.y + (m_tInfo.vSize.y * 0.5f) > pItem->GetY())
			{
				dynamic_cast<CItem*>(pItem.get())->SetIsDrop(true);
			}
		}
	

		return;
	}

	if (m_iCurHp >= m_iMaxHp * 0.8f && m_iMaxHp * 0.85f >= m_iCurHp)
		m_iCurDrawID += 1;

}

void CStructure::Release(void)
{
}


void CStructure::Render()
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Structure", m_strStateKey, m_iCurDrawID);
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
}
