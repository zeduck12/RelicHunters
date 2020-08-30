#pragma once
#include "CObj.h"
class CDecoStar :
    public CObj
{
public:
    CDecoStar() = default;
    CDecoStar(float _fX, float _fY, float _fWidth, float _fHeight, float _fGapX = 0.f, float _fGapY = 0.f);
    virtual ~CDecoStar();

public:
    // CObj을(를) 통해 상속됨
    virtual void Ready(void) override;
    virtual int Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;

private:
    float m_fSpawnX;
    float m_fSpawnY;
    float m_fGapX;
    float m_fGapY;
};

