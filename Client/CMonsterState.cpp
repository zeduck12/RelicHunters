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
	
	// ���࿡ �÷��̾ �����ߴٸ�
	if (_pMonster->IsDetectPlayer() == true)
		return new TrackingState;

	// ���� ����
	return nullptr;
}

void IdleState::Render(CMonster* _pMonster, const HDC& _hdc)
{
	// �ִϸ��̼� ���
}

CMonsterState* TrackingState::Update(CMonster* _pMonster)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	// ���࿡ �÷��̾ ���� ������ ����� �ٽ� Idle ���·�
	if (_pMonster->IsDetectPlayer() == false)
		return new IdleState;

	// ���࿡ ���ݹ��� �ȿ� ���´ٸ� AttackState��
	if (_pMonster->IsInAttackRangePlayer() == true)
		return new AttackState;

	// ���⺤�� ����.
	D3DXVECTOR3 vDir = pPlayer->GetInfo()->vPos - _pMonster->GetInfo()->vPos;
	D3DXVec3Normalize(&vDir, &vDir);
	_pMonster->SetDirectionVector(vDir);
	// �̵�
	D3DXVECTOR3 vMoveResult = _pMonster->GetPosVector() + _pMonster->GetDirectionVector();
	_pMonster->SetPosVector(vMoveResult);

	// ���� ����
	return nullptr;
}

void TrackingState::Render(CMonster* _pMonster, const HDC& _hdc)
{
}

CMonsterState* AttackState::Update(CMonster* _pMonster)
{
	// ���� ���� (Shoot Type, Crawl Type, Fly Type)
	// Shoot Type

	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return nullptr;

	// ���࿡ ���ݹ����� ����ٸ� �ٽ� IdleState��
	if (_pMonster->IsInAttackRangePlayer() == false)
		return new IdleState;
	
	// ���ݹ����� �ִٸ� Shoot
	m_fCoolTime = 0.2f;
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= m_fCoolTime)
	{
		m_fStackTime = 0.f;
		_pMonster->Shoot();
	}

	// ���� ����
	return nullptr;
}

void AttackState::Render(CMonster* _pMonster, const HDC& _hdc)
{
}
