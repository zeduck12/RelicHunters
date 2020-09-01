#pragma once
#include "CObj.h"

class CBullet 
	: public CObj
{
public:
	CBullet() = default;
	CBullet(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed = cfDefaultBulletSpeed,
		float _fShootingDegree = 0.f, OBJ::ID _eID = OBJ::PLAYER, 
		const wstring& _strBulletName = L"Small", float _fDamage = 10.f);
	virtual ~CBullet();
public:
	virtual void Ready(void)					 override;
	virtual int  Update(float _fDeltaTime = 0.f) override;
	virtual void LateUpdate(void)				 override;
	virtual void Render(const HDC& _hdc)		 override;
	virtual void Release(void)					 override;

public:
	float GetDamage(void) const { return m_fDamage; }

protected:
	float m_fDamage;
	// 방향
	D3DXVECTOR3 m_vDir;

	D3DXVECTOR3 m_vRotVertex[4];
	D3DXVECTOR3 m_vRealVertex[4];
	// 총알 사용자 Owner 아이디
	OBJ::ID m_eObjID;
	wstring m_strBulletName;

};

