#pragma once
#include "CPlayer.h"

class CPlayerState
{
public:
	virtual void Update(CPlayer* _pPlayer) = 0;
	virtual void Render(CPlayer* _pPlayer) = 0;

protected:
	void SetPlayerState(CPlayer* _pPlayer, CPlayerState* _pState) { _pPlayer->SetState(_pState); }

protected:
	float m_fStackTime = 0.f; // 누적시간
	float m_fCoolTime = 0.f;  // 쿨타임 시간
};


class PlayerIdleState : public CPlayerState
{
public:
	DECLARE_SINGLETON(PlayerIdleState)

public:
	// CPlayerState을(를) 통해 상속됨
	virtual void Update(CPlayer* _pPlayer) override;
	virtual void Render(CPlayer* _pPlayer) override;

};

class PlayerMoveState : public CPlayerState
{
public:
	DECLARE_SINGLETON(PlayerMoveState)

public:
	// CPlayerState을(를) 통해 상속됨
	virtual void Update(CPlayer* _pPlayer) override;
	virtual void Render(CPlayer* _pPlayer) override;
};
