#pragma once
#include "CObj.h"
#include "CWeapon.h"

class CImageSetting;
class CPlayerState;
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
	CWeapon* GetCurWeapon(void) { return m_pWeapon.get(); }
	void  SetCurDashHp(float _fHp) { m_fDashCurHp = _fHp; if (m_fDashCurHp <= 0.f) { m_fDashCurHp = 0.f; } }
	float GetCurDashHp(void) const { return m_fDashCurHp; }
	void  SetCurShieldHp(float _fHp) { m_fShieldCurHp = _fHp; if (m_fShieldCurHp <= 0.f) { m_fShieldCurHp = 0.f; } }
	float GetCurShieldHp(void) const { return m_fShieldCurHp; }
	void  SetCurHp(float _fHp) { m_fCurHp = _fHp; if (m_fCurHp <= 0.f) { m_fCurHp = 0.f; } }
	float GetCurHp(void) const { return m_fCurHp; }
	bool IsReloading(void) const { return m_bIsReloading; }
	void SetIsReloading(bool _bIsReloading) { m_bIsReloading = _bIsReloading; }
	bool IsAttacked(void) const { return m_bIsAttacked; }
	void SetIsAttacked(bool _bIsAttacked) { m_bIsAttacked = _bIsAttacked; }
	float GetShootingDegree(void) const { return m_fShootingDegree; }
	const D3DXVECTOR3& GetDirectionVector(void) const { return (const D3DXVECTOR3&)m_tInfo.vDir; }
	LINEINFO* GetLinesInfo(void)
	{
		//LINEINFO* pLineArray = new LINEINFO[4];
		LINEINFO pLineArray[4] = {};

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
	void SetState(CPlayerState* _pState) { m_pCurState = _pState; }

public:
	void ShootBoomerang(void);

public:
	void Dash(void);
	void RecoverDashHp(void);
	void UpdatePosinInfo(void);
	void TrackMousePos(void);
	void CheckKeyState(void);
	void DetectDirection(void);
	void ShowSpectrum(const HDC& _hdc);

private:
	bool m_bIsReloading;
	bool m_bIsAttacked;

	float m_fCurHp;
	float m_fMaxHp;
	float m_fShieldCurHp;
	float m_fShieldMaxHp;
	float m_fDashCurHp;
	float m_fDashMaxHp;

	float m_fStackTime; // 누적시간 담는 용도의 변수.
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

	unique_ptr<CWeapon> m_pWeapon;				// 현재 보유하고 있는 무기
	unique_ptr<CImageSetting> m_pImageSetting;	// 이미지 셋팅 객체
private:
	// 현재 플레이어 상태
	CPlayerState* m_pCurState = nullptr;

};

