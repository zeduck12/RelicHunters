#pragma once
#include "CParticle.h"
class CNumberParticle :
    public CParticle
{
public:
    explicit CNumberParticle() = default;
    CNumberParticle(float _fX, float _fY, float _fDamage = 0.f);
    virtual ~CNumberParticle() = default;

public:
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC & _hdc) override;

private:
    int   m_iRandNum = 0;
    int   m_iAlpha = 0;
    float m_fScale = 0.f;
    float m_fDamage = 0.f;
    float m_fDeltaX = 0.f;

};

