#include "pch.h"
#include "CPlayerInfo.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CPlayerManager.h"
#include "CPlayer.h"
#include "CCameraManager.h"
#include "CTimeManager.h"

CPlayerInfo::CPlayerInfo(const wstring& _strName)
{
	m_strCharacterName = _strName;
}

bool CPlayerInfo::Ready(void)
{
	return true;
}

void CPlayerInfo::Update(void)
{
}

void CPlayerInfo::Render(void)
{
	DrawCharacterEmoticon();	 // 플레이어 이모티콘
	DrawCharacterHpBar();		 // 플레이어 HP
	DrawCharacterShieldBar();	 // 플레이어 Sheild
	DrawIcons();				 // Hp, Shield 아이콘들
	DrawCoinBar();				 // Coin 바
	DrawDashBar();				 // Dash 바

	DrawSubWeapon();
	DrawWeapon();
	DrawBulletCount();
	DrawBombsCount();

	DrawSpecialCount();
}

void CPlayerInfo::DrawCharacterEmoticon(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(GET_SINGLE(CPlayerManager)->GetPlayer());
	int iDrawID = 0;
	if(pPlayer->GetCurHp() <= 100.f)
		iDrawID = 1;
	else
		iDrawID = 0;

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Emoticon", m_strCharacterName, iDrawID);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 50.f , 50.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPlayerInfo::DrawCharacterShieldBar(void)
{
	float fCurShieldHp = 0.f;
	float fDelta = 0.f;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(GET_SINGLE(CPlayerManager)->GetPlayer());
	if (GET_SINGLE(CPlayerManager)->GetPlayer() != nullptr)
	{
		fCurShieldHp = pPlayer->GetShield()->GetCurShieldHp();
	}

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"HpBar");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.2f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 170.f, 50.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(100, 0, 0, 0));
	////////////
	if (pPlayer->IsAttacked() == true)
	{ 
		if(m_bIsTracking == false)
			m_fSaveShieldHp = pPlayer->GetSaveShieldHp();
		m_bIsTracking = true;
	}

	if (m_bIsTracking)
	{
		m_fSaveShieldHp -= 0.3f;
		if (m_fSaveShieldHp <= fCurShieldHp)
		{
			m_bIsTracking = false;
			m_fSaveShieldHp = fCurShieldHp;
		}
	}

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"White");
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.2f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 170.f, 50.f, 0.f);
	matWorld = matScale * matTrans;

	RECT rc = { 0, 0, LONG(114.f * (m_fSaveShieldHp / 100.f)), 30 };

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, &rc, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(200, 255, 255,255));
	///////////////
	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"HpBar");
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.2f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 170.f, 50.f, 0.f);
	matWorld = matScale * matTrans;

	rc = { 0, 0, LONG(114.f * (fCurShieldHp / 100.f)), 30 };

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, &rc, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	TCHAR szBuf[MAX_PATH] = L"";
	wsprintf(szBuf, L"%d", int(fCurShieldHp));

	D3DXMatrixTranslation(&matTrans, 180.f, 40.f, 0.f);
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));


}

void CPlayerInfo::DrawCharacterHpBar(void)
{
	float fCurHp   = 0.f;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(GET_SINGLE(CPlayerManager)->GetPlayer());
	if (GET_SINGLE(CPlayerManager)->GetPlayer() != nullptr)
	{
		fCurHp = pPlayer->GetCurHp();
	}

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"DashBar", L"DashBar", 0);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 2.f, 2.3f, 0.f);
	D3DXMatrixTranslation(&matTrans, 183.f, 30.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(100, 0, 0, 0));
/////////////
	if (pPlayer->IsAttacked() == true && pPlayer->GetShield()->GetCurShieldHp() <= 0.f)
	{
		if (m_bIsTrackingHp == false)
			m_fSaveHp = pPlayer->GetSaveHp();
		m_bIsTrackingHp = true;
	}

	if (m_bIsTrackingHp)
	{
		m_fSaveHp -= 0.4f;
		if (m_fSaveHp <= fCurHp)
		{
			m_bIsTrackingHp = false;
			m_fSaveHp = fCurHp;
		}
	}

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"DashBar", L"DashBar", 0);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 2.f, 2.3f, 0.f);
	D3DXMatrixTranslation(&matTrans, 183.f, 30.f, 0.f);
	matWorld = matScale * matTrans;

	RECT rc = { 0, 0, LONG(81 * (m_fSaveHp / 200.f)), 18 };

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, &rc, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(200, 255, 255, 255));
/////////////

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"DashBar", L"DashBar", 0);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 2.f, 2.3f, 0.f);
	D3DXMatrixTranslation(&matTrans, 183.f, 30.f, 0.f);
	matWorld = matScale * matTrans;

	rc = { 0, 0, LONG(81 * (fCurHp / 200.f)), 18};

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, &rc, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 0, 70));
	
	TCHAR szBuf[MAX_PATH] = L"";
	wsprintf(szBuf, L"%d", int(fCurHp));

	D3DXMatrixTranslation(&matTrans, 210.f , 10.f, 0.f);
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CPlayerInfo::DrawCoinBar(void)
{
	int iCoins = 0;
	iCoins = GET_SINGLE(CPlayerManager)->GetInventory()->GetCoins();
	
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"CoinBar");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 30.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR szBuf[MAX_PATH] = L"";
	wsprintf(szBuf, L"%d", int(iCoins));

	D3DXMatrixTranslation(&matTrans, 400.f, 20.f, 0.f);
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CPlayerInfo::DrawIcons(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Icon", L"Icon", 1);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
	D3DXMatrixTranslation(&matTrans, 120.f, 20.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Icon", L"Icon", 0);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
	D3DXMatrixTranslation(&matTrans, 120.f, 50.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

///
void CPlayerInfo::DrawWeapon(void)
{
	GUN::ID eCurWeaponID = GUN::END;
	eCurWeaponID = GET_SINGLE(CPlayerManager)->GetInventory()->GetCurWeapon();
	
	wstring strName = L"";
	switch (eCurWeaponID)
	{
	case GUN::DEFAULT:
		strName = L"Pistol";
		break;
	case GUN::FLAME:
		strName = L"Flame";
		break;
	case GUN::HIGH_MAG:
		strName = L"HighMag";
		break;
	case GUN::ASSAULT:
		strName = L"Assault";
		break;
	case GUN::KEYTAR:
		strName = L"Keytar";
		break;
	case GUN::MACHINEGUN:
		strName = L"MachineGun";
		break;
	case GUN::PISTOL_ASSUALT:
		strName = L"Pistol_Assualt";
		break;
	case GUN::PISTOL_HEAVY:
		strName = L"Pistol_Heavy";
		break;
	case GUN::PLASMA:
		strName = L"Plasma";
		break;
	case GUN::SHOTGUN:
		strName = L"Shotgun";
		break;
	case GUN::SNIPER:
		strName = L"Sniper";
		break;
	case GUN::BOSS:
		strName = L"Boss";
		break;
	}

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", strName, 2);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (eCurWeaponID == GUN::BOSS)
		D3DXMatrixScaling(&matScale, 1.2f, 1.2f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 80.f, 550.f, 0.f);
	//D3DXMatrixTranslation(&matTrans, 80.f, 190.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CPlayerInfo::DrawSubWeapon(void)
{
	GUN::ID eSubWeaponID = GUN::END;
	GET_SINGLE(CPlayerManager)->GetInventory()->GetSubWeapon(&eSubWeaponID);

	wstring strName = L"";
	switch (eSubWeaponID)
	{
	case GUN::DEFAULT:
		strName = L"Pistol";
		break;
	case GUN::FLAME:
		strName = L"Flame";
		break;
	case GUN::HIGH_MAG:
		strName = L"HighMag";
		break;
	case GUN::ASSAULT:
		strName = L"Assault";
		break;
	case GUN::KEYTAR:
		strName = L"Keytar";
		break;
	case GUN::MACHINEGUN:
		strName = L"MachineGun";
		break;
	case GUN::PISTOL_ASSUALT:
		strName = L"Pistol_Assualt";
		break;
	case GUN::PISTOL_HEAVY:
		strName = L"Pistol_Heavy";
		break;
	case GUN::PLASMA:
		strName = L"Plasma";
		break;
	case GUN::SHOTGUN:
		strName = L"Shotgun";
		break;
	case GUN::SNIPER:
		strName = L"Sniper";
		break;
	case GUN::BOSS:
		strName = L"Boss";
		break;
	}

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", strName, 2);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	if(eSubWeaponID == GUN::BOSS)
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.7f, 1.7f, 0.f);
	D3DXMatrixTranslation(&matTrans, 120.f, 520.f, 0.f);
	//D3DXMatrixTranslation(&matTrans, 120.f, 160.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(100, 255, 255, 255));

}

void CPlayerInfo::DrawBulletCount(void)
{
	int iCapacity = 0;
	int iReloaded = 0;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(GET_SINGLE(CPlayerManager)->GetPlayer());
	if (GET_SINGLE(CPlayerManager)->GetPlayer() != nullptr)
	{
		iCapacity = GET_SINGLE(CPlayerManager)->GetInventory()->GetOwnBullets();
		iReloaded = pPlayer->GetCurWeapon()->GetReloadedBullets();
	}

	D3DXMATRIX matScale, matTrans, matWorld;
	TCHAR szBuf[MAX_PATH] = L"";
	wsprintf(szBuf, L"%d", int(iCapacity));
	D3DXMatrixScaling(&matScale, 1.2f, 1.2f, 1.f);
	D3DXMatrixTranslation(&matTrans, 240.f, 530.f , 0.f);
	//D3DXMatrixTranslation(&matTrans, 240.f, 140.f, 0.f);
	matWorld = matScale * matTrans;
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));


	wsprintf(szBuf, L"%d /", int(iReloaded));
	D3DXMatrixScaling(&matScale, 0.9f, 0.9f, 1.f);
	D3DXMatrixTranslation(&matTrans, 180.f , 515.f , 0.f);
	//D3DXMatrixTranslation(&matTrans, 180.f, 125.f, 0.f);
	matWorld = matScale * matTrans;
	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetFont()->DrawTextW(CGraphicDevice::Get_Instance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CPlayerInfo::DrawBombsCount(void)
{
	int iBombsCount = 0;
	iBombsCount = GET_SINGLE(CPlayerManager)->GetInventory()->GetBombsCount();

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Grenade");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	for (int i = 0; i < iBombsCount; i++)
	{
		D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
		D3DXMatrixTranslation(&matTrans, 50.f + (i * 20.f), 485.f, 0.f);
		//D3DXMatrixTranslation(&matTrans, 50.f + (i * 20.f), 125.f, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

///
void CPlayerInfo::DrawDashBar(void)
{
	float fCurDashHp = 0.f;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(GET_SINGLE(CPlayerManager)->GetPlayer());
	if (GET_SINGLE(CPlayerManager)->GetPlayer() != nullptr)
	{
		fCurDashHp = pPlayer->GetCurDashHp();
	}

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"DashBar", L"DashBar", 0);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 2.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 183.f, 80.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(100, 0, 0, 0));


	pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"DashBar", L"DashBar", 0);
	if (nullptr == pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 2.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 183.f, 80.f, 0.f);
	matWorld = matScale * matTrans;

	RECT rc = { 0, 0, LONG(81 * (fCurDashHp / 200.f)), 18 };

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, &rc, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPlayerInfo::DrawSpecialCount(void)
{
	//SpecialIcon
	int iSpecialCount = 0;
	iSpecialCount = dynamic_cast<CPlayer*>(GET_SINGLE(CPlayerManager)->GetPlayer())->GetSpecialCount();

	int iDrawID = 0;
	if (m_strCharacterName == L"Jimmy")
		iDrawID = 0;
	else if (m_strCharacterName == L"Pinky")
		iDrawID = 1;
	else if (m_strCharacterName == L"Raff")
		iDrawID = 2;

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Special", L"SpecialIcon", iDrawID);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	for (int i = 0; i < iSpecialCount - 1; i++)
	{
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
		D3DXMatrixTranslation(&matTrans, 600.f + (i * 70.f), 45.f, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}
