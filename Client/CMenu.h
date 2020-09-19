#pragma once
#include "CScene.h"
class CMenu :
    public CScene
{
public:
    CMenu();
    virtual ~CMenu();

public:
    // CScene을(를) 통해 상속됨
    virtual bool Ready(void) override;
    virtual void Update(void) override;
    virtual void LateUpdate(void) override;
    virtual void Render(const HDC& _hdc) override;
    virtual void Release(void) override;

public:
    list<CObj*>& GetButtons(void) { return m_listButtons; }

public:
    void DrawBackgorund(void);
    void DrawSelBackground(void);
    void DrawSceneTitle(void);
    void DrawOpenSource(void);


private:
    int m_iDrawID;
    int m_iOSDrawID = 0;
    float m_fStackTime;
    float m_fCheckTime = 0.f;

    list<CObj*> m_listButtons;
    list<shared_ptr<CObj>> m_listStars; // 배경 꾸며주는 별들
};

