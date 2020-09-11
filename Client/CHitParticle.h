#pragma once
#include "CParticle.h"
class CHitParticle :
    public CParticle
{
public:
    explicit CHitParticle() = default;
    CHitParticle(float _fX, float _fY);
    virtual ~CHitParticle() = default;

public:
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;

public:
    const D3DXVECTOR3& GetShadowPos(void) const { return m_vShadowPos; }

private:
    void MoveParticle(void);
    void ActiveGravity(void);
    void ActiveJump(void);
    void ActiveMiniJump(void);
    void ActiveMiniGravity(void);
private:
    bool  m_bIsJump = false;
    float m_fScale = 0.f;
    float m_fGravity = 0.f;
    float m_fMiniGravity = 0.f;
    D3DXVECTOR3 m_vShadowPos;


    float m_fJumpPower;
    float m_fJumpPowerOrigin;
    //미니 중력, 미니 점프
    float m_fMiniJumpPower;
    float m_fMiniJumpPowerOrigin;
    float m_fRotDegree;

    
};

