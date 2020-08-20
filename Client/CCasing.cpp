#include "pch.h"
#include "CCasing.h"
#include "CPlayerManager.h"
#include "CPlayer.h"

CCasing::CCasing(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed, float _fShootingDegree)
{
    m_tInfo.vPos = { _fX, _fY, 0 };
    m_tInfo.vDir = { 1.f, 0.f, 0.f };
    m_tInfo.vSize = { 5.f, 5.f, 0.f };

    // ���⺤��
    m_vDir = _vDir;
    m_fSpeed = _fSpeed;
    m_fDegree = _fShootingDegree  + 180.f;

    m_fGravity = 0.f;
    m_fJumpPower = 0.f;
    m_fJumpPowerOrigin = 4.f;

    m_eDir = DIRECTION::END;

    m_fMiniGravity = 0.f;
    m_fMiniJumpPower = 0.f;
    m_fMiniJumpPowerOrigin = 1.5f;

    m_fReflectDegree = 0.f;
	m_bIsCollide = false;

	m_fAddValue = GetNumberMinBetweenMax( 1.f, 3.f);
	m_fAddAngle = GetNumberMinBetweenMax(-30.f, 30.f);
	m_fRotDegree = 0.f;

	m_vRotVertex[0].x = - m_tInfo.vSize.x;
	m_vRotVertex[0].y = - m_tInfo.vSize.y;
	m_vRotVertex[1].x = + m_tInfo.vSize.x;
	m_vRotVertex[1].y = + m_tInfo.vSize.y;

}

CCasing::~CCasing()
{
    Release();
}

void CCasing::Ready(void)
{
    m_fJumpPower = m_fJumpPowerOrigin;
    m_fMiniJumpPower = m_fMiniJumpPowerOrigin;
    // �����ɶ� ���� ����.
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    m_eDir = dynamic_cast<CPlayer*>(pPlayer)->GetDirection();
}

int CCasing::Update(float _fDeltaTime )
{
	if (m_bIsCollide == true)
	{
		if (m_fSpeed <= -4.f)
			return 0;

		// �ݻ簢���� ������.
		Reflection();
	}
	else
	{
		if (m_fJumpPower >= -0.2f)
		{
			ActiveGravity();
			ShootCasing();
		}
		else
		{
			if (m_fMiniJumpPower >= -0.2f)
			{
				// �̴� ����
				ActiveMiniGravity();
				MiniJump();
			}
		}
	}

	return 0;
}

void CCasing::LateUpdate(void)
{
	// ���� ���� ȸ���ϸ� ��������.
	if (m_fRotDegree >= 5.f)
		this->SetIsValid(false);

	// ź�� �������� ȸ����Ű��
	m_fRotDegree += 0.1f;
	
	// ź���� ������ �������� ���������� ȸ��
	D3DXMATRIX matWorld, matRev, matParent;
	D3DXMatrixRotationZ(&matRev, m_fRotDegree);
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	matWorld = matRev * matParent;

	for(int i = 0; i < 2; i++)
		D3DXVec3TransformCoord(&m_vRealVertex[i], &m_vRotVertex[i], &matWorld);


}

void CCasing::Release(void)
{
}

void CCasing::Render(const HDC& _hdc)
{

	// ź��
	MoveToEx(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y, nullptr);
	LineTo(_hdc, (int)m_vRealVertex[1].x, (int)m_vRealVertex[1].y);
}

void CCasing::ActiveGravity(void)
{
	m_fGravity += 0.1f;
	// �ϴ� ��, ���� ���
	if (m_eDir == DIRECTION::LEFT || m_eDir == DIRECTION::RIGHT)
		m_tInfo.vPos.y += m_fGravity;

	if (m_eDir == DIRECTION::UP || m_eDir == DIRECTION::DOWN)
		m_tInfo.vPos.x += m_fGravity;
}

void CCasing::ShootCasing(void)
{
	m_fJumpPower -= 0.15f;

	if (m_eDir == DIRECTION::LEFT)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fJumpPower * 2.f + m_fAddValue;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fJumpPower * 2.f;
	}
	else if (m_eDir == DIRECTION::RIGHT)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fJumpPower * 2.f - m_fAddValue;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fJumpPower * 2.f;
	}
	else if (m_eDir == DIRECTION::UP)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fJumpPower * 2.f;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fJumpPower * 2.f - m_fAddValue;
	}
	else if (m_eDir == DIRECTION::DOWN)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fJumpPower * 2.f;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fJumpPower * 2.f + m_fAddValue;
	}
}

void CCasing::ActiveMiniGravity(void)
{
	m_fMiniGravity += 0.1f;
	// �ϴ� ��, ���� ���
	if (m_eDir == DIRECTION::LEFT || m_eDir == DIRECTION::RIGHT)
		m_tInfo.vPos.y += m_fMiniGravity;

	if (m_eDir == DIRECTION::UP || m_eDir == DIRECTION::DOWN)
		m_tInfo.vPos.x += m_fMiniGravity;
}

void CCasing::MiniJump(void)
{
	m_fMiniJumpPower -= 0.1f;

	if (m_eDir == DIRECTION::LEFT)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f + m_fAddValue;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f;
	}
	else if (m_eDir == DIRECTION::RIGHT)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f - m_fAddValue;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f;
	}
	else if (m_eDir == DIRECTION::UP)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f - m_fAddValue;
	}
	else if (m_eDir == DIRECTION::DOWN)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f + m_fAddValue;
	}
}

void CCasing::Reflection(void)
{
	m_fSpeed -= 3.f;

	m_tInfo.vPos.y += sinf(D3DXToRadian(m_fReflectDegree)) * m_fSpeed ;
	m_tInfo.vPos.x += cosf(D3DXToRadian(m_fReflectDegree)) * m_fSpeed ;
}
