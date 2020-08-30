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
    int m_iDrawID; // ��ȣ�ۿ뿡 ���̴� DrawID
    float m_fStackTime = 0.f;
    float m_fCoolTime  = 0.2f;

    CItemState* m_pNextState;
    unique_ptr<CImageSetting> m_pImageSetting;	// �̹��� ���� ��ü

};


class CPickUpLight : public CItem
{
public:
    explicit CPickUpLight() = default;
    CPickUpLight(float _fX, float _fY, float _fWidth, float _fHeight, IMAGE::ID _eID);
    virtual ~CPickUpLight();

public:
    // CItem��(��) ���� ��ӵ�
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
    // CItem��(��) ���� ��ӵ�
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
    // CItem��(��) ���� ��ӵ�
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) {}
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;
};
