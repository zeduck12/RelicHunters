#pragma once
#include "CObj.h"

class CImageSetting;
class CMonsterState;
class CMonster :
	public CObj
{
public:
	CMonster() = default;
	CMonster(float _fX, float _fY, float _fWidth, float _fHeight, float _fSpeed = cfMosterDefaultSpeed,
		float _fHp = cfMosterDefaultHp, IMAGE::ID _eID = IMAGE::DUCK);
	virtual ~CMonster();

public:
	// CObj��(��) ���� ��ӵ�
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
	void DetectDirection(void);
	void EquipWeapon(void);

public:
	// ���ð���
	void Shoot(void);
	void Dash(void);
	void ShowSpectrum(void);
	void Fly(void);
	void Patrol(void);
	void Landing(void);

public:
	D3DXVECTOR3 GetDirectionVector(void) const { return m_tInfo.vDir; }
	void SetDirectionVector(D3DXVECTOR3& _rVecDir) { m_tInfo.vDir = _rVecDir; }
	float GetHp(void) const { return m_fCurHp; }
	float GetMaxHp(void) const { return m_fMaxHp; }
	LINEINFO* GetLinesInfo(void)
	{
		LINEINFO* pLineArray = new LINEINFO[3];
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
		tRightPoint = { m_vRealVertex[0].x, m_vRealVertex[0].y };
		tInfo = { tLeftPoint, tRightPoint };
		pLineArray[2].tLPoint = tInfo.tLPoint;
		pLineArray[2].tRPoint = tInfo.tRPoint;

		return pLineArray;
	}
	bool IsDash(void) const { return m_bIsDash; }
	void SetIsDash(bool _bIsDash) { m_bIsDash = _bIsDash; }
	float GetAddSpeed(void) const { return m_fAddSpeed; }
	void SetAddSpeed(float _fAddSpd) { m_fAddSpeed = _fAddSpd; }
	void SetStackTime(float _fTime) { m_fStackTime = _fTime; }
	bool IsFlying(void) const { return m_bIsFlying; }
	void SetIsFlying(bool _bIsFlying) { m_bIsFlying = _bIsFlying; }
	void SetOldPos(D3DXVECTOR3 _vPos) { m_vOldPos = _vPos; }
	// �׸��ڿ�
	D3DXVECTOR3 GetOldPos(void) const { return m_vOldPos; }
	D3DXVECTOR3 GetGap(void) const { return m_vGap; }
	void SetGap(D3DXVECTOR3 _vGap) { m_vGap = _vGap; }

public:
	void SetState(CMonsterState* _pState);

private:

	float m_fStackTime;
	bool  m_bIsFlying;
	D3DXVECTOR3 m_vOldPos;
	D3DXVECTOR3 m_vGap;

	bool  m_bIsDash;
	float m_fAddSpeed;

    float m_fCurHp;
    float m_fMaxHp;

	float m_fShootingDegree ;
	float m_fShootingDist ;

    D3DXVECTOR3 m_vRotVertex[3];
    D3DXVECTOR3 m_vRealVertex[3];

	// State ��� ����
	CMonsterState* m_pNextState;
	unique_ptr<CImageSetting> m_pImageSetting;
};

