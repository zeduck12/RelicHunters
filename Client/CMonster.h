#pragma once
#include "CObj.h"

class CMonsterState;
class CMonster :
    public CObj
{
public:
    CMonster() = default;
    CMonster(float _fX, float _fY, float _fWidth, float _fHeight, float _fSpeed = cfMosterDefaultSpeed,
	float _fHp = cfMosterDefaultHp);
    virtual ~CMonster();

public:
    // CObj을(를) 통해 상속됨
    virtual void Ready(void)				     override;
    virtual int Update(float _fDeltaTime = 0.f)  override;
    virtual void LateUpdate(void)				 override;
    virtual void Release(void)					 override;
    virtual void Render(const HDC& _hdc)		 override;

public:
    bool IsDied(void) { return m_fCurHp <= 0.f; }
	void TakeDamage(float _fDamage) { m_fCurHp -= _fDamage; }
    bool IsDetectPlayer(void);
	bool IsInAttackRangePlayer(void);
    void CollidePlayer(void);
	void UpdateMonsterDirection(void);

public:
	// 어택관련
	void Shoot(void);

public:

	D3DXVECTOR3 GetDirectionVector(void) const { return m_tInfo.vDir; }
	void SetDirectionVector(D3DXVECTOR3& _rVecDir) { m_tInfo.vDir = _rVecDir; }
    float GetHp(void) const { return m_fCurHp; }
    float GetMaxHp(void) const { return m_fMaxHp; }
	LINEINFO* GetLinesInfo(void)
	{
		LINEINFO* pLineArray = new LINEINFO[3];
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
		tRightPoint = { m_vRealVertex[0].x, m_vRealVertex[0].y };
		tInfo = { tLeftPoint, tRightPoint };
		pLineArray[2].tLPoint = tInfo.tLPoint;
		pLineArray[2].tRPoint = tInfo.tRPoint;

		return pLineArray;
	}
	
public:
	void SetState(CMonsterState* _pState);

private:

    float m_fCurHp;
    float m_fMaxHp;

    D3DXVECTOR3 m_vRotVertex[3];
    D3DXVECTOR3 m_vRealVertex[3];

	// State 담는 변수
	CMonsterState* m_pNextState;

};

