#include "pch.h"
#include "CShotGun.h"
#include "CObjManager.h"
#include "CPlayerManager.h"
#include "CCollisionManager.h"

CShotGun::CShotGun(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fAddDegree, float _fSpeed, float _fShootingDegree)
	:
	m_fAddDegree{ _fAddDegree }
{
	m_fDegree = _fShootingDegree;
	m_fSpeed = _fSpeed;

	m_tInfo.vPos = { _fX , _fY , 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 20.f, 10.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_vDir = _vDir;

	// 회전할 버텍스 좌표
	m_vRotVertex[0].x = -(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[0].y = -(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[1].x = +(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[1].y = -(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[2].x = +(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[2].y = +(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[3].x = -(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[3].y = +(m_tInfo.vSize.y * 0.5f);
}

CShotGun::~CShotGun()
{
	Release();
}

int CShotGun::Update(float _fDeltaTime)
{
	// 현재 방향벡터를 기준으로 3발 발사하기.
	// 총알이 발사될 각도 구함.
	D3DXMATRIX matWorld, matRev, matParent;

	D3DXMatrixRotationZ(&matRev, D3DXToRadian(m_fDegree + m_fAddDegree));
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	matWorld = matRev * matParent;

	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&m_vRealVertex[i], &m_vRotVertex[i], &matWorld);

	if (m_fAddDegree != 0.f)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + m_fAddDegree)) * m_fSpeed;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + m_fAddDegree)) * m_fSpeed;
	}
	else
		m_tInfo.vPos += m_vDir * m_fSpeed;

	return 0;
}

void CShotGun::LateUpdate(void)
{
	for (auto& pMonster : CObjManager::Get_Instance()->GetMonsters())
		CCollisionManager::CollideBullet(pMonster.get(), this);
}

void CShotGun::Render(const HDC& _hdc)
{
	MoveToEx(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y, nullptr);

	for (int i = 1; i < 4; i++)
		LineTo(_hdc, (int)m_vRealVertex[i].x, (int)m_vRealVertex[i].y);
	LineTo(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y);
}
