#pragma once

// ���� �����Լ�
class CMonster;
class CMonsterState 
{
public:
	explicit CMonsterState() = default;
	virtual ~CMonsterState() = default;

public:
	virtual CMonsterState* Update(CMonster* _pMonster)		  = 0;
	virtual void Render(CMonster* _pMonster, const HDC& _hdc) = 0;

protected:
	float m_fCoolTime  = 0.f;
	float m_fStackTime = 0.f;
};

// Tracking ����
// IDLE ����
// Move ����
// Attack ����

class IdleState : public CMonsterState
{
public:
	explicit IdleState() = default;
	virtual ~IdleState() = default;

public:
	// CMonsterState��(��) ���� ��ӵ�
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster, const HDC& _hdc) override;
};

class TrackingState : public CMonsterState
{
public:
	explicit TrackingState() = default;
	virtual ~TrackingState() = default;

public:
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster, const HDC& _hdc) override;
};

class AttackState : public CMonsterState
{
public:
	explicit AttackState() = default;
	virtual ~AttackState() = default;

public:
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster, const HDC& _hdc) override;

};
