#pragma once
#include "CObj.h"
class CCasing : // ź��
    public CObj
{
public:
    CCasing() = default;
    CCasing(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed = cfCasingSpeed, float _fShootingDegree = 0.f);
    virtual ~CCasing();

public:
    // CObj��(��) ���� ��ӵ�
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


    // �ݻ簢���� ������ �Լ�.
    void Reflection(void);

private:
    D3DXVECTOR3 m_vRotVertex[2];
    D3DXVECTOR3 m_vRealVertex[2];
    D3DXVECTOR3 m_vShadowPos;

    // ����
    D3DXVECTOR3 m_vDir;

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
    // �߰� ������
    float m_fAddValue;
    float m_fAddAngle;
    float m_fRotDegree;
    float m_fReflectValue;
};

