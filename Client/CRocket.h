#pragma once
#include "CBullet.h"
class CRocket :
    public CBullet
{
public:
	explicit CRocket() = default;
	CRocket(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed = cfDefaultBulletSpeed, float _fShootingDegree = 0.f, OBJ::ID _eID = OBJ::PLAYER);
	virtual ~CRocket();

public:
	void Ready(void);
	int  Update(float _fDeltaTime = 0.f);
	void LateUpdate(void);
	void Render(const HDC& _hdc);
	void Release(void);

private:
	float m_fStackTime;

};

