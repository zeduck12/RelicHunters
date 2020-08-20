#include "pch.h"
#include "CBoomerang.h"
#include "CCollisionManager.h"
#include "CObjManager.h"
#include "CPlayerManager.h"

CBoomerang::CBoomerang(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed)
	:
	m_fSpeedAccel{0.7f}
{
	m_fDegree = 0.f;
	m_fSpeed = _fSpeed;

	// 시작 위치 기억.
	m_vStartPos = { _fX , _fY , 0.f };

	m_tInfo.vPos = { _fX , _fY , 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 10.f, 10.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_vDir = _vDir;
	
	m_fAddDegree = 60.f;
	m_fAccelDegree = 5.f;

	m_fVertical = { m_vDir.y, -m_vDir.x, 0.f };
	m_fVertical *= 50.f;

	m_fRVertical = { m_vDir.y, -m_vDir.x, 0.f };
	m_fRVertical *= 1.f;

	m_fR2Vertical = { m_vDir.y, -m_vDir.x, 0.f };
	m_fR2Vertical *= 20.f;

	m_fR3Vertical = { m_vDir.y, -m_vDir.x, 0.f };
	m_fR3Vertical *= 1.f;
}

CBoomerang::~CBoomerang()
{
	Release();
}

void CBoomerang::Ready(void)
{
}

int CBoomerang::Update(float _fDeltaTime)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	m_fSpeed -= m_fSpeedAccel;
	m_tInfo.vPos += m_vDir * m_fSpeed;

	// 수직인 벡터를 더한다.
	// 수직인 벡터의 크기를 일정량 조금씩 빼준다.
	if (D3DXVec3Length(&m_fVertical) <= 0.1f)
	{
		if (m_fSpeed <= 0.f)
		{
			if (D3DXVec3Length(&m_fR2Vertical) <= 0.1f)
			{

				if (CheckTheReturn() == true)
					this->SetIsValid(false);

				m_fR3Vertical += m_fR3Vertical * 1.f;
				m_tInfo.vPos += m_fR3Vertical;
			}
			else
			{
				m_fR2Vertical -= m_fR2Vertical * 0.2f;
				m_tInfo.vPos -= m_fR2Vertical;
			}

		}
		else
		{
			m_fRVertical += m_fRVertical * 0.1f;
			m_tInfo.vPos -= m_fRVertical;
		}

	}
	else
	{
		m_fVertical -= m_fVertical * 0.5f;
		m_tInfo.vPos += m_fVertical;
	}


	RECT rcPlayer = pPlayer->GetRect();
	RECT rc = {};
	if (IntersectRect(&rc, &this->GetRect(), &rcPlayer) == TRUE)
	{
		// 충돌했다면 false
		this->SetIsValid(false);
	}

	return 0;
}

void CBoomerang::LateUpdate(void)
{
	for (auto& pMonster : CObjManager::Get_Instance()->GetMonsters())
		CCollisionManager::CollideBullet(pMonster.get(), this);
}

void CBoomerang::Render(const HDC& _hdc)
{
	Ellipse(_hdc, GetLeft(), GetTop(), GetRight(), GetBottom());
}

void CBoomerang::Release(void)
{
}

bool CBoomerang::CheckTheReturn(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	if (m_vStartPos.x + 50.f >= m_tInfo.vPos.x &&
		m_vStartPos.x - 50.f <= m_tInfo.vPos.x &&
		m_vStartPos.y - 50.f <= m_tInfo.vPos.y &&
		m_vStartPos.y + 50.f >= m_tInfo.vPos.y)
	{
		return true;
	}
	else
		return false;

}
