#pragma once
#include "CScene.h"
class CGame :
    public CScene
{
public:
    CGame();
    virtual ~CGame();

public:
    virtual bool Ready(void)             override;
    virtual void Update(void)            override;
    virtual void LateUpdate(void)        override;
    virtual void Render(const HDC& _hdc) override;
    virtual void Release(void)           override;
};

