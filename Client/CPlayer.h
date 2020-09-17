#pragma once
#include "CObj.h"
#include "CWeapon.h"
#include "CShield.h"
#include "CReflectBoard.h"

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
	int  GetSpecialCount(void) const { return m_iSpecialCount; }
	void SetSpecialCount(int _iCount) { m_iSpecialCount = _iCount; }
	bool IsSpecialMode(void) const { return m_bIsSpecialMode; }
	void SetIsSpecialMode(bool _bIsSpecialMode) { m_bIsSpecialMode = _bIsSpecialMode; }
	list<TILE*>& GetCollideWalls(void) { return m_listCollideWalls; }
	CShield* GetShield(void) { return m_pShield.get(); }
	CReflectBoard* GetReflectBoard(void) { return m_pReflectBoard.get(); }
	CWeapon* GetCurWeapon(void) { return m_pWeapon.get(); }
	void  SetCurDashHp(float _fHp) { m_fDashCurHp = _fHp; if (m_fDashCurHp <= 0.f) { m_fDashCurHp = 0.f; } }
	float GetCurDashHp(void) const { return m_fDashCurHp; }
	float GetSaveShieldHp(void) const { return m_fSaveShieldHp; }
	float GetSaveHp(void) const { return m_fSaveHp; }
	void  TakeDamage(float _fHp) 
	{
		if (m_pShield->GetCurShieldHp() <= 0.f)
		{
			// ����
			m_fSaveHp = m_fCurHp;
			//
			m_fCurHp -= _fHp; 
			if (m_fCurHp <= 0.f) 
				m_fCurHp = 0.f; 
		}
		else
		{
			// ����
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

		// �÷��̾� ���� ���� ������Ʈ
		// ù��° ����
		LINEPOS tLeftPoint = { m_vRealVertex[0].x, m_vRealVertex[0].y };
		LINEPOS tRightPoint = { m_vRealVertex[1].x, m_vRealVertex[1].y };
		LINEINFO tInfo = { tLeftPoint, tRightPoint };
		pLineArray[0].tLPoint = tInfo.tLPoint;
		pLineArray[0].tRPoint = tInfo.tRPoint;

		// �ι�° ����
		tLeftPoint = { m_vRealVertex[1].x, m_vRealVertex[1].y };
		tRightPoint = { m_vRealVertex[2].x, m_vRealVertex[2].y };
		tInfo = { tLeftPoint, tRightPoint };
		pLineArray[1].tLPoint = tInfo.tLPoint;
		pLineArray[1].tRPoint = tInfo.tRPoint;

		// ����° ����
		tLeftPoint = { m_vRealVertex[2].x, m_vRealVertex[2].y };
		tRightPoint = { m_vRealVertex[3].x, m_vRealVertex[3].y };
		tInfo = { tLeftPoint, tRightPoint };
		pLineArray[2].tLPoint = tInfo.tLPoint;
		pLineArray[2].tRPoint = tInfo.tRPoint;

		// �׹�° ����
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

	// �ñر�� �Լ���
public:
	void DrawSpecialParticle(void);
	void CheckSpecialMode(void);


private:
	RECT m_rcShadowRect;
	list<TILE*> m_listCollideWalls;

	bool m_bIsReloading;
	bool m_bIsAttacked;
	bool m_bIsAttack = false;

	// ���ݹ޾��� �� ���� Shield HP
	float m_fSaveShieldHp = 0.f;
	float m_fSaveHp = 0.f;

	float m_fCurHp;
	float m_fMaxHp;
	float m_fDashCurHp;
	float m_fDashMaxHp;
	bool  m_bIsDead = false;
	bool  m_bIsInvicible = false;

	float m_fStackTime; // �����ð� ��� �뵵�� ����.
	float m_fShootingDegree;
	float m_fShootingDist;

	D3DXVECTOR3 m_vRotVertex[4];
	D3DXVECTOR3 m_vRealVertex[4];

	// ���ż��� 
	LINEINFO m_tPosin;
	LINEPOS m_tCrossPos;

	// ��� ���ú���
	bool m_bIsDash;
	float m_fAddSpeed;

	wstring m_strDashName;						// ĳ���Ϳ� ���� ����̸�
	unique_ptr<CShield> m_pShield;
	unique_ptr<CWeapon> m_pWeapon;				// ���� �����ϰ� �ִ� ����
	unique_ptr<CReflectBoard> m_pReflectBoard;
	unique_ptr<CImageSetting> m_pImageSetting;	// �̹��� ���� ��ü

private:
	bool m_bIsSpecialMode = false;
	int  m_iSpecialCount = 4; // �ñر� ī��Ʈ ���� - 1 �ؾ���
	int  m_iDrawArmorID = 0;
	float m_fSpecialCheckTime = 0.f;
	float m_fDrawCheckTime = 0.f;
	float m_fDebugCheckTime = 0.f;
	int   m_iDrawDebugID = 0;

private:
	// ���� �÷��̾� ����
	CPlayerState* m_pCurState = nullptr;

};

