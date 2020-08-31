#pragma once
#include "CObj.h"

class CStatBox;
class CStatBoxButton :
    public CObj
{
public:
    CStatBoxButton() = default;
    CStatBoxButton(CStatBox* _pOwner, float _fX, float _fY, BUTTON::ID _eID = BUTTON::START);
    virtual ~CStatBoxButton() = default;

public:
    // CObj��(��) ���� ��ӵ�
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;

private:
    int  m_iDrawID;
    BUTTON::ID m_eID;

    // ������
    CStatBox* m_pOwner;
};

