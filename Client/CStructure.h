#pragma once
#include "CObj.h"
class CStructure :
    public CObj
{
public:
    explicit CStructure() = default;
    CStructure(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vSize, D3DXVECTOR3 _vImageSize, int _iDrawID);
    virtual ~CStructure();

public:
    // CObj을(를) 통해 상속됨
    virtual void Ready(void)                    override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void)               override;
    virtual void Release(void)                  override;
    virtual void Render(const HDC& _hdc) {}

public:
    void Render();

public:
    int GetMaxDrawID(void) const { return m_iMaxDrawID; }
    int GetCurDrawID(void) const { return m_iCurDrawID; }
    int GetDrawID(void) const { return m_iDrawID; }
    int GetCurHp(void) const { return m_iCurHp; }
    int GetMaxHp(void) const { return m_iMaxHp; }

public:

    void SetCurDrawID(int _iDrawID) { m_iCurDrawID = _iDrawID; }
    void SetDrawID(int _iDrawID) { m_iDrawID = _iDrawID; }
    void SetCurHp(int _iHp) { m_iCurHp = _iHp; }
    void SetMaxHp(int _iMaxHp) { m_iMaxHp = _iMaxHp; }

private:

    int m_iDrawID;
    int m_iCurDrawID;
    int m_iMaxDrawID;
    int m_iCurHp;
    int m_iMaxHp;

    wstring m_strStateKey;
};

