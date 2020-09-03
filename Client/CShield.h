#pragma once
#include "CObj.h"
class CShield :
    public CObj
{
public:
    explicit CShield() = default;
    virtual ~CShield() = default;

public:
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;

public:
    void DrawShieldActive(void);
    void DrawShieldExplosion(void);

public:
    void RecoverShield(void);
    void UpdateShieldPos(void);
public:
    float GetCurShieldHp(void) const { return m_fShieldCurHp; }
    void  SetCurShieldHp(float _fHp) { m_fShieldCurHp = _fHp; if (m_fShieldCurHp <= 0.f) { m_fShieldCurHp = 0.f; } }

private:
    int m_iDrawID;
    float m_fStackTime;

    float m_fShieldCurHp;
    float m_fShieldMaxHp;

    D3DXVECTOR3 m_vRotVertex[4];
    D3DXVECTOR3 m_vRealVertex[4];
};

