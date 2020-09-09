#pragma once
#include "CMonster.h"
class CGenerator :
    public CMonster
{
public:
	CGenerator() = default;
	CGenerator(float _fX, float _fY, float _fWidth, float _fHeight);
	virtual ~CGenerator();

public:
	virtual void Ready(void)				     override;
	virtual int  Update(float _fDeltaTime = 0.f)  override;
	virtual void LateUpdate(void)				 override;
	virtual void Release(void)					 override;
	virtual void Render(const HDC& _hdc)		 override;
};

