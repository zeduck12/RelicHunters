#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CBoomerang.h"
#include "CShotGun.h"
#include "CGrenade.h"
#include "CObjManager.h"
#include "CCollisionManager.h"
#include "CKeyManager.h"
#include "CMonster.h"
#include "CCasing.h"
#include "CInventory.h"
#include "CPlayerManager.h"
#include "CWeapon.h"
#include "CTimeManager.h"
#include "CCameraManager.h"
#include "CGraphicDevice.h"
#include "CTextureManager.h"
#include "CAnimation.h"
#include "CPlayerState.h"
#include "CImageSetting.h"
#include "CShadow.h"
#include "CItem.h"
#include "CSceneManager.h"
#include "CShield.h"
#include "CMapManager.h"
#include "CStructure.h"
#include "CParticle.h"


CPlayer::CPlayer()
	:
	m_vRotVertex{},
	m_vRealVertex{},
	m_fShootingDegree{ 0.f },
	m_fShootingDist{ 0.f },
	m_fAddSpeed{ 1.f },
	m_bIsDash{ false },
	m_pWeapon{ nullptr },
	m_pShield{ nullptr },
	m_fStackTime{ 0.f },
	m_pCurState{ nullptr },
	m_bIsAttacked{ false },
	m_pImageSetting{ nullptr },
	m_bIsReloading{ false },
	m_fCurHp{ 0.f },
	m_fMaxHp{ 0.f },
	m_strDashName{ L"" }
{
	ZeroMemory(&m_rcShadowRect, sizeof(RECT));
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tPosin, sizeof(LINEINFO));
	ZeroMemory(&m_tCrossPos, sizeof(LINEPOS));
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Ready()
{
	m_eImageID =IMAGE::PLAYER;

	CSceneManager::ID eSceneID = GET_SINGLE(CSceneManager)->GetNextSceneID();
	if(eSceneID == CSceneManager::SCENE_GAME)
		m_tInfo.vPos = {800.f, 600.f, 0.f};
	else if (eSceneID == CSceneManager::SCENE_GAME2)
		m_tInfo.vPos = { 450.f, 500.f, 0.f };
	else if (eSceneID == CSceneManager::SCENE_GAME3)
		m_tInfo.vPos = { 830.f, 576.f, 0.f };
	else if (eSceneID == CSceneManager::SCENE_GAME4)
		m_tInfo.vPos = { 548.f, 767.f, 0.f };
	else if (eSceneID == CSceneManager::SCENE_TEST)
		m_tInfo.vPos = { 1000.f, 1000.f, 0.f };

	m_tInfo.vDir = {1.0f, 0.f, 0.f};
	m_tInfo.vSize = { 50.f, 50.f, 0.f };
	m_tInfo.vLook = {1.f, 0.f, 0.f};

	m_fSpeed = 4.f;
	m_fDegree = 0.f;
	m_fMaxHp = 200.f;
	m_fDashMaxHp = 200.f;
	m_fCurHp = m_fMaxHp;
	m_fDashCurHp = m_fDashMaxHp;

	// 회전할 버텍스 좌표
	m_vRotVertex[0].x = - (m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[0].y = - (m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[1].x = + (m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[1].y = - (m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[2].x = + (m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[2].y = + (m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[3].x = - (m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[3].y = + (m_tInfo.vSize.y * 0.5f);
	
	m_eDir = DIRECTION::END;

	// 여기서 무기 세팅하고 셋팅
	GET_SINGLE(CPlayerManager)->GetInventory()->GainWeapon(GUN::DEFAULT);
	GET_SINGLE(CPlayerManager)->GetInventory()->GainWeapon(GUN::DEFAULT);

	// 무기 생성
	m_pWeapon = make_unique<CWeapon>();
	m_pWeapon->Ready();

	// 쉴드 생성
	m_pShield = make_unique<CShield>();
	m_pShield->Ready();

	// 애니메이션 생성
	m_pImageSetting = make_unique<CImageSetting>(this, "PlayerAnimation");
	if (!m_pImageSetting->Ready())
		return;

	// 상태초기화
	if (m_pCurState == nullptr)
	{
		m_pCurState = GET_SINGLE(PlayerIdleState);
		m_eDir = DIRECTION::RIGHT;
	}
}

int CPlayer::Update(float _fDeltaTime)
{
	UpdateDashName();
	// 포신좌표 갱신
	UpdatePosinInfo();
	RecoverDashHp();

	// 방향감지
	DetectDirection();
	TrackMousePos();
	CheckKeyState();

	if(m_bIsDash == true)
		Dash();

	if (m_bIsAttack == true)
		CheckDelaySniper();

	m_pShield->Update();
	m_pWeapon->Update();
	m_pCurState->Update(this); // 상태 업데이트


	return 0;
}

void CPlayer::LateUpdate()
{
	if (GET_SINGLE(CCameraManager)->IsPressing() == true)
	{
		// 벽관 충돌했을때
		LINEINFO* pLineArray = nullptr;
		for (auto& pTile : GET_SINGLE(CMapManager)->GetWalls())
		{
			pLineArray = pTile->GetLinesInfo();
			for (int i = 0; i < 4; i++)
			{
				if (CCollisionManager::CollideLineToLineReturn(pLineArray[i], m_tPosin, &m_tCrossPos))
				{
					m_tPosin.tRPoint.fX = m_tCrossPos.fX;
					m_tPosin.tRPoint.fY = m_tCrossPos.fY;
				}
			}

			Safe_Delete(pLineArray);
		};

		// 구조물과 충돌했을때
		for (auto& pObj : GET_SINGLE(CMapManager)->GetStructures())
		{
			CStructure* pStructure = dynamic_cast<CStructure*>(pObj.get());
			pLineArray = pStructure->GetLinesInfo();
			for (int i = 0; i < 4; i++)
			{
				if (CCollisionManager::CollideLineToLineReturn(pLineArray[i], m_tPosin, &m_tCrossPos))
				{
					m_tPosin.tRPoint.fX = m_tCrossPos.fX;
					m_tPosin.tRPoint.fY = m_tCrossPos.fY;
				}
			}

			Safe_Delete(pLineArray);
		};
	}


}

void CPlayer::Render(const HDC& _hdc)
{

	// Line으로 그리기
	//MoveToEx(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y, nullptr);
	//for (int i = 1; i < 4; i++)
	//	LineTo(_hdc, (int)m_vRealVertex[i].x, (int)m_vRealVertex[i].y);
	//LineTo(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y);

	m_pWeapon->DrawSubGun();

	if (m_bIsDash)
		ShowSpectrum(_hdc);
	else
		m_pCurState->Render(this); // 상태 Render

	// 무기 그리기
	m_pWeapon->Render(_hdc);
	m_pShield->Render(_hdc);

	if (GET_SINGLE(CCameraManager)->IsPressing() == true)
	{
		HPEN myPen, oldPen;
		HBRUSH myBrush, oldBrush;
		myPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(_hdc, myPen);
		myBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(_hdc, myBrush);

		// 포신그리기
		MoveToEx(_hdc, (int)m_tPosin.tLPoint.fX, (int)m_tPosin.tLPoint.fY, nullptr);
		LineTo(_hdc, (int)m_tPosin.tRPoint.fX, (int)m_tPosin.tRPoint.fY);
		// 포신 머리판
		Ellipse(_hdc, (int)m_tPosin.tRPoint.fX - 5, (int)m_tPosin.tRPoint.fY - 5,
			(int)m_tPosin.tRPoint.fX + 5, (int)m_tPosin.tRPoint.fY + 5);


		SelectObject(_hdc, oldPen);
		SelectObject(_hdc, oldBrush);
		//쓰고난 펜을 삭제해준다.
		DeleteObject(myPen);
		DeleteObject(myBrush);
	}

	if (m_bIsInvicible == true)
	{
		TCHAR str[10] = {};
		wsprintf(str, TEXT("Debug"), int(m_fShootingDegree));
		TextOut(_hdc, (int)m_tInfo.vPos.x - 10.f, (int)m_tInfo.vPos.y + 20.f, str, lstrlen(str));
	}

}

void CPlayer::Release()
{
	m_pWeapon.reset();
	m_pShield.reset();
	m_pImageSetting.reset();

	PlayerIdleState::Destroy_Instance();
	PlayerMoveState::Destroy_Instance();
	PlayerAttacked::Destroy_Instance();
	PlayerDeath::Destroy_Instance();
}

void CPlayer::TrackMousePos(void)
{
	// 목적지 - 출발지
	// 마우스 위치를 받고 그 마우스 방향으로 포신 움직이게
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	D3DXVECTOR3 vMouse = { float(pt.x) + m_tInfo.vPos.x - (WINCX>>1), float(pt.y) + m_tInfo.vPos.y - (WINCY >> 1), 0.f };
	m_tInfo.vDir = vMouse - m_tInfo.vPos;

	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
}

void CPlayer::CheckKeyState(void)
{
	if (m_bIsDead)
		return;

	D3DXMATRIX matWorld;
	D3DXMATRIX matParent;
	// 행렬 초기화
	D3DXMatrixIdentity(&matWorld);

	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_T))
	{
		if (m_bIsInvicible == true)
			m_bIsInvicible = false;
		else
			m_bIsInvicible = true;
	}

	// 플레이어 이동
	if (GET_SINGLE(CKeyManager)->Key_Pressing(KEY_A))
	{
		m_tInfo.vPos.x -= m_fSpeed;
		if(m_bIsAttacked == false)
			SetState(GET_SINGLE(PlayerMoveState));

	}
	if (GET_SINGLE(CKeyManager)->Key_Pressing(KEY_D))
	{
		m_tInfo.vPos.x += m_fSpeed;
		if (m_bIsAttacked == false)
			SetState(GET_SINGLE(PlayerMoveState));
	}
	if (GET_SINGLE(CKeyManager)->Key_Pressing(KEY_W))
	{
		m_tInfo.vPos.y -= m_fSpeed;
		if (m_bIsAttacked == false)
			SetState(GET_SINGLE(PlayerMoveState));
	}
	if (GET_SINGLE(CKeyManager)->Key_Pressing(KEY_S))
	{
		m_tInfo.vPos.y += m_fSpeed;
		if (m_bIsAttacked == false)
			SetState(GET_SINGLE(PlayerMoveState));
	}

	// 나중에 크기 * 자전 * 이동 * 공전 * 부모
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matParent;

	// 월드 행렬 적용해서 플레이어 움직임.
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&m_vRealVertex[i], &m_vRotVertex[i], &matWorld);

	// 수류탄
	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_F))
	{
		int iBombsCount = GET_SINGLE(CPlayerManager)->GetInventory()->GetBombsCount();
		if (GET_SINGLE(CPlayerManager)->GetInventory()->GetBombsCount() <= 0)
		{
			GET_SINGLE(CPlayerManager)->GetInventory()->SetBombsCount(0);
			return;
		}

		GET_SINGLE(CPlayerManager)->GetInventory()->SetBombsCount(iBombsCount - 1);
		shared_ptr<CGrenade> pGrenade = make_shared<CGrenade>(m_tInfo.vPos.x + m_tInfo.vDir.x * 50.f,
			m_tInfo.vPos.y + m_tInfo.vDir.y * 50.f, m_tInfo.vDir, 20.f, m_fShootingDegree, m_fShootingDist);
		pGrenade->Ready();
		GET_SINGLE(CObjManager)->GetGrenades().emplace_back(pGrenade);

		pGrenade = make_shared<CGrenade>(m_tInfo.vPos.x + m_tInfo.vDir.x * 50.f,
			m_tInfo.vPos.y + m_tInfo.vDir.y * 50.f, m_tInfo.vDir, 20.f, m_fShootingDegree, m_fShootingDist, true);
		pGrenade->Ready();
		GET_SINGLE(CObjManager)->GetGrenades().emplace_back(pGrenade);
	}

	// 연사
	if (GET_SINGLE(CKeyManager)->Key_Pressing(KEY_LBUTTON) && m_pWeapon->GetCurWeaponID() == GUN::MACHINEGUN)
	{
		m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fStackTime >= 0.15f)
		{
			m_fStackTime = 0.f;
			// 무기에서 총알 발사
			m_pWeapon->Shoot();
		}
	}

	// 단발
	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_LBUTTON))
	{
		if (m_pWeapon->GetCurWeaponID() == GUN::SNIPER)
		{
			//m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
			//if (m_fStackTime >= 0.2f)
			//{
			//	m_fStackTime = 0.f;
			//	// 무기에서 총알 발사
			//	m_pWeapon->Shoot();
			//}
			if (m_bIsAttack == true)
				return;

			m_bIsAttack = true;
			m_pWeapon->Shoot();
		}

		if(m_pWeapon->GetCurWeaponID() != GUN::SNIPER)
			m_pWeapon->Shoot();
	}

	// 장전
	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_R))
	{
		if (m_pWeapon->ReloadBullets() == true)
			m_bIsReloading = true; // 장전중
		
	}

	// 무기 교체
	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_TAB))
		GET_SINGLE(CPlayerManager)->GetInventory()->ChangeWeapon();

	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_RBUTTON)) // 카메라 확대
	{
		GET_SINGLE(CCameraManager)->SetIsPressing(true);
		GET_SINGLE(CCameraManager)->SetIsEnlargeScale(true);
	}

	if (GET_SINGLE(CKeyManager)->Key_UP(KEY_RBUTTON)) // 카메라 축소
	{
		GET_SINGLE(CCameraManager)->SetIsPressing(false);
		GET_SINGLE(CCameraManager)->SetIsReduceScale(true);
	}


	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_SHIFT)) // 플레이어 대쉬
	{
		if (m_fDashCurHp < 50.f)
			return;

		m_bIsDash = true;
		m_fDashCurHp -= 50.f;

		shared_ptr<CObj> pParticle = make_shared<CParticle>(this->GetX(), this->GetY()
			,CParticle::DASH, 6, L"Dash", this);
		pParticle->Ready();
		GET_SINGLE(CObjManager)->GetParticles().emplace_back(pParticle);
	}



	// Test용
	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_G))
	{
		float fRandom = GetNumberMinBetweenMax(0.f, 360.f);
		// 아이템 드랍되게
		shared_ptr<CObj> pItem = make_shared<CStarCoin>(m_tInfo.vPos.x, m_tInfo.vPos.y, 30.f, 30.f, IMAGE::COIN, fRandom);
		pItem->Ready();
		GET_SINGLE(CObjManager)->GetItems().emplace_back(pItem);
	}

}

void CPlayer::DetectDirection(void)
{
	// 마우스 위치를 기준으로 방향 설정
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	// 마우스 좌표
	D3DXVECTOR3 vMouse = { float(pt.x) + m_tInfo.vPos.x - (WINCX >> 1), float(pt.y) + m_tInfo.vPos.y - (WINCY >> 1), 0.f };

	float fDeltaX = vMouse.x - this->GetX();
	float fDeltaY = vMouse.y - this->GetY();

	float fDist = sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY);

	float fRadian = acosf(fDeltaX / fDist);
	if (vMouse.y < m_tInfo.vPos.y)
		fRadian = 2 * D3DX_PI - fRadian;
	float fDegree = D3DXToDegree(fRadian);
	m_fShootingDegree = fDegree;
	m_fShootingDist = fDist;

	if (90.f < fDegree && fDegree < 270.f)
		m_eDir = DIRECTION::LEFT;
	else
		m_eDir = DIRECTION::RIGHT;

}

void CPlayer::UpdatePosinInfo(void)
{
	// 포신선분 좌표
	m_tPosin.tLPoint.fX = m_tInfo.vPos.x;
	m_tPosin.tLPoint.fY = m_tInfo.vPos.y;
	m_tPosin.tRPoint.fX = m_tInfo.vPos.x + m_tInfo.vDir.x * 300.f;
	m_tPosin.tRPoint.fY = m_tInfo.vPos.y + m_tInfo.vDir.y * 300.f;
}

void CPlayer::Dash(void)
{
	if (m_fAddSpeed >= 14.f)
	{
		m_fAddSpeed = 0.f;
		m_bIsDash = false;
		return;
	}

	m_fAddSpeed += 0.8f;
	m_tInfo.vPos += m_tInfo.vDir * m_fAddSpeed;
}

void CPlayer::RecoverDashHp(void)
{
	if (m_fDashCurHp >= m_fDashMaxHp)
		return;

	m_fDashCurHp += 10.f * GET_SINGLE(CTimeManager)->GetElapsedTime();
}

// 대쉬 잔상
void CPlayer::ShowSpectrum(const HDC& _hdc)
{

	if (m_fAddSpeed >= 5.f && m_fAddSpeed <= 6.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Player", m_strDashName, 2);

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
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(150, 255, 255, 255));

	}
	
	if (m_fAddSpeed >= 3.f && m_fAddSpeed <= 8.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Player", m_strDashName, 1);

		float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
		float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

		D3DXMATRIX matScale, matTrans, matWorld;
		if (this->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos - (this->GetInfo()->vDir * 20.f);
		vSpectrumPos.y -= 20.f;

		// 20은 렉트 중심에 이미지 맞추기 위해.
		D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(150, 255, 255, 255));
	}

	if (m_fAddSpeed >= 1.f && m_fAddSpeed <= 10.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Player", m_strDashName, 0);

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
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(150, 255, 255, 255));
	}

	const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Player", m_strDashName, 0);

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if (this->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos;
	vSpectrumPos.y -= 20.f;

	// 20은 렉트 중심에 이미지 맞추기 위해.
	D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderSheetShadow(this, pTexInfo);
}

void CPlayer::UpdateDashName(void)
{
	PLAYER::ID ePlayerID = GET_SINGLE(CSceneManager)->GetPlayerID();
	switch (ePlayerID)
	{
	case PLAYER::JIMMY:
		m_strDashName = L"Dash";
		break;
	case PLAYER::PINKY:
		m_strDashName = L"Pinky_Dash";
		break;
	case PLAYER::RAFF:
		m_strDashName = L"Raff_Dash";
		break;
	}
}

void CPlayer::CheckDelaySniper(void)
{
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 0.7f)
	{
		m_fStackTime = 0.f;
		m_bIsAttack = false;
	}
}





