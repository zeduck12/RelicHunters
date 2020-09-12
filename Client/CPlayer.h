#pragma once
#include "CObj.h"
#include "CWeapon.h"
#include "CShield.h"

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
	list<TILE*>& GetCollideWalls(void) { return m_listCollideWalls; }
	CShield* GetShield(void) { return m_pShield.get(); }
	CWeapon* GetCurWeapon(void) { return m_pWeapon.get(); }
	void  SetCurDashHp(float _fHp) { m_fDashCurHp = _fHp; if (m_fDashCurHp <= 0.f) { m_fDashCurHp = 0.f; } }
	float GetCurDashHp(void) const { return m_fDashCurHp; }
	float GetSaveShieldHp(void) const { return m_fSaveShieldHp; }
	float GetSaveHp(void) const { return m_fSaveHp; }
	void  TakeDamage(float _fHp) 
	{
		if (m_pShield->GetCurShieldHp() <= 0.f)
		{
			// 저장
			m_fSaveHp = m_fCurHp;
			//
			m_fCurHp -= _fHp; 
			if (m_fCurHp <= 0.f) 
				m_fCurHp = 0.f; 
		}
		else
		{
			// 저장
			m_fSaveShieldHp = m_pShield->GetCurShieldHp();
			//
			float fCurHp = m_pShield->GetCurShieldHp();
			m_pShield->SetCurShieldHp(fCurHp - _fHp);
			GET_SINGLE(CSoundManager)->StopSound(CSoundManager::EFFECT);
			GET_SINGLE(CSoundManager)->PlaySound((TCHAR*)L"sfx_shield_hit1.wav", CSoundManager::EFFECT);
		}
	}
	float GetCurHp(void) const { return m_fCurHp; }
	void  SetCurHp(float _fHp) { m_fCurHp = _fHp; if (m_fCurHp >= m_fMaxHp) { m_fCurHp = m_fMaxHp; } }
	bool IsReloading(void) const { return m_bIsReloading; }
	void SetIsReloading(bool _bIsReloading) { m_bIsReloading = _bIsReloading; }
	bool IsAttacked(void) const { return m_bIsAttacked; }
	void SetIsAttacked(bool _bIsAttacked) { m_bIsAttacked = _bIsAttacked; }
	float GetShootingDegree(void) const { return m_fShootingDegree; }
	const D3DXVECTOR3& GetDirectionVector(void) const { return (const D3DXVECTOR3&)m_tInfo.vDir; }
	const LINEINFO& GetPosinPos(void) const { return m_tPosin; }
	RECT GetShadowRect(void) const { return m_rcShadowRect; }
	void SetIsDead(bool _bIsDead) { m_bIsDead = _bIsDead; }
	bool IsDead(void) const { return m_bIsDead; }
	void SetIsInvicible(bool _bIsInvicible) { m_bIsInvicible = _bIsInvicible; }
	bool IsInvicible(void) const { return m_bIsInvicible; }
	void SetShadowRect(RECT _rcRect)
	{
		LONG lWidth = _rcRect.right - _rcRect.left;
		LONG lHeight = _rcRect.bottom - _rcRect.top;

		m_rcShadowRect = { 36, 10, lWidth, lHeight };
	}
	void SetShadowRect2(RECT _rcRect)
	{
		LONG lHeight = _rcRect.bottom - _rcRect.top;

		m_rcShadowRect = { 36, 10, 100, lHeight };
	}
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
	void UpdateDashName(void);
	void CheckDelaySniper(void);

private:
	RECT m_rcShadowRect;
	list<TILE*> m_listCollideWalls;

	bool m_bIsReloading;
	bool m_bIsAttacked;
	bool m_bIsAttack = false;

	// 공격받았을 시 저장 Shield HP
	float m_fSaveShieldHp = 0.f;
	float m_fSaveHp = 0.f;

	float m_fCurHp;
	float m_fMaxHp;
	float m_fDashCurHp;
	float m_fDashMaxHp;
	bool  m_bIsDead = false;
	bool  m_bIsInvicible = false;

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

	wstring m_strDashName;						// 캐릭터에 따른 대시이름
	unique_ptr<CShield> m_pShield;
	unique_ptr<CWeapon> m_pWeapon;				// 현재 보유하고 있는 무기
	unique_ptr<CImageSetting> m_pImageSetting;	// 이미지 셋팅 객체
private:
	// 현재 플레이어 상태
	CPlayerState* m_pCurState = nullptr;

};

