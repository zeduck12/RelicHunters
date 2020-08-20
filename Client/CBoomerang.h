#pragma once
#include "CBullet.h"
class CBoomerang :
    public CBullet
{
public:
	CBoomerang() = default;
	CBoomerang(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed = cfBoomerangBulletSpeed);
    virtual ~CBoomerang();
public:
	virtual void Ready(void)					 override;
	virtual int  Update(float _fDeltaTime = 0.f) override;
	virtual void LateUpdate(void)				 override;
	virtual void Render(const HDC& _hdc)		 override;
	virtual void Release(void)					 override;

public:
	bool CheckTheReturn(void);

private:
	float m_fSpeedAccel;

	float m_fAddDegree;
	float m_fAccelDegree;

	float m_fRadian;
	D3DXVECTOR3 m_fVertical;
	D3DXVECTOR3 m_fRVertical;
	D3DXVECTOR3 m_fR2Vertical;
	D3DXVECTOR3 m_fR3Vertical;

	// start pos
	D3DXVECTOR3 m_vStartPos;

};

