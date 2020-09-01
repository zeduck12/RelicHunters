#pragma once
#include "CObj.h"
class CGrenade :
    public CObj
{
public:
    CGrenade() = default;
    CGrenade(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed = cfGrenadeSpeed, float _fShootingDegree = 0.f, float _fShootingDist = 0.f, bool _bIsReverse = false);

public:
    virtual void Ready(void)                    override;
    virtual int Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void)               override;
    virtual void Release(void)                  override;
    virtual void Render(const HDC& _hdc)        override;

public:
    float GetReflectDegree(void) const { return m_fReflectDegree; }
    void  SetReflectDegree(float _fDegree) { m_fReflectDegree = _fDegree; }
    float GetIncidenceDegree(void) const { return m_fDegree; }
    void  SetIncidenceDegree(float _fDegree) { m_fDegree = _fDegree; }
    bool  GetIsCollide(void) const { return m_bIsCollide; }
    void  SetIsCollide(bool _bIsCollide) { m_bIsCollide = _bIsCollide; }
    bool IsReverse(void) const { return m_bIsReverse; }
    void SetCollideCount(int _iCount) { m_iCollideCount += _iCount; }
    
public:
    void TakeDamageToObejcts(void);
    void DrawBombParticle(void);
    void ShootGrenade(void);
    void MiniJump(void);

    // 반사각으로 보내는 함수.
    void Reflection(void);

private:
    int m_iDrawID;
    float m_fStackTime;
    float m_fBombStackTime;

    int m_iCollideCount;
    bool m_bIsReverse;
    float m_fRotZAngle;
    float m_fSaveDegree;

    float m_fBombX;
    float m_fBombY;

    // 방향
    D3DXVECTOR3 m_vDir;

    float m_fShootingDist;

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
};

