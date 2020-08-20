#pragma once
#include "CObj.h"

class CBullet 
	: public CObj
{
public:
	CBullet() = default;
	CBullet(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed = cfDefaultBulletSpeed, float _fShootingDegree = 0.f, OBJ::ID _eID = OBJ::PLAYER);
	virtual ~CBullet();
public:
	virtual void Ready(void)					 override;
	virtual int  Update(float _fDeltaTime = 0.f) override;
	virtual void LateUpdate(void)				 override;
	virtual void Render(const HDC& _hdc)		 override;
	virtual void Release(void)					 override;

protected:
	// ����
	D3DXVECTOR3 m_vDir;

private:
	D3DXVECTOR3 m_vRotVertex[4];
	D3DXVECTOR3 m_vRealVertex[4];
	// �Ѿ� ����� Owner ���̵�
	OBJ::ID m_eObjID;

};
