#pragma once
#include "CMonster.h"
class CKamikazeCage :
    public CMonster
{
public:
	CKamikazeCage() = default;
	CKamikazeCage(float _fX, float _fY, float _fWidth, float _fHeight);
	virtual ~CKamikazeCage();

public:
	// CObj��(��) ���� ��ӵ�
	virtual void Ready(void)				     override;
	virtual int  Update(float _fDeltaTime = 0.f)  override;
	virtual void LateUpdate(void)				 override;
	virtual void Release(void)					 override;
	virtual void Render(const HDC& _hdc)		 override;

public:
	void CreateKamikazes(void);
};

