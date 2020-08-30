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

CBoss::CBoss(float _fX, float _fY, float _fWidth, float _fHeight, float _fSpeed, float _fHp, IMAGE::ID _eID)
	:
	m_pBossNextState{ nullptr }
{
	m_fDegree = 0.f;
	m_fMaxHp = _fHp;
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

	m_pBossNextState = new BossIdleState;
	m_eDir = DIRECTION::RIGHT;
}

int CBoss::Update(float _fDeltaTime)
{
	DetectDirection();

	CBossState* pCurState = m_pBossNextState->Update(this);
	if (pCurState != nullptr)
	{
		delete	m_pBossNextState;
		m_pBossNextState = nullptr;

		m_pBossNextState = pCurState;
	}

	// 몬스터 방향 플레이어 기준으로 설정. 나중에 이미지 넣으면 그때는 좌우만
	UpdateMonsterDirection();

	return 0;
}

void CBoss::LateUpdate(void)
{
	// 플레이어 출동 선과 선 충돌
	//CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	//LINEINFO* pPlayerLineArray = dynamic_cast<CPlayer*>(pPlayer)->GetLinesInfo();
	//LINEINFO* pMonsterLineArray = GetLinesInfo();

	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		if (CCollisionManager::CollideLineToLine(pPlayerLineArray[i], pMonsterLineArray[j]))
	//		{
	//			// 충돌이 일어났다면 방향벡터 쪽으로 밀기
	//			pPlayer->SetX(pPlayer->GetX() + m_tInfo.vDir.x);
	//			pPlayer->SetY(pPlayer->GetY() + m_tInfo.vDir.y);
	//			dynamic_cast<CPlayer*>(pPlayer)->SetState(GET_SINGLE(PlayerAttacked));
	//		}
	//	}
	//}

	//// 다쓴 라인정보 삭제
	//delete[] pPlayerLineArray;
	//delete[] pMonsterLineArray;
}

void CBoss::Render(const HDC& _hdc)
{
	m_pBossNextState->Render(this);

	EquipWeapon(); // 총 장착

	// 삼각형 그리기
	MoveToEx(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y, nullptr);
	for (int i = 1; i < 3; i++)
		LineTo(_hdc, (int)m_vRealVertex[i].x, (int)m_vRealVertex[i].y);
	LineTo(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y);


	// 뷰 포인트
	RECT rcEllipse = { LONG(m_vRealVertex[0].x - 10), LONG(m_vRealVertex[0].y - 10),
		LONG(m_vRealVertex[0].x + 10) , LONG(m_vRealVertex[0].y + 10) };
	Ellipse(_hdc, rcEllipse.left, rcEllipse.top, rcEllipse.right, rcEllipse.bottom);
}

void CBoss::Release(void)
{
	SAFE_DELETE(m_pBossNextState);
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

	int iRandNum = rand() % 10 + 1;
	for (int i = 0; i < 2; i++)
	{
		pShotGun = make_shared<CShotGun>(this->GetX() + this->GetDirectionVector().x * 10.f,
			this->GetY() + this->GetDirectionVector().y * 10.f,
			this->GetDirectionVector(), -float(iRandNum) * (i - 1), 15.f, D3DXToDegree(fRadian), OBJ::MONSTER, L"Blue");
		pShotGun->Ready();
		GET_SINGLE(CObjManager)->GetBullets().emplace_back(pShotGun);
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

