#pragma once
#include "CObj.h"
#include "CMenu.h"

class CStatBox;
class CSelButton :
    public CObj
{
public:
    explicit CSelButton() = default;
    CSelButton(CMenu& _rMenu ,float _fX, float _fY, float _fWidth, float _fHeight, const wstring & _strName, const wstring& _strClickedName, BUTTON::SEL_ID _eID = BUTTON::JIMMY);
    virtual ~CSelButton();

public:
    // CObj¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;

public:
    BUTTON::SEL_ID GetSelID(void) const { return m_eID; }
    bool IsClicked(void) const { return m_bIsClicked; }
    void SetIsClicked(bool _bIsClicked) { m_bIsClicked = _bIsClicked; }

public:
    void DrawIdle(void);
    void DrawActive(void);
    void DrawClicked(void);
    void DrawName(void);

private:

    int m_iDrawID;
    int m_iDrawClickedID;
    bool m_bIsActive;
    bool m_bIsClicked;
    float m_fStackTime = 0.f;

    wstring m_strName;
    wstring m_strClickedName;
    BUTTON::SEL_ID m_eID;

    CStatBox* m_pStatBox;
    // ∏ﬁ¥∫æ¿
    CMenu& m_rMenuScene;
};

