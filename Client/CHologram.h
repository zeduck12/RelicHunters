#pragma once
#include "CObj.h"
class CHologram :
    public CObj
{
public:
    CHologram(float _fX, float _fY, float _fWidth, float _fHeight, const wstring& _strName = L"Jimmy");
    virtual ~CHologram();

public: 
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;

public:
    int  GetInteractDrawID(void) const { return m_iInteractDrawID; }
    void SetInteractDrawID(int _iDrawID) { m_iInteractDrawID = _iDrawID; }

public:
    void DrawAltar(void);

private:
    int m_iDrawID = 0;
    float m_fStackTime = 0.f;
    float m_fHoloStackTime = 0.f;
    wstring m_strName = L"";
    PLAYER::ID m_ePlayerID = PLAYER::JIMMY;

    // 상호작용 DrawID
    int m_iInteractDrawID = 0;
};

