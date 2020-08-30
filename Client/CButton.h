#pragma once
#include "CObj.h"


class CButton :
    public CObj
{
public:
    explicit CButton() = default;
    CButton(float _fX, float _fY, float _fWidth, float _fHeight, const wstring& _strName, BUTTON::ID _eID = BUTTON::START);
    virtual ~CButton();

public:
    // CObj을(를) 통해 상속됨
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;

private:
    int m_iDrawID;
    bool m_bIsPress; 
    wstring m_strName;
    BUTTON::ID m_eID;
};

