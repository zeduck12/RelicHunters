#pragma once
#include "CBoss.h"

class CBossState
{
public:
	CBossState() = default;
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
	EggIdleState() = default;
	virtual ~EggIdleState() {}

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss* _pBoss) override;
	virtual void Render(CBoss* _pBoss) override;

private:
	bool m_bIsInstall = false;
};

class EggStartState : public CBossState
{
public:
	EggStartState() = default;
	virtual ~EggStartState() {}

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss * _pBoss) override;
	virtual void Render(CBoss * _pBoss) override;
};

class EggCrackState : public CBossState
{
public:
	EggCrackState() = default;
	virtual ~EggCrackState() {}

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss * _pBoss) override;
	virtual void Render(CBoss * _pBoss) override;
};

class BossIdleState : public CBossState
{
public:
	BossIdleState() = default;
	virtual ~BossIdleState() {}

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss * _pBoss) override;
	virtual void Render(CBoss * _pBoss) override;
};

class BossMoveState : public CBossState
{
public:
	BossMoveState() = default;
	virtual ~BossMoveState() {}

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss* _pBoss) override;
	virtual void Render(CBoss* _pBoss) override;
};

class BossAttackedState : public CBossState
{
public:
	BossAttackedState() = default;
	virtual ~BossAttackedState() {}

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss* _pBoss) override;
	virtual void Render(CBoss* _pBoss) override;
};

class BossDeathState : public CBossState
{
public:
	BossDeathState() = default;
	virtual ~BossDeathState() {}

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss* _pBoss) override;
	virtual void Render(CBoss* _pBoss) override;
};

class BossAttackState : public CBossState
{
public:
	BossAttackState() = default;
	virtual ~BossAttackState() {}

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss* _pBoss) override;
	virtual void Render(CBoss* _pBoss) override;

private:
	int m_iCount = 0;
};

class BossRandomAttackState : public CBossState
{
public:
	BossRandomAttackState() = default;
	virtual ~BossRandomAttackState() {}

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
	BossRocketAttackState() = default;
	virtual ~BossRocketAttackState() {}

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
	BossShotGunState() = default;
	virtual ~BossShotGunState() {}

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss* _pBoss) override;
	virtual void Render(CBoss* _pBoss) override;

private:
	int m_iCount = 0;
};

class BossRapidAttack : public CBossState
{
public:
	BossRapidAttack() = default;
	virtual ~BossRapidAttack() {}

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss * _pBoss) override;
	virtual void Render(CBoss * _pBoss) override;

private:
	int m_iCount = 0;
};

class BossDashAttack : public CBossState
{
public:
	BossDashAttack() = default;
	virtual ~BossDashAttack() {}

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss * _pBoss) override;
	virtual void Render(CBoss * _pBoss) override;

private:
	int m_iCount = 0;
	bool m_bIsPlayingSFX = false;
};

class BossFullRangeAttack : public CBossState
{
public:
	BossFullRangeAttack() = default;
	virtual ~BossFullRangeAttack() {}

public:
	// CBossState을(를) 통해 상속됨
	virtual CBossState* Update(CBoss * _pBoss) override;
	virtual void Render(CBoss * _pBoss) override;

private:
	int m_iCount = 0;
};