#pragma once
#include "CScene.h"
class CEnd :
    public CScene
{
public:
    CEnd();
    virtual ~CEnd();

public:
    virtual bool Ready(void) override;
    virtual void Update(void) override;
    virtual void LateUpdate(void) override;
    virtual void Render(const HDC& _hdc) override;
    virtual void Release(void) override;

public:
    void DrawBackground(void);
    void DrawEndingText(void);

private:
    float m_fDeltaY    = 0.f;
    float m_fStackTime = 0.f;
    
};

