#pragma once
#include "CObj.h"


class CImageSetting;
class CItemState;
class CItem :
    public CObj
{
public:
    explicit CItem() = default;
    virtual ~CItem() = default;
public:
    int  GetDrawID(void) const { return m_iDrawID; }
    void SetDrawID(int _iDrawID) { m_iDrawID = _iDrawID; }

protected:
    int m_iDrawID; // 상호작용에 쓰이는 DrawID
    float m_fStackTime = 0.f;
    float m_fCoolTime  = 0.2f;

    CItemState* m_pNextState;
    unique_ptr<CImageSetting> m_pImageSetting;	// 이미지 셋팅 객체

};


class CPickUpLight : public CItem
{
public:
    explicit CPickUpLight() = default;
    CPickUpLight(float _fX, float _fY, float _fWidth, float _fHeight, IMAGE::ID _eID);
    virtual ~CPickUpLight();

public:
    // CItem을(를) 통해 상속됨
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;
};


class CPickUpMedium : public CItem
{
public:
    explicit CPickUpMedium() = default;
    CPickUpMedium(float _fX, float _fY, float _fWidth, float _fHeight, IMAGE::ID _eID);
    virtual ~CPickUpMedium();

public:
    // CItem을(를) 통해 상속됨
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) override;
    virtual void Render(const HDC & _hdc) override;
};


class CPickUpHeavy : public CItem
{
public:
    explicit CPickUpHeavy() = default;
    CPickUpHeavy(float _fX, float _fY, float _fWidth, float _fHeight, IMAGE::ID _eID);
    virtual ~CPickUpHeavy();

public:
    // CItem을(를) 통해 상속됨
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;
};

class CStarCoin : public CItem
{
public:
    explicit CStarCoin() = default;
    CStarCoin(float _fX, float _fY, float _fWidth, float _fHeight, IMAGE::ID _eID, float _fDropDegree = 0.f);
    virtual ~CStarCoin();

public:
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;

public:
    float GetDropDegree(void) const { return m_fDropDegree; }

private:
    float m_fDropDegree = 0.f;
};

//DEFAULT,			    // 플레이어 기본총 - 연발 x
//HIGH_MAG,			    // 부메랑 총
//SHOTGUN,			    // 샷건 
//MACHINEGUN,			// 연속발사 되는 총
//SNIPER,				// 스나이퍼 총 단발 원샷총
//FLAME,				// 몬스터   기본총
//ASSAULT,			    // 3발 연속으로 나가는 총
//KEYTAR,				// 2발 연속으로 나가는 총
//PISTOL_ASSUALT,		// 연속발사 되는 총 (총알만 다름
//PISTOL_HEAVY,		    // 데미지 쌘 연속발사 총
//PLASMA,				// 플라즈마 볼 총

class DefaultGun : public CItem
{
public:
    explicit DefaultGun() = default;
    DefaultGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID);
    virtual ~DefaultGun();

public:
    // CItem을(를) 통해 상속됨
    virtual void Ready(void) {}
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) {}
    virtual void Render(const HDC& _hdc) override;

private:
    GUN::ID m_eGunID;
};

class FlameGun : public CItem
{
public:
    explicit FlameGun() = default;
    FlameGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID);
    virtual ~FlameGun();

public:
    // CItem을(를) 통해 상속됨
    virtual void Ready(void) {}
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) {}
    virtual void Render(const HDC& _hdc) override;

private:
    GUN::ID m_eGunID;
};

class HighMagGun : public CItem
{
public:
    explicit HighMagGun() = default;
    HighMagGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID);
    virtual ~HighMagGun();

public:
    // CItem을(를) 통해 상속됨
    virtual void Ready(void) {}
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) {}
    virtual void Render(const HDC & _hdc) override;

private:
    GUN::ID m_eGunID;
};

class AssaultGun : public CItem
{
public:
    explicit AssaultGun() = default;
    AssaultGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID);
    virtual ~AssaultGun();

public:
    // CItem을(를) 통해 상속됨
    virtual void Ready(void) {}
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) {}
    virtual void Render(const HDC & _hdc) override;

private:
    GUN::ID m_eGunID;
};

class KeytarGun : public CItem
{
public:
    explicit KeytarGun() = default;
    KeytarGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID);
    virtual ~KeytarGun();

public:
    // CItem을(를) 통해 상속됨
    virtual void Ready(void) {}
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) {}
    virtual void Render(const HDC & _hdc) override;

private:
    GUN::ID m_eGunID;
};

class MachineGun : public CItem
{
public:
    explicit MachineGun() = default;
    MachineGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID);
    virtual ~MachineGun();

public:
    // CItem을(를) 통해 상속됨
    virtual void Ready(void) {}
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) {}
    virtual void Render(const HDC & _hdc) override;

private:
    GUN::ID m_eGunID;
};

class PistolAssualtGun : public CItem
{
public:
    explicit PistolAssualtGun() = default;
    PistolAssualtGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID);
    virtual ~PistolAssualtGun();

public:
    // CItem을(를) 통해 상속됨
    virtual void Ready(void) {}
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) {}
    virtual void Render(const HDC & _hdc) override;

private:
    GUN::ID m_eGunID;
};

class PistolHeavyGun : public CItem
{
public:
    explicit PistolHeavyGun() = default;
    PistolHeavyGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID);
    virtual ~PistolHeavyGun();

public:
    // CItem을(를) 통해 상속됨
    virtual void Ready(void) {}
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) {}
    virtual void Render(const HDC & _hdc) override;

private:
    GUN::ID m_eGunID;
};

class PlasmaGun : public CItem
{
public:
    explicit PlasmaGun() = default;
    PlasmaGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID);
    virtual ~PlasmaGun();

public:
    // CItem을(를) 통해 상속됨
    virtual void Ready(void) {}
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) {}
    virtual void Render(const HDC & _hdc) override;

private:
    GUN::ID m_eGunID;
};

class ShotGun : public CItem
{
public:
    explicit ShotGun() = default;
    ShotGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID);
    virtual ~ShotGun();

public:
    // CItem을(를) 통해 상속됨
    virtual void Ready(void) {}
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) {}
    virtual void Render(const HDC & _hdc) override;

private:
    GUN::ID m_eGunID;
};


class SniperGun : public CItem
{
public:
    explicit SniperGun() = default;
    SniperGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID);
    virtual ~SniperGun();

public:
    // CItem을(를) 통해 상속됨
    virtual void Ready(void) {}
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) {}
    virtual void Render(const HDC & _hdc) override;

private:
    GUN::ID m_eGunID;
};