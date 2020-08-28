#pragma once
#include "CBoss.h"

class CBossState
{
public:
	explicit CBossState() = default;
	virtual ~CBossState() = default;
public:
	virtual CBossState* Update(CBoss * _pBoss)				 = 0;
	virtual void Render(CBoss * _pBoss)						 = 0;

protected:
	void SetBossState(CBoss * _pBoss, CBossState * _pState)
	{
		_pBoss->SetState(_pState);
	}

protected:
	float m_fStackTime = 0.f;
	float m_fCoolTime  = 0.f;

};


class EggIdleState : public CBossState
{
public:
	explicit EggIdleState() = default;
	virtual ~EggIdleState() = default;

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss* _pBoss) override;
	virtual void Render(CBoss* _pBoss) override;
};

class EggStartState : public CBossState
{
public:
	explicit EggStartState() = default;
	virtual ~EggStartState() = default;

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss * _pBoss) override;
	virtual void Render(CBoss * _pBoss) override;
};

class EggCrackState : public CBossState
{
public:
	explicit EggCrackState() = default;
	virtual ~EggCrackState() = default;

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss * _pBoss) override;
	virtual void Render(CBoss * _pBoss) override;
};

class BossIdleState : public CBossState
{
public:
	explicit BossIdleState() = default;
	virtual ~BossIdleState() = default;

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss * _pBoss) override;
	virtual void Render(CBoss * _pBoss) override;
};

class BossMoveState : public CBossState
{
public:
	explicit BossMoveState() = default;
	virtual ~BossMoveState() = default;

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss* _pBoss) override;
	virtual void Render(CBoss* _pBoss) override;
};

class BossAttackedState : public CBossState
{
public:
	explicit BossAttackedState() = default;
	virtual ~BossAttackedState() = default;

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss* _pBoss) override;
	virtual void Render(CBoss* _pBoss) override;
};

class BossDeathState : public CBossState
{
public:
	explicit BossDeathState() = default;
	virtual ~BossDeathState() = default;

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss* _pBoss) override;
	virtual void Render(CBoss* _pBoss) override;
};

class BossAttackState : public CBossState
{
public:
	explicit BossAttackState() = default;
	virtual ~BossAttackState() = default;

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss* _pBoss) override;
	virtual void Render(CBoss* _pBoss) override;

private:
	int m_iCount = 0;
};

class BossRocketAttackState : public CBossState
{
public:
	explicit BossRocketAttackState() = default;
	virtual ~BossRocketAttackState() = default;

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss* _pBoss) override;
	virtual void Render(CBoss* _pBoss) override;

private:
	int m_iCount = 0;
};

class BossShotGunState : public CBossState
{
public:
	explicit BossShotGunState() = default;
	virtual ~BossShotGunState() = default;

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss* _pBoss) override;
	virtual void Render(CBoss* _pBoss) override;

private:
	int m_iCount = 0;
};