#include "pch.h"
#include "CMonster.h"
#include "CObjManager.h"
#include "CPlayerManager.h"
#include "CCollisionManager.h"
#include "CGrenade.h"
#include "CPlayer.h"
#include "CMonsterState.h"
#include "CBullet.h"

// 삼각형 몬스터 
CMonster::CMonster(float _fX, float _fY, float _fWidth, float _fHeight, float _fSpeed, float _fHp)
	:
	m_fMaxHp{ _fHp },
	m_pNextState{ nullptr }
{
	m_tInfo.vPos = { _fX, _fY, 0.f };
	m_tInfo.vDir = { 1.0f, 0.f, 0.f };
	m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_fDegree = 0.f;
	m_fSpeed = _fSpeed;
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
	m_pNextState = new IdleState;
}

int CMonster::Update(float _fDeltaTime)
{
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
	// 삼각형 그리기
	MoveToEx(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y, nullptr);
	for (int i = 1; i < 3; i++)
		LineTo(_hdc, (int)m_vRealVertex[i].x, (int)m_vRealVertex[i].y);
	LineTo(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y);
	
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
			this->GetX(), this->GetY(),
			m_tInfo.vDir, 5.f, D3DXToDegree(fRadian),
			OBJ::MONSTER);
	pBullet->Ready();
	GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBullet);

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
