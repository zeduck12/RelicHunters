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
	float m_fStackTime = 0.f; // �����ð�
	float m_fCoolTime = 0.f;  // ��Ÿ�� �ð�
};


class PlayerIdleState : public CPlayerState
{
public:
	DECLARE_SINGLETON(PlayerIdleState)

public:
	// CPlayerState��(��) ���� ��ӵ�
	virtual void Update(CPlayer* _pPlayer) override;
	virtual void Render(CPlayer* _pPlayer) override;

};

class PlayerMoveState : public CPlayerState
{
public:
	DECLARE_SINGLETON(PlayerMoveState)

public:
	// CPlayerState��(��) ���� ��ӵ�
	virtual void Update(CPlayer* _pPlayer) override;
	virtual void Render(CPlayer* _pPlayer) override;
};
