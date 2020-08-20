#include "pch.h"
#include "CBullet.h"
#include "CCollisionManager.h"
#include "CObjManager.h"
#include "CPlayerManager.h"
#include "CCameraManager.h"

CBullet::CBullet(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed, float _fShootingDegree, OBJ::ID _eID)
	:
	m_eObjID{ _eID }
{
	m_fDegree = _fShootingDegree;
	m_fSpeed = _fSpeed;

	m_tInfo.vPos = { _fX , _fY , 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { cfDefaultBulletSizeX, cfDefaultBulletSizeY, 0.f };

	// 회전할 버텍스 좌표
	m_vRotVertex[0].x = -(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[0].y = -(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[1].x = +(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[1].y = -(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[2].x = +(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[2].y = +(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[3].x = -(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[3].y = +(m_tInfo.vSize.y * 0.5f);

	m_vDir = _vDir;

	float fAddAngle = 0.f;
	if (GET_SINGLE(CCameraManager)->IsPressing() == false)
		fAddAngle = GetNumberMinBetweenMax(-7.f,7.f);

	D3DXMATRIX matWorld, matRotZ;
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(fAddAngle));
	matWorld = matRotZ;
	D3DXVec3TransformCoord(&m_vDir, &m_vDir, &matWorld);

}

CBullet::~CBullet()
{
	Release();
}

void CBullet::Ready()
{

}

int CBullet::Update(float _fDeltaTime)
{
	D3DXMATRIX matWorld, matRev, matParent;

	D3DXMatrixRotationZ(&matRev, D3DXToRadian(m_fDegree));
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	matWorld = matRev * matParent;

	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&m_vRealVertex[i], &m_vRotVertex[i], &matWorld);

	m_tInfo.vPos += m_vDir * m_fSpeed;

	return 0;
}

void CBullet::LateUpdate()
{
	if (m_eObjID == OBJ::PLAYER)
	{
		for (auto& pMonster : GET_SINGLE(CObjManager)->GetMonsters())
			CCollisionManager::CollideBullet(pMonster.get(), this);
	}
	
	if (m_eObjID == OBJ::MONSTER)
	{
		CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
		CCollisionManager::CollideBullet(pPlayer, this);
	}


}

void CBullet::Render(const HDC& _hdc)
{
	MoveToEx(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y, nullptr);

	for (int i = 1; i < 4; i++)
		LineTo(_hdc, (int)m_vRealVertex[i].x, (int)m_vRealVertex[i].y);
	LineTo(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y);
}

void CBullet::Release()
{
}
