#pragma once
#include "CParticle.h"
class CFireParticle :
    public CParticle
{
public:
    explicit CFireParticle(float _fX, float _fY);
    virtual ~CFireParticle() = default;

public:
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC & _hdc) override;

private:
    int   m_iAlpha = 0;
    float m_fDeltaY = 0.f;
    float m_fDeltaX = 0.f;
};

