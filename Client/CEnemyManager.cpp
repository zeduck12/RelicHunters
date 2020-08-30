#include "pch.h"
#include "CEnemyManager.h"
#include "CObjManager.h"
#include "CMonster.h"

bool CEnemyManager::LoadMonsterData()
{
	// 맵툴에서 작업한 Data파일에 있는 맵 정보 벡터에 넣어주기.
	TCHAR szFilePath[MAX_PATH] = L"..\\Data\\MonsterData.dat";
	HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	//shared_ptr<CObj> pMonster = nullptr;
	CObj* pMonster = nullptr;
	DWORD dwByte = 0;
	int	  iCount = 0;

	float fX = 0.f;
	float fY = 0.f;
	float fWidth = 0.f;
	float fHeight = 0.f;
	float fSpeed = 0.f;
	float fHp = 0.f;
	IMAGE::ID eID = IMAGE::END;

	ReadFile(hFile, &iCount, sizeof(int), &dwByte, nullptr);
	for (int i = 0; i < iCount; i++)
	{
		ReadFile(hFile, &fX, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fY, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fWidth, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fHeight, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fSpeed, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fHp, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &eID, sizeof(IMAGE::ID), &dwByte, nullptr);

		//pMonster = make_shared<CMonster>(fX, fY, fWidth, fHeight, fSpeed, fHp, eID);
		pMonster = new CMonster(fX, fY, fWidth, fHeight, fSpeed, fHp, eID);
		pMonster->Ready();
		GET_SINGLE(CObjManager)->GetMonsters().emplace_back(pMonster);
	}

    return true;
}
