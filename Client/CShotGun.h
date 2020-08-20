#pragma once
#include "CBullet.h"
class CShotGun :
    public CBullet
{
public:
	CShotGun() = default;
	CShotGun(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fAddDegree = 0.f , float _fSpeed = cfDefaultBulletSpeed, float _fShootingDegree = 0.f);
	virtual ~CShotGun();
public:
	virtual void Ready(void) { }
	virtual int  Update(float _fDeltaTime = 0.f) override;
	virtual void LateUpdate(void)				 override;
	virtual void Render(const HDC& _hdc)		 override;
	virtual void Release(void) { }

private:
	float m_fAddDegree;

	D3DXVECTOR3 m_vRotVertex[4];
	D3DXVECTOR3 m_vRealVertex[4];

};


