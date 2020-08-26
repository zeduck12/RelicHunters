#include "pch.h"
#include "CMapManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CStructure.h"
#include "CPlayerManager.h"
#include "CPlayer.h"
#include "CCollisionManager.h"

DEFINITION_SINGLETON(CMapManager)

CMapManager::~CMapManager()
{
	Release();
}

bool CMapManager::Ready(void)
{
	// 파일 불러오기 실패할 경우 FALSE
	if (!LoadFile())
		return false;

	return true;
}

void CMapManager::LateUpdate(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();

	for (auto& pTile : m_vecCreateTile)
		CCollisionManager::CollideCharacterTile(pPlayer, pTile);

}

void CMapManager::Render(void)
{
	TCHAR szBuf[MAX_PATH] = L"";
	int iIndex = 0;
	for (auto& pTile : m_vecTile)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Terrain", L"Tile", pTile->iDrawID);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		// 크 자 이 공 부
		D3DXMatrixScaling(&matScale, pTile->vSize.x, pTile->vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, pTile->vPos.x, pTile->vPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	}

	// 새로 생성한 타일
	for (auto& pTile : m_vecCreateTile)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Terrain", L"Tileset", pTile->iDrawID);
		if (nullptr == pTexInfo)
			return;
		RECT rc = { pTile->iFrameX * 72, pTile->iFrameY * 72, pTile->iFrameX * 72 + 72, pTile->iFrameY * 72 + 72 };
		float fCenterX = float(rc.right - rc.left) * 0.5f;
		float fCenterY = float(rc.bottom - rc.top) * 0.5f;

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, pTile->vSize.x, pTile->vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, pTile->vPos.x, pTile->vPos.y, 0.f);
		matWorld = matScale * matTrans;

		// 이미지 출력영역
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, &rc,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);

	}

	// 맵 조형물 
	for (auto& pObj : m_vecStructure)
		pObj->Render();

}

void CMapManager::Render(D3DXMATRIX _rMat)
{
	TCHAR szBuf[MAX_PATH] = L"";
	for (auto& pTile : m_vecTile)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Terrain", L"Tile", pTile->iDrawID);
		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		// 크 자 이 공 부
		D3DXMatrixScaling(&matScale, pTile->vSize.x, pTile->vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, pTile->vPos.x, pTile->vPos.y, 0.f);
		matWorld = matScale * matTrans;

		matWorld *= _rMat;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	}


	// 새로 생성한 타일
	for (auto& pTile : m_vecCreateTile)
	{
		const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Terrain", L"Tileset", pTile->iDrawID);
		if (nullptr == pTexInfo)
			return;
		RECT rc = { pTile->iFrameX * 72, pTile->iFrameY * 72, pTile->iFrameX * 72 + 72, pTile->iFrameY * 72 + 72 };
		float fCenterX = float(rc.right - rc.left) * 0.5f;
		float fCenterY = float(rc.bottom - rc.top) * 0.5f;

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, pTile->vSize.x, pTile->vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, pTile->vPos.x, pTile->vPos.y, 0.f);
		matWorld = matScale * matTrans;

		matWorld *= _rMat;

		// 이미지 출력영역
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, &rc,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	}

}

void CMapManager::Release(void)
{
	for (auto& pTile : m_vecTile)
		Safe_Delete(pTile);

	m_vecTile.clear();
	m_vecTile.shrink_to_fit();


	for (auto& pTile : m_vecCreateTile)
		Safe_Delete(pTile);

	m_vecCreateTile.clear();
	m_vecCreateTile.shrink_to_fit();

	for (auto& pObj : m_vecStructure)
		Safe_Delete(pObj);

	m_vecStructure.clear();
	m_vecStructure.shrink_to_fit();

}

// 나중에 파일 경로 받아서 스테이지마다 다른 맵 생성되게 해주기.
bool CMapManager::LoadFile(void)
{
	// 맵툴에서 작업한 Data파일에 있는 맵 정보 벡터에 넣어주기.
	TCHAR szFilePath[MAX_PATH] = L"..\\Data\\Test.dat";
	HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	TILE* pTile = nullptr;
	DWORD dwByte = 0;
	int	  iTileCount = 0;

	ReadFile(hFile, &iTileCount, sizeof(int), &dwByte, nullptr);
	for (int i = 0; i < iTileCount; i++)
	{
		pTile = new TILE;
		ReadFile(hFile, &pTile->vPos, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &pTile->vSize, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &pTile->iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pTile->iOption, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pTile->iFrameX, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pTile->iFrameY, sizeof(int), &dwByte, nullptr);
		m_vecTile.emplace_back(pTile);
	}

	ReadFile(hFile, &iTileCount, sizeof(int), &dwByte, nullptr);
	for (int i = 0; i < iTileCount; i++)
	{
		pTile = new TILE;
		ReadFile(hFile, &pTile->vPos, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &pTile->vSize, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &pTile->iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pTile->iOption, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pTile->iFrameX, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pTile->iFrameY, sizeof(int), &dwByte, nullptr);
		m_vecCreateTile.emplace_back(pTile);
	}

	CStructure* pObj = nullptr;
	INFO tInfo = {};
	ZeroMemory(&tInfo, sizeof(INFO));
	bool  bIsValid = false;
	float fSpeed   = 0.f;
	float fDegree  = 0.f;
	int   iDrawID  = 0;
	int	  iCurHp   = 0;
	int	  iMaxHp = 0;

	ReadFile(hFile, &iTileCount, sizeof(int), &dwByte, nullptr);
	for (int i = 0; i < iTileCount; i++)
	{
		pObj = new CStructure;
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &bIsValid, sizeof(bool), &dwByte, nullptr);
		ReadFile(hFile, &fSpeed, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fDegree, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iCurHp, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iMaxHp, sizeof(int), &dwByte, nullptr);

		pObj->SetInfo(tInfo);
		pObj->SetIsValid(bIsValid);
		pObj->SetSpeed(fSpeed);
		pObj->SetDegree(fDegree);
		pObj->SetDrawID(iDrawID);
		pObj->SetCurHp(iCurHp);
		pObj->SetMaxHp(iMaxHp);

		m_vecStructure.emplace_back(pObj);
	}

	CloseHandle(hFile);

	return true;
}
