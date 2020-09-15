#pragma once
#include "CObj.h"
class CParticle :
    public CObj
{
public:
    enum ID { HIT, DASH, BOMB, DEATH, SMOKE, NUMBER, COUNT, FINISH};
   
public:
     explicit CParticle() = default;
     CParticle(float _fX, float _fY, CParticle::ID _eID, int _iDrawMax = 6, const wstring& _strName = L"Hit", CObj* _pOwner = nullptr);
     virtual ~CParticle() = default;

public:
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;

    void Move(void);
    void MoveToPlayer();

protected:

    int           m_iDrawID;
    int           m_iDrawMax;
    float         m_fStackTime;
    float         m_fCoolTime = 0.f;
    float         m_fAddSpeed = 0.f;
    float         m_fFinishTime = 0.f;

    CParticle::ID m_eID;
    CObj*         m_pOwner;

private:
    wstring       m_strName;
};

