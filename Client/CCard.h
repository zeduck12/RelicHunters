#pragma once
#include "CObj.h"
class CCard :
    public CObj
{
public:
    CCard() = default;
    CCard(float _fX, float _fY, CARD::ID _eID);
    virtual ~CCard();

public:
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;
    void Render_Card(void);

public:
    void SetIsActive(bool _bIsActive) { m_bIsActive = _bIsActive; }
    bool IsActive(void) const { return m_bIsActive; }
    void SetIsClicked(bool _bIsClicked) { m_bIsClicked = _bIsClicked; }
    bool IsClicked(void) const { return m_bIsClicked; }

public:
    const CARD::ID& GetCardID(void) const { return m_eID; }

private:
    int  m_iDrawID = 0;
    bool m_bIsClicked = false;
    bool m_bIsActive  = false;
    bool m_bIsStart   = false;

    float m_fStackTime = 0.f;
    float m_fStartTime = 0.f;

    CARD::ID m_eID = CARD::END;
};

