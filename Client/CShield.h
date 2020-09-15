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
    void CheckExplosionAnimation(void);
public:
    float GetCurShieldHp(void) const { return m_fShieldCurHp; }
    void  SetCurShieldHp(float _fHp)
    { 
        m_fShieldCurHp = _fHp; 
        if (m_fShieldCurHp <= 0.f) 
            m_fShieldCurHp = 0.f; 

        if (m_fShieldCurHp >= m_fShieldMaxHp)
            m_fShieldCurHp = m_fShieldMaxHp;
    }
  

private:
    int   m_iDrawID;
    float m_fStackTime;
    float m_fCheckTime = 0.f;     // 쉴드 터지는 애니매이션 체크용 변수
    float m_fShieldCurHp;
    float m_fShieldMaxHp;

    bool  m_bIsChecking = false;  // 쉴드 애니매이션 체크가 끝났는지 않끝났는지 확인용
    bool  m_bIsExplosion;         // 현재 쉴드가 터졌는지 않터졌는지 확인하는 변수
    bool  m_bIsPlayingSFX = false;

    D3DXVECTOR3 m_vRotVertex[4];
    D3DXVECTOR3 m_vRealVertex[4];
};

