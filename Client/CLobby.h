#pragma once
#include "CObj.h"
#include "CScene.h"

class CLobby :
    public CScene
{
public:
    CLobby();
    virtual ~CLobby();

public:
    // CScene을(를) 통해 상속됨
    virtual bool Ready(void) override;
    virtual void Update(void) override;
    virtual void LateUpdate(void) override;
    virtual void Render(const HDC& _hdc) override;
    virtual void Release(void) override;

public:
    void DrawMainBackGround(void);
    void DrawMainBackGround2(void);
    void DrawLogoBox(void);
    void DrawLogo(void);
    void DrawZueira(void);

private:
    int   m_iLogoBoxID;
    int   m_iZueiraID;
    float m_fStackTime;
    float m_fStackTimeZ;

    list<shared_ptr<CObj>> m_listButtons;
    list<shared_ptr<CObj>> m_listStars;
};

