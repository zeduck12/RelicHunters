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
	void SetState(CBossState* _pState) { m_pNextState = _pState; }

private:

	CBossState* m_pNextState ; 


};

