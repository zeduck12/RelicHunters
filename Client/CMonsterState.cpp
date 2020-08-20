#include "pch.h"
#include "CMonsterState.h"
#include "CTimeManager.h"
#include "CPlayerManager.h"
#include "CPlayer.h"
#include "CMonster.h"


CMonsterState* IdleState::Update(CMonster* _pMonster)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;
	
	// 만약에 플레이어를 감지했다면
	if (_pMonster->IsDetectPlayer() == true)
		return new TrackingState;

	// 상태 유지
	return nullptr;
}

void IdleState::Render(CMonster* _pMonster, const HDC& _hdc)
{
	// 애니메이션 출력
}

CMonsterState* TrackingState::Update(CMonster* _pMonster)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	// 만약에 플레이어가 감지 범위를 벗어나면 다시 Idle 상태로
	if (_pMonster->IsDetectPlayer() == false)
		return new IdleState;

	// 만약에 공격범위 안에 들어온다면 AttackState로
	if (_pMonster->IsInAttackRangePlayer() == true)
		return new AttackState;

	// 방향벡터 구함.
	D3DXVECTOR3 vDir = pPlayer->GetInfo()->vPos - _pMonster->GetInfo()->vPos;
	D3DXVec3Normalize(&vDir, &vDir);
	_pMonster->SetDirectionVector(vDir);
	// 이동
	D3DXVECTOR3 vMoveResult = _pMonster->GetPosVector() + _pMonster->GetDirectionVector();
	_pMonster->SetPosVector(vMoveResult);

	// 상태 유지
	return nullptr;
}

void TrackingState::Render(CMonster* _pMonster, const HDC& _hdc)
{
}

CMonsterState* AttackState::Update(CMonster* _pMonster)
{
	// 공격 종류 (Shoot Type, Crawl Type, Fly Type)
	// Shoot Type

	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	// 만약에 공격범위를 벗어났다면 다시 IdleState로
	if (_pMonster->IsInAttackRangePlayer() == false)
		return new IdleState;
	
	// 공격범위에 있다면 Shoot
	m_fCoolTime = 0.2f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= m_fCoolTime)
	{
		m_fStackTime = 0.f;
		_pMonster->Shoot();
	}

	// 상태 유지
	return nullptr;
}

void AttackState::Render(CMonster* _pMonster, const HDC& _hdc)
{
}
