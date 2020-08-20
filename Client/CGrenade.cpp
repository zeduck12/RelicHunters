#include "pch.h"
#include "CGrenade.h"
#include "CPlayerManager.h"
#include "CPlayer.h"

CGrenade::CGrenade(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed, float _fShootingDegree, float _fShootingDist)
	:
	m_fBombX{ 0.f },
	m_fBombY{ 0.f }

{
	m_tInfo.vPos = { _fX , _fY , 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 10.f, 10.f, 0.f };

	m_vDir = _vDir;

	m_fDegree = _fShootingDegree;
	m_fSpeed = _fSpeed;
	m_fShootingDist = _fShootingDist;

	m_fGravity = 0.f;
	m_fJumpPower = 0.f;
	m_fJumpPowerOrigin = 4.f; 

	m_eDir = DIRECTION::END;

	m_fMiniGravity = 0.f;
	m_fMiniJumpPower = 0.f;
	m_fMiniJumpPowerOrigin = 1.5f;

	m_fReflectDegree = 0.f;
}

void CGrenade::Ready(void)
{
	m_fJumpPower = m_fJumpPowerOrigin;
	m_fMiniJumpPower = m_fMiniJumpPowerOrigin;
	// 생성될때 방향 설정.
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	m_eDir = dynamic_cast<CPlayer*>(pPlayer)->GetDirection();
}

int CGrenade::Update(float _fDeltaTime)
{
	if (m_bIsCollide == true)
	{
		if (m_fSpeed <= -4.f)
			BombGrenade();
			
		// 반사각으로 보내기.
		Reflection();
	}
	else
	{
		if (m_fJumpPower >= -0.2f)
		{
			ActiveGravity();
			ShootGrenade();
		}
		else
		{
			if (m_fMiniJumpPower >= -0.2f)
			{
				// 미니 점프
				ActiveMiniGravity();
				MiniJump();
			}
			else
				BombGrenade();
		}
	}

	return 0;
}

void CGrenade::LateUpdate(void)
{
}

void CGrenade::Release(void)
{
}

void CGrenade::Render(const HDC& _hdc)
{
	Ellipse(_hdc, GetLeft(), GetTop(), GetRight(), GetBottom());

	// 폭발 범위 그리기
	Rectangle(_hdc, int(m_tInfo.vPos.x - m_fBombX), int(m_tInfo.vPos.y - m_fBombY),
			int(m_tInfo.vPos.x + m_fBombX), int(m_tInfo.vPos.y + m_fBombY));
}

void CGrenade::BombGrenade(void)
{
	// 폭파 범위
	if (m_fBombX >= 160.f)
		this->SetIsValid(false);

	m_fBombX += 8.f;
	m_fBombY += 4.f;
}

void CGrenade::ActiveGravity(void)
{
	m_fGravity += 0.1f;
	// 일단 좌, 우일 경우
	if (m_eDir == DIRECTION::LEFT || m_eDir == DIRECTION::RIGHT)
		m_tInfo.vPos.y += m_fGravity;
	
	if(m_eDir == DIRECTION::UP || m_eDir == DIRECTION::DOWN)
		m_tInfo.vPos.x += m_fGravity;
}

void CGrenade::ShootGrenade(void)
{
	m_fJumpPower -= 0.1f;
	
	if (m_eDir == DIRECTION::LEFT)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 40.f)) * m_fJumpPower * 2.f - (m_fShootingDist / 100);
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 40.f)) * m_fJumpPower * 2.f;
	}
	else if (m_eDir == DIRECTION::RIGHT)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 40.f)) * m_fJumpPower * 2.f + (m_fShootingDist / 100);
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 40.f)) * m_fJumpPower * 2.f;
	}
	else if (m_eDir == DIRECTION::UP) 
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 40.f)) * m_fJumpPower * 2.f;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 40.f)) * m_fJumpPower * 2.f + (m_fShootingDist / 100);
	}
	else if(m_eDir == DIRECTION::DOWN)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 40.f)) * m_fJumpPower * 2.f;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 40.f)) * m_fJumpPower * 2.f - (m_fShootingDist / 100);
	}
}

void CGrenade::ActiveMiniGravity(void)
{
	m_fMiniGravity += 0.1f;
	// 일단 좌, 우일 경우
	if (m_eDir == DIRECTION::LEFT || m_eDir == DIRECTION::RIGHT)
		m_tInfo.vPos.y += m_fMiniGravity;

	if (m_eDir == DIRECTION::UP || m_eDir == DIRECTION::DOWN)
		m_tInfo.vPos.x += m_fMiniGravity;
}

void CGrenade::MiniJump(void)
{
	m_fMiniJumpPower -= 0.1f;

	if (m_eDir == DIRECTION::LEFT)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 40.f)) * m_fMiniJumpPower * 2.f - (m_fShootingDist / 100);
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 40.f)) * m_fMiniJumpPower * 2.f;
	}
	else if (m_eDir == DIRECTION::RIGHT)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 40.f)) * m_fMiniJumpPower * 2.f + (m_fShootingDist / 100);
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 40.f)) * m_fMiniJumpPower * 2.f;
	}
	else if (m_eDir == DIRECTION::UP)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 40.f)) * m_fMiniJumpPower * 2.f;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 40.f)) * m_fMiniJumpPower * 2.f + (m_fShootingDist / 100);
	}
	else if (m_eDir == DIRECTION::DOWN)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 40.f)) * m_fMiniJumpPower * 2.f;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 40.f)) * m_fMiniJumpPower * 2.f - (m_fShootingDist / 100);
	}
}

void CGrenade::Reflection(void)
{
	m_fShootingDist -= 0.1f;
	m_fSpeed -= 3.f;

	m_tInfo.vPos.y += sinf(D3DXToRadian(m_fReflectDegree )) * m_fShootingDist / 100  ;
	m_tInfo.vPos.x += cosf(D3DXToRadian(m_fReflectDegree )) * m_fShootingDist / 100 ;

}



