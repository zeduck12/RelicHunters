#include "pch.h"
#include "CMapManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"

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

}

void CMapManager::Render(D3DXMATRIX _rMat)
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
}

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

	CloseHandle(hFile);

	return true;
}
