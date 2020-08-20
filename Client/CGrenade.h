#pragma once
#include "CObj.h"
class CGrenade :
    public CObj
{
public:
    CGrenade() = default;
    CGrenade(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed = cfGrenadeSpeed, float _fShootingDegree = 0.f, float _fShootingDist = 0.f);

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
    
public:

    void BombGrenade(void);
    void ActiveGravity(void);
    void ShootGrenade(void);

    void ActiveMiniGravity(void);
    void MiniJump(void);

    // �ݻ簢���� ������ �Լ�.
    void Reflection(void);


private:

    float m_fBombX;
    float m_fBombY;

    // ����
    D3DXVECTOR3 m_vDir;

    float m_fShootingDist;

    DIRECTION::ID m_eDir;
    // �浹 Ȯ�� ����
    bool m_bIsCollide;
    // �ݻ簢
    float m_fReflectDegree;
    //�⺻ �߷�, ����
    float m_fGravity;
    float m_fJumpPower;
    float m_fJumpPowerOrigin;
    //�̴� �߷�, �̴� ����
    float m_fMiniGravity;
    float m_fMiniJumpPower;
    float m_fMiniJumpPowerOrigin;
};

