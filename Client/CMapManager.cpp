#include "pch.h"
#include "CMapManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CStructure.h"
#include "CPlayerManager.h"
#include "CPlayer.h"
#include "CCollisionManager.h"
#include "CItem.h"
#include "CSceneManager.h"

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

void CMapManager::Update(void)
{
	for (auto& pItem : m_listItems) { DO_IF_IS_VALID_OBJ(pItem) { pItem->Update(); } }
}

void CMapManager::LateUpdate(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();

	for (auto& pTile : m_vecCreateTile)
		CCollisionManager::CollideCharacterTile(pPlayer, pTile);
	for (auto& pStruc : m_listStructure)
		CCollisionManager::CollideCharacterStructure(pPlayer, pStruc.get());

	for (auto& pStructure : m_listStructure) { DO_IF_IS_VALID_OBJ(pStructure) { pStructure->LateUpdate(); } }

	// 무효한 객체면 삭제
	CollectGarbageObjects(m_listStructure);
	CollectGarbageObjects(m_listItems);
}

void CMapManager::Render(const HDC& _hdc)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return;

	int iIndex = 0;
	const TEXINFO* pTexInfo = nullptr;
	for (auto& pTile : m_vecTile)
	{
		if (pTile->vPos.x < pPlayer->GetX() - ((WINCX >> 1) + 200.f) || pPlayer->GetX() + ((WINCX >> 1) + 200.f) < pTile->vPos.x ||
			pTile->vPos.y < pPlayer->GetY() - ((WINCY >> 1) + 200.f) || pPlayer->GetY() + ((WINCY >> 1) + 200.f) < pTile->vPos.y)
			continue;

		pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Terrain", L"Tile", pTile->iDrawID);
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
		if (pTile->vPos.x < pPlayer->GetX() - ((WINCX >> 1) + 200.f) || pPlayer->GetX() + ((WINCX >> 1) + 200.f) < pTile->vPos.x ||
			pTile->vPos.y < pPlayer->GetY() - ((WINCY >> 1) + 200.f) || pPlayer->GetY() + ((WINCY >> 1) + 200.f) < pTile->vPos.y)
			continue;

		pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Terrain", L"Tileset", pTile->iDrawID);
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
	for (auto& pObj : m_listStructure)
	{
		DO_IF_IS_VALID_OBJ(pObj)
		{
			if (pObj->GetInfo()->vPos.x < pPlayer->GetX() - ((WINCX >> 1) + 200.f) || pPlayer->GetX() + ((WINCX >> 1) + 200.f) < pObj->GetInfo()->vPos.x ||
				pObj->GetInfo()->vPos.y < pPlayer->GetY() - ((WINCY >> 1) + 200.f) || pPlayer->GetY() + ((WINCY >> 1) + 200.f) < pObj->GetInfo()->vPos.y)
				continue;

			dynamic_cast<CStructure*>(pObj.get())->Render();
			
		}
	}

	// 아이템
	for (auto& pItem : m_listItems) { DO_IF_IS_VALID_OBJ(pItem) { pItem->Render(_hdc); } }
}

void CMapManager::Release(void)
{
	DeleteListSafe(m_vecTile);
	DeleteListSafe(m_vecCreateTile);
}

// 나중에 파일 경로 받아서 스테이지마다 다른 맵 생성되게 해주기.
bool CMapManager::LoadFile(void)
{
	TCHAR szFilePath[MAX_PATH] = L"";
	CSceneManager::ID eSceneID = GET_SINGLE(CSceneManager)->GetNextSceneID();

	if (eSceneID == CSceneManager::SCENE_GAME)
		lstrcpy(szFilePath, L"..\\Data\\Stage1.dat");
	if (eSceneID == CSceneManager::SCENE_GAME2)
		lstrcpy(szFilePath, L"..\\Data\\Stage2.dat");
	if (eSceneID == CSceneManager::SCENE_GAME3)
		lstrcpy(szFilePath, L"..\\Data\\Stage3.dat");
	if (eSceneID == CSceneManager::SCENE_GAME4)
		lstrcpy(szFilePath, L"..\\Data\\Stage4.dat");

	// 맵툴에서 작업한 Data파일에 있는 맵 정보 벡터에 넣어주기.
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

		if (pTile->iDrawID == 3)
		{
			Safe_Delete(pTile);
			continue;
		}

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
		pTile->Ready();
		m_vecCreateTile.emplace_back(pTile);
	}

	shared_ptr<CStructure> pObj = nullptr;
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
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &bIsValid, sizeof(bool), &dwByte, nullptr);
		ReadFile(hFile, &fSpeed, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fDegree, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iCurHp, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iMaxHp, sizeof(int), &dwByte, nullptr);

	
		pObj = make_shared<CStructure>(tInfo.vPos, tInfo.vSize, tInfo.vImageSize, iDrawID);
		m_listStructure.emplace_back(pObj);
	}

	shared_ptr<CObj> pItem = nullptr;
	float fX = 0.f;
	float fY = 0.f;
	float fWidth = 0.f;
	float fHeight = 0.f;
	iDrawID   =  0;
	IMAGE::ID eID = IMAGE::END;

	ReadFile(hFile, &iTileCount, sizeof(int), &dwByte, nullptr);
	for (int i = 0; i < iTileCount; i++)
	{
		ReadFile(hFile, &fX, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fY, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fWidth, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fHeight, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &eID, sizeof(IMAGE::ID), &dwByte, nullptr);

		if (eID == IMAGE::PICKUP_LIGHT)
			pItem = make_shared<CPickUpLight>(fX, fY, fWidth, fHeight, eID);
		else if (eID == IMAGE::PICKUP_MEDIUM)
			pItem = make_shared<CPickUpMedium>(fX, fY, fWidth, fHeight, eID);
		else if (eID == IMAGE::PICKUP_HEAVY)
			pItem = make_shared<CPickUpHeavy>(fX, fY, fWidth, fHeight, eID);
		else if(eID == IMAGE::PICKUP_SHIELD)
			pItem = make_shared<CPickUpShield>(fX, fY, fWidth, fHeight, eID);
		else if (eID == IMAGE::PICKUP_HEALTH)
			pItem = make_shared<CPickUpHealth>(fX, fY, fWidth, fHeight, eID);

		pItem->Ready();
		m_listItems.emplace_back(pItem);

	}

	GUN::ID eGunID = GUN::END;
	ReadFile(hFile, &iTileCount, sizeof(int), &dwByte, nullptr);
	for (int i = 0; i < iTileCount; i++)
	{
		ReadFile(hFile, &fX, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fY, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fWidth, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fHeight, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &eGunID, sizeof(GUN::ID), &dwByte, nullptr);

		switch (eGunID)
		{
		case GUN::DEFAULT:
			pItem = make_shared<DefaultGun>(fX, fY, fWidth, fHeight, eGunID);
			break;
		case GUN::FLAME:
			pItem = make_shared<FlameGun>(fX, fY, fWidth, fHeight, eGunID);
			break;
		case GUN::HIGH_MAG:
			pItem = make_shared<HighMagGun>(fX, fY, fWidth, fHeight, eGunID);
			break;
		case GUN::ASSAULT:
			pItem = make_shared<AssaultGun>(fX, fY, fWidth, fHeight, eGunID);
			break;
		case GUN::KEYTAR:
			pItem = make_shared<KeytarGun>(fX, fY, fWidth, fHeight, eGunID);
			break;
		case GUN::MACHINEGUN:
			pItem = make_shared<MachineGun>(fX, fY, fWidth, fHeight, eGunID);
			break;
		case GUN::PISTOL_ASSUALT:
			pItem = make_shared<PistolAssualtGun>(fX, fY, fWidth, fHeight, eGunID);
			break;
		case GUN::PISTOL_HEAVY:
			pItem = make_shared<PistolHeavyGun>(fX, fY, fWidth, fHeight, eGunID);
			break;
		case GUN::PLASMA:
			pItem = make_shared<PlasmaGun>(fX, fY, fWidth, fHeight, eGunID);
			break;
		case GUN::SHOTGUN:
			pItem = make_shared<ShotGun>(fX, fY, fWidth, fHeight, eGunID);
			break;
		case GUN::SNIPER:
			pItem = make_shared<SniperGun>(fX, fY, fWidth, fHeight, eGunID);
			break;
		}

		pItem->Ready();
		m_listItems.emplace_back(pItem);

	}

	CloseHandle(hFile);

	return true;
}
