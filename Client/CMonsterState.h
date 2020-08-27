#pragma once

// 순수 가상함수
class CMonster;
class CMonsterState 
{
public:
	explicit CMonsterState() = default;
	virtual ~CMonsterState() = default;

public:
	virtual CMonsterState* Update(CMonster* _pMonster)		  = 0;
	virtual void Render(CMonster* _pMonster)				  = 0;

protected:
	float m_fCoolTime  = 0.f;
	float m_fStackTime = 0.f;
};

class IdleState : public CMonsterState
{
public:
	explicit IdleState() = default;
	virtual ~IdleState() = default;

public:
	// CMonsterState을(를) 통해 상속됨
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster)				  override;
};

class TrackingState : public CMonsterState
{
public:
	explicit TrackingState() = default;
	virtual ~TrackingState() = default;

public:
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster)				  override;
};

class AttackState : public CMonsterState
{
public:
	explicit AttackState() = default;
	virtual ~AttackState() = default;

public:
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster)				  override;

};

class AttackedState : public CMonsterState
{
public:
	explicit AttackedState() = default;
	virtual ~AttackedState() = default;

public:
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster)				  override;

};

class FlyState : public CMonsterState
{
public:
	explicit FlyState() = default;
	virtual ~FlyState() = default;

public:
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster)				  override;
};

class PatrolState : public CMonsterState
{
public:
	explicit PatrolState() = default;
	virtual ~PatrolState() = default;

public:
	virtual CMonsterState* Update(CMonster * _pMonster)		  override;
	virtual void Render(CMonster * _pMonster)				  override;
};

class LandingState : public CMonsterState
{
public:
	explicit LandingState() = default;
	virtual ~LandingState() = default;

public:
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster)				  override;
};
