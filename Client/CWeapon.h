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
    bool GetWorldMatrix(D3DXMATRIX* _pOutMatrix);

public:
    void    SetCurWeaponID(GUN::ID _eID) { m_eCurWeaponID = _eID; }
    GUN::ID GetCurWeaponID(void) const { return m_eCurWeaponID; }

public:
    void Shoot(void);
    void ShootDefault(void);
    void ShootShotGun(void);
    void ShootBoomerang(void);
    void ShootSniper(void);
    void ShootFlame(void);
    void ShootAssault(void);
    void ShootPistolAssault(void);
    void ShootPistolHeavy(void);
    void ShootKeytar(void);
    void ShootPlasma(void);

public:
    void DrawCurGun(void);
    bool ReloadBullets(void);
  
public:
private:
    D3DXVECTOR3 m_vRotVertex[4];
    D3DXVECTOR3 m_vRealVertex[4];

    GUN::ID m_eCurWeaponID;				 // ���� ������� ���� ID

    int m_iCurCapacity;     // ���� ���� źâũ��     // 50
    int m_iReloadedBullets; // ���� ������ �Ѿ� ����
};

