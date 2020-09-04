#pragma once
#include "CObj.h"
class CCasing : // 탄피
    public CObj
{
public:
    CCasing() = default;
    CCasing(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed = cfCasingSpeed, float _fShootingDegree = 0.f);
    virtual ~CCasing();

public:
    // CObj을(를) 통해 상속됨
    virtual void Ready(void)                     override;
    virtual int Update(float _fDeltaTime = 0.f)  override;
    virtual void LateUpdate(void)                override;
    virtual void Release(void)                   override;
    virtual void Render(const HDC& _hdc)         override;

public:
    float GetReflectDegree(void) const { return m_fReflectDegree; }
    void  SetReflectDegree(float _fDegree) { m_fReflectDegree = _fDegree; }
    float GetIncidenceDegree(void) const { return m_fDegree; }
    void  SetIncidenceDegree(float _fDegree) { m_fDegree = _fDegree; }
    bool  GetIsCollide(void) const { return m_bIsCollide; }
    void  SetIsCollide(bool _bIsCollide) { m_bIsCollide = _bIsCollide; }
    const D3DXVECTOR3& GetShadowPos(void) const { return m_vShadowPos; }
    void  SetGravity(float _fValue) { m_fGravity = 0.f; }

public:
    void ActiveGravity(void);
    void ActiveMiniGravity(void);
    void ShootCasing(void);
    void MiniJump(void);


    // 반사각으로 보내는 함수.
    void Reflection(void);

private:
    D3DXVECTOR3 m_vRotVertex[2];
    D3DXVECTOR3 m_vRealVertex[2];
    D3DXVECTOR3 m_vShadowPos;

    // 방향
    D3DXVECTOR3 m_vDir;

    DIRECTION::ID m_eDir;
    // 충돌 확인 변수
    bool m_bIsCollide;
    // 반사각
    float m_fReflectDegree;
    //기본 중력, 점프
    float m_fGravity;
    float m_fJumpPower;
    float m_fJumpPowerOrigin;
    //미니 중력, 미니 점프
    float m_fMiniGravity;
    float m_fMiniJumpPower;
    float m_fMiniJumpPowerOrigin;
    // 추가 변수들
    float m_fAddValue;
    float m_fAddAngle;
    float m_fRotDegree;
    float m_fReflectValue;
};

