#include "pch.h"
#include "CMonster.h"
#include "CObjManager.h"
#include "CPlayerManager.h"
#include "CCollisionManager.h"
#include "CGrenade.h"
#include "CPlayer.h"
#include "CMonsterState.h"
#include "CBullet.h"
#include "CImageSetting.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CPlayerState.h"
#include "CTimeManager.h"


// 삼각형 몬스터 
CMonster::CMonster(float _fX, float _fY, float _fWidth, float _fHeight,
	float _fSpeed, float _fHp, IMAGE::ID _eID /*= IMAGE::DUCK*/)
	:
	m_fMaxHp{ _fHp },
	m_pNextState{ nullptr },
	m_pImageSetting{ nullptr },
	m_fStackTime{ 0.f },
	m_bIsFlying{ false }
{
	m_eImageID = _eID;

	m_tInfo.vPos = { _fX, _fY, 0.f };
	m_tInfo.vDir = { 1.0f, 0.f, 0.f };
	m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_fShootingDegree = 0.f;
	m_fShootingDist = 0.f;

	m_fDegree = 0.f;
	m_fSpeed = _fSpeed;
	m_bIsDash = false;
	m_fAddSpeed = 0.f;
	m_fCurHp = m_fMaxHp;

	// 기준
	m_vRotVertex[0].x = m_tInfo.vSize.x * 0.5f;
	m_vRotVertex[0].y = 0.f ;

	// 나머지는 기준이 되는 점을 회전해서
	D3DXMATRIX matWorld, matRot;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(120.f));
	// 일단 회전
	matWorld = matRot ;
	D3DXVec3TransformCoord(&m_vRotVertex[1], &m_vRotVertex[0], &matWorld);
	D3DXVec3TransformCoord(&m_vRotVertex[2], &m_vRotVertex[1], &matWorld);
	// 삼각형 버텍스 위치 완료

}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Ready(void)
{
	// 애니메이션 생성
	m_pImageSetting = make_unique<CImageSetting>(this, "MonsterAnimation");
	if (!m_pImageSetting->Ready())
		return;

	m_pNextState = new IdleState;
	m_eDir = DIRECTION::RIGHT;
}

int CMonster::Update(float _fDeltaTime)
{
	DetectDirection();

	CMonsterState* pCurState = m_pNextState->Update(this);
	if (pCurState != nullptr)
	{
		delete	m_pNextState;
		m_pNextState = nullptr;

		m_pNextState = pCurState;
	}

	if (m_pNextState != nullptr)
		m_pNextState->Update(this);

	// 몬스터 방향 플레이어 기준으로 설정. 나중에 이미지 넣으면 그때는 좌우만
	UpdateMonsterDirection();

	if (m_bIsDash == true)
		Dash();

    return 0;
}

void CMonster::LateUpdate(void)
{
	// 수류탄 충돌
	for (auto& pGrenade : CObjManager::Get_Instance()->GetGrenades())
		CCollisionManager::CollideWallGrenade(this, pGrenade.get());

	// 플레이어 출동 선과 선 충돌
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	LINEINFO* pPlayerLineArray = dynamic_cast<CPlayer*>(pPlayer)->GetLinesInfo();
	LINEINFO* pMonsterLineArray = GetLinesInfo();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (CCollisionManager::CollideLineToLine( pPlayerLineArray[i], pMonsterLineArray[j]))
			{
				// 충돌이 일어났다면 방향벡터 쪽으로 밀기
				pPlayer->SetX(pPlayer->GetX() + m_tInfo.vDir.x);
				pPlayer->SetY(pPlayer->GetY() + m_tInfo.vDir.y);
				dynamic_cast<CPlayer*>(pPlayer)->SetState(GET_SINGLE(PlayerAttacked));
			}
		}
	}

	// 다쓴 라인정보 삭제
	delete[] pPlayerLineArray;
	delete[] pMonsterLineArray;
	
}

void CMonster::Release(void)
{
	SAFE_DELETE(m_pNextState);
}

void CMonster::Render(const HDC& _hdc)
{
	if (m_bIsDash == true)
		ShowSpectrum();
	m_pNextState->Render(this);


	// 몬스터 아이디에 따라 총장착 여부 달라짐.
	if(m_eImageID != IMAGE::KAMIKAZE && m_eImageID != IMAGE::KAMIKAZE_FLY)
		EquipWeapon(); // 총 장착

	// 삼각형 그리기
	MoveToEx(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y, nullptr);
	for (int i = 1; i < 3; i++)
		LineTo(_hdc, (int)m_vRealVertex[i].x, (int)m_vRealVertex[i].y);
	LineTo(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y);
	

	// 뷰 포인트
	RECT rcEllipse = { LONG(m_vRealVertex[0].x - 10), LONG(m_vRealVertex[0].y - 10),
		LONG(m_vRealVertex[0].x + 10) , LONG(m_vRealVertex[0].y + 10 )};
	Ellipse(_hdc, rcEllipse.left, rcEllipse.top, rcEllipse.right, rcEllipse.bottom);


}

bool CMonster::IsInAttackRangePlayer(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_VALID_OBJ(pPlayer)
	{
		// 범위내에 플레이어가 있다면 true or 없다면 false.
		if (pPlayer->GetX() > this->GetX() - cfMonsterAttackRange &&
			pPlayer->GetX() < this->GetX() + cfMonsterAttackRange &&
			pPlayer->GetY() > this->GetY() - cfMonsterAttackRange &&
			pPlayer->GetY() < this->GetY() + cfMonsterAttackRange)
		{
			return true;
		}
	}

	return false;
}

void CMonster::UpdateMonsterDirection(void)
{
	// 플레이어를 인지하면 쫓아감.
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return;

	float fDeltaX = pPlayer->GetX() - this->GetX();
	float fDeltaY = pPlayer->GetY() - this->GetY();
	float fDist = sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY);

	float fRadian = acosf(fDeltaX / fDist);
	if (pPlayer->GetY() < m_tInfo.vPos.y)
		fRadian = 2 * D3DX_PI - fRadian;

	D3DXMATRIX matWorld, matParent, matRot;
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	D3DXMatrixRotationZ(&matRot, fRadian);
	matWorld = matRot * matParent;

	// 월드 행렬 적용해서 중심을 잡아줌.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformCoord(&m_vRealVertex[i], &m_vRotVertex[i], &matWorld);
}

void CMonster::DetectDirection(void)
{
	// 플레이어 위치를 기준으로 방향 설정
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return;


	// 플레이어 좌표
	D3DXVECTOR3 vPlayerPos = pPlayer->GetInfo()->vPos;

	float fDeltaX = vPlayerPos.x - this->GetX();
	float fDeltaY = vPlayerPos.y - this->GetY();

	float fDist = sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY);

	float fRadian = acosf(fDeltaX / fDist);
	if (pPlayer->GetY() < m_tInfo.vPos.y)
		fRadian = 2 * D3DX_PI - fRadian;
	float fDegree = D3DXToDegree(fRadian);
	m_fShootingDegree = fDegree;
	m_fShootingDist = fDist;

	if (90.f < fDegree && fDegree < 270.f)
		m_eDir = DIRECTION::LEFT;
	else
		m_eDir = DIRECTION::RIGHT;

}

void CMonster::EquipWeapon(void)
{
	wstring szWeaponName = L"";
	if (m_eImageID == IMAGE::TURTLE)
		szWeaponName = L"Flame";
	else
		szWeaponName = L"Pistol";


	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", szWeaponName, 0);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matWorld, matScale, matRev, matParent;
	
	D3DXMatrixRotationZ(&matRev, D3DXToRadian(m_fShootingDegree));

	if (m_eDir == DIRECTION::LEFT)
	{
		D3DXMatrixScaling(&matScale, 1.f, -1.f, 1.f);
		D3DXMatrixTranslation(&matParent, this->GetX() - 14.f , this->GetY() + 3, 0.f);
	}
	else
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matParent, this->GetX() + 14.f , this->GetY() + 3, 0.f);
	}


	matWorld = matScale * matRev * matParent;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

// 총알 발사
void CMonster::Shoot(void)
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

	// 총알 생성
	shared_ptr<CObj> pBullet = make_shared<CBullet>(
			this->GetX() + m_tInfo.vDir.x * 50.f, this->GetY() + m_tInfo.vDir.y * 50.f,
			m_tInfo.vDir, 5.f, D3DXToDegree(fRadian),
			OBJ::MONSTER);
	pBullet->Ready();
	GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBullet);

}

void CMonster::Dash(void)
{
	if (m_fAddSpeed >= 13.f)
		return;

	m_fAddSpeed += 0.2f;
	m_tInfo.vPos += m_tInfo.vDir * m_fAddSpeed;
}

void CMonster::ShowSpectrum(void)
{
	if (m_fAddSpeed >= 7.f && m_fAddSpeed <= 7.5f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Kamikaze", L"Dash", 6);

		float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
		float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

		D3DXMATRIX matScale, matTrans, matWorld;
		if (this->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos - (this->GetInfo()->vDir * 70.f);
		vSpectrumPos.y -= 20.f;

		// 20은 렉트 중심에 이미지 맞추기 위해.
		D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_fAddSpeed >= 6.f && m_fAddSpeed <= 8.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Kamikaze", L"Dash", 5);

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
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_fAddSpeed >= 5.f && m_fAddSpeed <= 9.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Kamikaze", L"Dash", 4);

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
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_fAddSpeed >= 4.f && m_fAddSpeed <= 10.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Kamikaze", L"Dash", 3);

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
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_fAddSpeed >= 3.f && m_fAddSpeed <= 11.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Kamikaze", L"Dash", 2);

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
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_fAddSpeed >= 2.f && m_fAddSpeed <= 12.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Kamikaze", L"Dash", 1);

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
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_fAddSpeed >= 1.f && m_fAddSpeed <= 13.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Kamikaze", L"Dash", 0);

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
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CMonster::Fly(void)
{
	// 단순히 올라가는 함수
	m_tInfo.vPos.y -= 1.5f;
}

void CMonster::Patrol(void)
{
	// 몇초동안 이렇게 한쪽으로 갔다가
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime < 5.f)
	{

		if (m_fStackTime < 2.f)
		{
			m_tInfo.vPos.x += 1.5f;
			m_tInfo.vPos.y += -0.6f;
		}
		else
		{
			m_tInfo.vPos.x -= 1.5f;
			m_tInfo.vPos.y += 0.6f;
		}
	}
	// 몇초동안 반대로 가기
	else if(5.f < m_fStackTime && m_fStackTime < 10.f)
	{
		
		if (m_fStackTime < 8.f)
		{
			m_tInfo.vPos.x -= 1.5f;
			m_tInfo.vPos.y += -0.6f;
		}
		else
		{
			m_tInfo.vPos.x += 1.5f;
			m_tInfo.vPos.y += 0.6f;
		}
	}
}

void CMonster::Landing(void)
{
	// 단순히 내랴가는 함수
	m_tInfo.vPos.y += 1.5f;
}

void CMonster::SetState(CMonsterState* _pState)
{
	if (m_pNextState)
	{
		delete m_pNextState;
		m_pNextState = nullptr;
	}

	m_pNextState = _pState;
}


bool CMonster::IsDetectPlayer(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_VALID_OBJ(pPlayer)
	{
		// 범위내에 플레이어가 있다면 true or 없다면 false.
		if (pPlayer->GetX() > this->GetX() - cfMonsterDetectRange &&
			pPlayer->GetX() < this->GetX() + cfMonsterDetectRange &&
			pPlayer->GetY() > this->GetY() - cfMonsterDetectRange &&
			pPlayer->GetY() < this->GetY() + cfMonsterDetectRange)
		{
			return true;
		}
	}

	return false;
}
