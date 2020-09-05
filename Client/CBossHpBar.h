#pragma once
#include "CObj.h"
#include "CBoss.h"

class CBossHpBar :
    public CObj
{
public:
    explicit CBossHpBar();
    CBossHpBar(CBoss* _pOwner);
    virtual ~CBossHpBar() = default;

public:
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) {};
    
public:
    bool IsFinish(void) const { return m_bIsFinish; }

public:
    void StartDrawHpBar();
    void Render_HpBar(void);

private:
    // º¸½º
    float m_fValue;
    float m_fStackTime;
    bool  m_bIsFinish;
    CBoss* m_pOwner;
};

