#pragma once

// 순수 가상함수
class CMonster;
class CMonsterState 
{
public:
	CMonsterState() = default;
	virtual ~CMonsterState() {}

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
	IdleState() = default;
	virtual ~IdleState() {}

public:
	// CMonsterState을(를) 통해 상속됨
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster)				  override;
};

class TrackingState : public CMonsterState
{
public:
	TrackingState() = default;
	virtual ~TrackingState() {}

public:
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster)				  override;
};

class AttackState : public CMonsterState
{
public:
	AttackState() = default;
	virtual ~AttackState() {};

public:
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster)				  override;

};

class AttackedState : public CMonsterState
{
public:
	AttackedState() = default;
	virtual ~AttackedState() {}

public:
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster)				  override;

};

class FlyState : public CMonsterState
{
public:
	FlyState() = default;
	virtual ~FlyState() {}

public:
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster)				  override;
};

class PatrolState : public CMonsterState
{
public:
	PatrolState() = default;
	virtual ~PatrolState() {}

public:
	virtual CMonsterState* Update(CMonster * _pMonster)		  override;
	virtual void Render(CMonster * _pMonster)				  override;
};

class LandingState : public CMonsterState
{
public:
	LandingState() = default;
	virtual ~LandingState() {}

public:
	virtual CMonsterState* Update(CMonster* _pMonster)		  override;
	virtual void Render(CMonster* _pMonster)				  override;
};

class DeathState : public CMonsterState
{
public:
	DeathState() = default;
	virtual ~DeathState() {}

public:
	virtual CMonsterState* Update(CMonster * _pMonster)		  override;
	virtual void Render(CMonster * _pMonster)				  override;

};

class SpawnState : public CMonsterState
{
public:
	SpawnState() = default;
	virtual ~SpawnState() {}

public:
	virtual CMonsterState* Update(CMonster * _pMonster)		  override;
	virtual void Render(CMonster * _pMonster)				  override;
};