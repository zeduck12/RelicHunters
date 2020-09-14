#include "pch.h"
#include "CBoss.h"
#include "CImageSetting.h"
#include "CGraphicDevice.h"
#include "CTextureManager.h"
#include "CBossState.h"
#include "CPlayer.h"
#include "CPlayerManager.h"
#include "CCollisionManager.h"
#include "CPlayerState.h"
#include "CBullet.h"
#include "CRocket.h"
#include "CObjManager.h"
#include "CShotGun.h"
#include "CMapManager.h"
#include "UICameraManager.h"
#include "CBossHpBar.h"
#include "CGenerator.h"
#include "CTimeManager.h"

CBoss::CBoss(float _fX, float _fY, float _fWidth, float _fHeight, float _fSpeed, float _fHp, IMAGE::ID _eID)
	:
	m_pBossNextState{ nullptr }
{
	m_pNextState = nullptr;
	m_fAddSpeed = 0.f;
	m_bIsDash = false;
	m_bIsCrack = false;
	m_bIsPhase2 = false;
	m_fDegree = 0.f;
	//m_fMaxHp = _fHp;
	m_fMaxHp = 4000.f;
	m_fStackTime = 0.f;

	m_tInfo.vPos = { _fX, _fY, 0.f };
	m_tInfo.vDir = { 1.0f, 0.f, 0.f };
	m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_fShootingDegree = 0.f;
	m_fShootingDist = 0.f;

	m_fSpeed = _fSpeed;
	m_fCurHp = m_fMaxHp;

	m_eImageID = _eID;
	m_pImageSetting = nullptr;

	// 기준
	m_vRotVertex[0].x = m_tInfo.vSize.x * 0.5f;
	m_vRotVertex[0].y = 0.f;

	// 나머지는 기준이 되는 점을 회전해서
	D3DXMATRIX matWorld, matRot;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(120.f));
	// 일단 회전
	matWorld = matRot;
	D3DXVec3TransformCoord(&m_vRotVertex[1], &m_vRotVertex[0], &matWorld);
	D3DXVec3TransformCoord(&m_vRotVertex[2], &m_vRotVertex[1], &matWorld);
	// 삼각형 버텍스 위치 완료
}

CBoss::~CBoss()
{
	Release();
}

void CBoss::Ready(void)
{
	// 이미지 셋팅
	m_pImageSetting = make_unique<CImageSetting>(this, "BossAnimation");
	if (!m_pImageSetting->Ready())
		return;

	m_pBossNextState = new EggIdleState;
	//m_pBossNextState = new BossIdleState;
	m_eDir = DIRECTION::RIGHT;

	shared_ptr<CBossHpBar> pBossHpBar = make_shared<CBossHpBar>(this);
	pBossHpBar->Ready();
	GET_SINGLE(UICameraManager)->SetBossHpBar(pBossHpBar);

}

int CBoss::Update(float _fDeltaTime)
{
	DetectDirection();

	CBossState* pCurState = m_pBossNextState->Update(this);
	if (pCurState != nullptr)
	{
		Safe_Delete(m_pBossNextState);
		m_pBossNextState = pCurState;
	}

	// 몬스터 방향 플레이어 기준으로 설정. 나중에 이미지 넣으면 그때는 좌우만
	UpdateMonsterDirection();

	if (m_bIsCrack == true)
		CheckGeneratorsCount();

	if (m_bIsDash == true)
		Dash();

	return 0;
}

void CBoss::LateUpdate(void)
{
	for (auto& pTile : GET_SINGLE(CMapManager)->GetWalls())
		CCollisionManager::CollideCharacterTile(this, pTile);

	for (auto& pStruc : GET_SINGLE(CMapManager)->GetStructures())
		CCollisionManager::CollideCharacterStructure(this, pStruc.get());

	// 플레이어 출동 선과 선 충돌
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	CPlayer* pRealPlayer = dynamic_cast<CPlayer*>(pPlayer);
	LINEINFO* pPlayerLineArray = dynamic_cast<CPlayer*>(pPlayer)->GetLinesInfo();
	LINEINFO* pMonsterLineArray = GetLinesInfo();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (CCollisionManager::CollideLineToLine(pPlayerLineArray[i], pMonsterLineArray[j]))
			{
				// 충돌이 일어났다면 방향벡터 쪽으로 밀기
				pPlayer->SetX(pPlayer->GetX() + m_tInfo.vDir.x);
				pPlayer->SetY(pPlayer->GetY() + m_tInfo.vDir.y);

				if (pRealPlayer->IsDead() == false && m_bIsDead == false && m_bIsCrack == true)
				{
					pRealPlayer->SetState(GET_SINGLE(PlayerAttacked));
					pRealPlayer->SetIsAttacked(true);
					pRealPlayer->TakeDamage(1.5f);
				}
			}
		}
	}

	// 다쓴 라인정보 삭제
	delete[] pPlayerLineArray;
	delete[] pMonsterLineArray;



	if (m_fCurHp <= m_fMaxHp * 0.5f && m_bIsPhase2 == false)
	{
		m_bIsPhase2 = true;
		//페이지 2상태 시작
	}



}

void CBoss::Render(const HDC& _hdc)
{
	if (m_bIsDash == true)
		ShowBossSpectrum();
	m_pBossNextState->Render(this);
	
	if(m_bIsCrack == true)
		EquipWeapon(); // 총 장착

	if(m_bIsInvicible == true)
		DrawSuperArmor();

}

void CBoss::Release(void)
{
	Safe_Delete(m_pBossNextState);
}

void CBoss::SetState(CBossState* _pState)
{
	Safe_Delete(m_pBossNextState);
	m_pBossNextState = _pState;
}

void CBoss::ShootRocket(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return;

	// 슈팅각도 계산
	float fDeltaX = pPlayer->GetX() - this->GetX();
	float fDeltaY = pPlayer->GetY() - this->GetY();
	float fDist = sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY);

	float fRadian = acosf(fDeltaX / fDist);
	if (pPlayer->GetY() < m_tInfo.vPos.y)
		fRadian = 2 * D3DX_PI - fRadian;

	D3DXVECTOR3 vDeltaPos = pPlayer->GetInfo()->vPos - this->GetInfo()->vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &vDeltaPos);

	if (m_eDir == DIRECTION::LEFT)
	{
		shared_ptr<CObj> pBullet = make_shared<CRocket>(
			this->GetX() + m_tInfo.vDir.x * 50.f - 25.f, this->GetY() + m_tInfo.vDir.y * 50.f + 25.f,
			m_tInfo.vDir, 5.f, D3DXToDegree(fRadian),
			OBJ::MONSTER);
		pBullet->Ready();
		GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBullet);
	}
	else
	{
		shared_ptr<CObj> pBullet = make_shared<CRocket>(
			this->GetX() + m_tInfo.vDir.x * 50.f + 25.f, this->GetY() + m_tInfo.vDir.y * 50.f + 25.f,
			m_tInfo.vDir, 5.f, D3DXToDegree(fRadian),
			OBJ::MONSTER);
		pBullet->Ready();
		GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBullet);
	}

}

void CBoss::ShootShotgun(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return;

	GET_SINGLE(CSoundManager)->StopSound(CSoundManager::MONSTER);
	GET_SINGLE(CSoundManager)->PlaySound((TCHAR*)L"sfx_shotgun.wav", CSoundManager::MONSTER);

	// 슈팅각도 계산
	float fDeltaX = pPlayer->GetX() - this->GetX();
	float fDeltaY = pPlayer->GetY() - this->GetY();
	float fDist = sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY);

	float fRadian = acosf(fDeltaX / fDist);
	if (pPlayer->GetY() < m_tInfo.vPos.y)
		fRadian = 2 * D3DX_PI - fRadian;

	D3DXVECTOR3 vDeltaPos = pPlayer->GetInfo()->vPos - this->GetInfo()->vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &vDeltaPos);

	// Shot Gun 발사
	shared_ptr<CBullet> pShotGun = nullptr;
	for (int i = 0; i < 3; i++)
	{
		pShotGun = make_shared<CShotGun>(this->GetX() + this->GetDirectionVector().x * 10.f,
			this->GetY() + this->GetDirectionVector().y * 10.f,
			this->GetDirectionVector(), -7.f * (i - 1), 15.f, D3DXToDegree(fRadian), OBJ::MONSTER, L"Blue");
		pShotGun->Ready();
		GET_SINGLE(CObjManager)->GetBullets().emplace_back(pShotGun);
	}

}

void CBoss::FullRangeAttack(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return;

	GET_SINGLE(CSoundManager)->StopSound(CSoundManager::MONSTER);
	GET_SINGLE(CSoundManager)->PlaySound((TCHAR*)L"sfx_shotgun.wav", CSoundManager::MONSTER);

	// 슈팅각도 계산
	float fDeltaX = pPlayer->GetX() - this->GetX();
	float fDeltaY = pPlayer->GetY() - this->GetY();
	float fDist = sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY);

	float fRadian = acosf(fDeltaX / fDist);
	if (pPlayer->GetY() < m_tInfo.vPos.y)
		fRadian = 2 * D3DX_PI - fRadian;

	D3DXVECTOR3 vDeltaPos = pPlayer->GetInfo()->vPos - this->GetInfo()->vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &vDeltaPos);

	shared_ptr<CBullet> pShotGun = nullptr;
	for (int i = 0; i < 8; i++)
	{
		pShotGun = make_shared<CShotGun>(this->GetX() + this->GetDirectionVector().x * 10.f,
			this->GetY() + this->GetDirectionVector().y * 10.f,
			this->GetDirectionVector(), -45.f * (i - 4), 15.f, D3DXToDegree(fRadian), OBJ::MONSTER, L"Blue");
		pShotGun->Ready();
		GET_SINGLE(CObjManager)->GetBullets().emplace_back(pShotGun);
	}
}

void CBoss::ShowBossSpectrum(void)
{
	if (m_fAddSpeed >= 6.f && m_fAddSpeed <= 8.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Boss", L"Move", 5);

		float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
		float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

		D3DXMATRIX matScale, matTrans, matWorld;
		if (this->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos - (this->GetInfo()->vDir * 60.f);
		vSpectrumPos.y -= 20.f;

		// 20은 렉트 중심에 이미지 맞추기 위해.
		D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}

	if (m_fAddSpeed >= 5.f && m_fAddSpeed <= 9.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Boss", L"Move", 4);

		float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
		float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

		D3DXMATRIX matScale, matTrans, matWorld;
		if (this->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos - (this->GetInfo()->vDir * 50.f);
		vSpectrumPos.y -= 20.f;

		// 20은 렉트 중심에 이미지 맞추기 위해.
		D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}

	if (m_fAddSpeed >= 4.f && m_fAddSpeed <= 10.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Boss", L"Move", 3);

		float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
		float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

		D3DXMATRIX matScale, matTrans, matWorld;
		if (this->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos - (this->GetInfo()->vDir * 40.f);
		vSpectrumPos.y -= 20.f;

		// 20은 렉트 중심에 이미지 맞추기 위해.
		D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}

	if (m_fAddSpeed >= 3.f && m_fAddSpeed <= 11.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Boss", L"Move", 2);

		float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
		float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

		D3DXMATRIX matScale, matTrans, matWorld;
		if (this->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos - (this->GetInfo()->vDir * 30.f);
		vSpectrumPos.y -= 20.f;

		D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}

	if (m_fAddSpeed >= 2.f && m_fAddSpeed <= 12.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Boss", L"Move", 1);

		float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
		float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

		D3DXMATRIX matScale, matTrans, matWorld;
		if (this->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos - (this->GetInfo()->vDir * 20.f);
		vSpectrumPos.y -= 20.f;

		D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}

	if (m_fAddSpeed >= 1.f && m_fAddSpeed <= 13.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Boss", L"Move", 0);

		float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
		float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

		D3DXMATRIX matScale, matTrans, matWorld;
		if (this->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos - (this->GetInfo()->vDir * 10.f);
		vSpectrumPos.y -= 20.f;

		// 20은 렉트 중심에 이미지 맞추기 위해.
		D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}
}

void CBoss::InstallGenerators(void)
{
	shared_ptr<CObj> pMonster = make_shared<CGenerator>(
		1219.f,
		1290.f,
		50.f, 50.f);
	pMonster->Ready();
	GET_SINGLE(CObjManager)->GetMonsters().emplace_back(pMonster);

	pMonster = make_shared<CGenerator>(
		1905.f,
		1266.f,
		50.f, 50.f);
	pMonster->Ready();
	GET_SINGLE(CObjManager)->GetMonsters().emplace_back(pMonster);

	pMonster = make_shared<CGenerator>(
		1261.f,
		2140.f,
		50.f, 50.f);
	pMonster->Ready();
	GET_SINGLE(CObjManager)->GetMonsters().emplace_back(pMonster);

	pMonster = make_shared<CGenerator>(
		1950.f,
		2172.f,
		50.f, 50.f);
	pMonster->Ready();
	GET_SINGLE(CObjManager)->GetMonsters().emplace_back(pMonster);

}

void CBoss::DrawSuperArmor(void)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"SuperArmor");
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 2.4f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBoss::CheckGeneratorsCount(void)
{
	if (GET_SINGLE(CObjManager)->GetMonsters().size() <= 1)
	{
		m_bIsInvicible = false;
		m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fStackTime >= 7.f)
		{
			m_fStackTime = 0.f;
			m_bIsInvicible = true;
			InstallGenerators();
		}
	}

}

bool CBoss::IsDetectPlayerBossVersion(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_VALID_OBJ(pPlayer)
	{
		// 범위내에 플레이어가 있다면 true or 없다면 false.
		if (pPlayer->GetX() > this->GetX() - 600.f &&
			pPlayer->GetX() < this->GetX() + 600.f &&
			pPlayer->GetY() > this->GetY() - 600.f &&
			pPlayer->GetY() < this->GetY() + 600.f)
		{
			return true;
		}
	}

	return false;
}

bool CBoss::IsInAttackRangePlayerBossVersion(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_VALID_OBJ(pPlayer)
	{
		// 범위내에 플레이어가 있다면 true or 없다면 false.
		if (pPlayer->GetX() > this->GetX() - 400.f &&
			pPlayer->GetX() < this->GetX() + 400.f &&
			pPlayer->GetY() > this->GetY() - 400.f &&
			pPlayer->GetY() < this->GetY() + 400.f)
		{
			return true;
		}
	}

	return false;
}

