#pragma once
#include "CScene.h"
class CLogo :
    public CScene
{
public:
    CLogo();
    virtual ~CLogo();

public:
    // CScene을(를) 통해 상속됨
    virtual bool Ready(void) override;
    virtual void Update(void) override;
    virtual void LateUpdate(void) override;
    virtual void Render(const HDC& _hdc) override;
    virtual void Release(void) override;

private:
    float m_fStackTime;
    bool  m_bIsPlayingBGM = false;

};

