#pragma once
#include "CParticle.h"
class CSmoke :
    public CParticle
{
public:
    explicit CSmoke() = default;
    CSmoke(float _fX, float _fY, CObj * _pOwner = nullptr);
    virtual ~CSmoke() = default;

public:
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;

private:
    int m_iDelta = 0;
    float m_fScale = 0.f;
};

