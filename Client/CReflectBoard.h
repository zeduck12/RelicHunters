#pragma once
#include "CObj.h"
class CReflectBoard :
    public CObj
{
public:
    CReflectBoard() = default;
    CReflectBoard(CObj* _pOwner);
    virtual ~CReflectBoard();

public:
    // CObj을(를) 통해 상속됨
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;

private:
    CObj* m_pOwner;
};

