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
	LINEINFO* GetLinesInfo(void)
	{
		LINEINFO* pLineArray = new LINEINFO[4];

		// 플레이어 보유 선분 업데이트
		// 첫번째 선분
		LINEPOS tLeftPoint = { m_vRealVertex[0].x, m_vRealVertex[0].y };
		LINEPOS tRightPoint = { m_vRealVertex[1].x, m_vRealVertex[1].y };
		LINEINFO tInfo = { tLeftPoint, tRightPoint };
		pLineArray[0].tLPoint = tInfo.tLPoint;
		pLineArray[0].tRPoint = tInfo.tRPoint;

		// 두번째 선분
		tLeftPoint = { m_vRealVertex[1].x, m_vRealVertex[1].y };
		tRightPoint = { m_vRealVertex[2].x, m_vRealVertex[2].y };
		tInfo = { tLeftPoint, tRightPoint };
		pLineArray[1].tLPoint = tInfo.tLPoint;
		pLineArray[1].tRPoint = tInfo.tRPoint;

		// 세번째 선분
		tLeftPoint = { m_vRealVertex[2].x, m_vRealVertex[2].y };
		tRightPoint = { m_vRealVertex[3].x, m_vRealVertex[3].y };
		tInfo = { tLeftPoint, tRightPoint };
		pLineArray[2].tLPoint = tInfo.tLPoint;
		pLineArray[2].tRPoint = tInfo.tRPoint;

		// 네번째 선분
		tLeftPoint = { m_vRealVertex[3].x, m_vRealVertex[3].y };
		tRightPoint = { m_vRealVertex[0].x, m_vRealVertex[0].y };
		tInfo = { tLeftPoint, tRightPoint };
		pLineArray[3].tLPoint = tInfo.tLPoint;
		pLineArray[3].tRPoint = tInfo.tRPoint;

		return pLineArray;
	}

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
    D3DXVECTOR3 m_vRealVertex[4];
};

