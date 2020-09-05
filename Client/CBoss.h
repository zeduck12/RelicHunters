#pragma once
#include "CMonster.h"

class CBossState;
class CImageSetting;
class CBoss :
    public CMonster
{
public:
    explicit CBoss() = default;
	CBoss(float _fX, float _fY, float _fWidth, float _fHeight, float _fSpeed = cfMosterDefaultSpeed,
		float _fHp = cfMosterDefaultHp, IMAGE::ID _eID = IMAGE::BOSS);
	virtual ~CBoss();

public:
	void Ready(void) ;
	int  Update(float _fDeltaTime = 0.f);
	void LateUpdate(void);
	void Render(const HDC& _hdc);
	void Release(void);

public:
	bool IsCrack(void) const { return m_bIsCrack; }
	bool IsPhase2(void) const { return m_bIsPhase2; }
	void SetIsPhase2(bool _bIsPhase2) { m_bIsPhase2 = _bIsPhase2; }
	void SetIsCrack(bool _bIsCrack) { m_bIsCrack = _bIsCrack; }
	void SetState(CBossState* _pState) { m_pBossNextState = _pState; }
	void ShootRocket(void);
	void ShootShotgun(void);

public:
	void FullRangeAttack(void);
	bool IsDetectPlayerBossVersion(void);
	bool IsInAttackRangePlayerBossVersion(void);
	void ShowBossSpectrum(void);

private:
	bool m_bIsCrack;
	bool m_bIsPhase2;
	CBossState* m_pBossNextState ; 


};

