#pragma once
#include "CObj.h"
class CMouse :
    public CObj
{
public:
    explicit CMouse() = default;
    virtual ~CMouse() = default;

public:
    // CObj��(��) ���� ��ӵ�
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;

public:
    void ToEnlargeScale(void);  // Ȯ��
    void ToReduceScale(void);	// ���

private:
    int m_iDrawID = 0;
    int m_iReloadID = 0;
    float m_fStackTime = 0.f;

    float m_fScale = 1.f;
    bool m_bIsEnlargeScale = false;
    bool m_bIsReduceScale  = false;
    bool m_bIsPressing     = false;
    bool m_bIsReloading    = false;
 
};

