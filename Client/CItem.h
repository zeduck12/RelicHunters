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
