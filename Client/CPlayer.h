#pragma once
#include "CObj.h"
#include "CWeapon.h"

class CPlayer
	: public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	virtual void Ready()						override;
	virtual int Update(float _fDeltaTime = 0.f) override;
	virtual void LateUpdate()					override;
	virtual void Render(const HDC& _hdc)		override;
	virtual void Release()						override;

public:
	float GetShootingDegree(void) const { return m_fShootingDegree; }
	const D3DXVECTOR3& GetDirectionVector(void) const { return (const D3DXVECTOR3&)m_tInfo.vDir; }
	DIRECTION::ID GetDirection(void) const { return m_eDir; }
	void SetDirection(DIRECTION::ID _eDir) { m_eDir = _eDir; }
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
	void ShootBoomerang(void);

public:
	void TrackMousePos(void);
	void CheckKeyState(void);
	void DetectDirection(void);
	void UpdatePosinInfo(void);
	void Dash(void);
	void ShowSpectrum(const HDC& _hdc);

private:
	float m_fStackTime; // 누적시간 담는 용도의 변수.

	// 현재 방향
	DIRECTION::ID m_eDir;
	float m_fShootingDegree;
	float m_fShootingDist;

	D3DXVECTOR3 m_vRotVertex[4];
	D3DXVECTOR3 m_vRealVertex[4];

	// 포신선분 
	LINEINFO m_tPosin;
	LINEPOS m_tCrossPos;

	// 대시 관련변수
	bool m_bIsDash;
	float m_fAddSpeed;

	// 현재 보유하고 있는 무기
	unique_ptr<CWeapon> m_pWeapon;

};

