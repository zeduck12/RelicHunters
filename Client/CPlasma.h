#pragma once
#include "CBullet.h"
class CPlasma :
    public CBullet
{
public:
	CPlasma() = default;
	CPlasma(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed = cfBoomerangBulletSpeed, 
		float _fShootingDegree = 0.f,OBJ::ID _eID = OBJ::PLAYER, float _fDamage = 10.f);
	virtual ~CPlasma();

public:
	virtual void Ready(void)					 override;
	virtual int  Update(float _fDeltaTime = 0.f) override;
	virtual void LateUpdate(void)				 override;
	virtual void Render(const HDC& _hdc)		 override;
	virtual void Release(void)					 override;

private:
	int m_iDrawID;
	float m_fStackTime;
	float m_fCoolTime;
};

