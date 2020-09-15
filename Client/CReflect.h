#pragma once
#include "CBullet.h"
class CReflect :
    public CBullet
{
public:
	CReflect() = default;
	CReflect(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed = cfBoomerangBulletSpeed,
		float _fShootingDegree = 0.f, OBJ::ID _eID = OBJ::PLAYER, float _fDamage = 10.f);
	virtual ~CReflect();

public:
	virtual void Ready(void)					 override;
	virtual int  Update(float _fDeltaTime = 0.f) override;
	virtual void LateUpdate(void)				 override;
	virtual void Render(const HDC & _hdc)		 override;
	virtual void Release(void)					 override;

public:
	void Reflect(void);

public:
	void SetIsCollide(bool _bIsCollide) { m_bIsCollide = _bIsCollide; }
	bool IsCollide(void) const { return m_bIsCollide; }

private:
	int   m_iDrawID;
	float m_fStackTime;
	float m_fCoolTime;
	bool  m_bIsCollide = false;
};

