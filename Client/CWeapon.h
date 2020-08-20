#pragma once
#include "CObj.h"
class CWeapon :
    public CObj
{
public:
    CWeapon() = default;
    virtual ~CWeapon();

public:
    // CObj��(��) ���� ��ӵ�
    virtual void Ready(void)                     override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void)                override;
    virtual void Release(void)                   override;
    virtual void Render(const HDC& _hdc)         override;

public:
    void SetCurWeaponID(GUN::ID _eID) { m_eCurWeaponID = _eID; }

public:
    void Shoot(void);
    void ShootDefault(void);
    void ShootShotGun(void);
    void ShootBoomerang(void);
  
public:
private:
    D3DXVECTOR3 m_vRotVertex[4];
    D3DXVECTOR3 m_vRealVertex[4];

    GUN::ID m_eCurWeaponID;				 // ���� ������� ���� ID
};

