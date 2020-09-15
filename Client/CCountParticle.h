#pragma once
#include "CParticle.h"
class CCountParticle :
    public CParticle
{
public:
    explicit CCountParticle();
    virtual ~CCountParticle() = default;

public:
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;

private:
    int   m_iCount = 0;
    int   m_iAlpha = 0;
    float m_fDeltaY = 0.f;
};

