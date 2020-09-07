#include "pch.h"
#include "CCasing.h"
#include "CPlayerManager.h"
#include "CPlayer.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CShadow.h"
#include "CMapManager.h"
#include "CCollisionManager.h"
#include "CTimeManager.h"

CCasing::CCasing(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed,
	float _fShootingDegree, const wstring& _strName /*= L"CasingLite"*/)
	:
	m_strName{ _strName }
{
	m_vShadowPos = { _fX, _fY, 0 };
    m_tInfo.vPos = { _fX, _fY, 0 };
    m_tInfo.vDir = { 1.f, 0.f, 0.f };
    m_tInfo.vSize = { 5.f, 5.f, 0.f };

    // 방향벡터
    m_vDir = _vDir;
    m_fSpeed = 0.f;
    m_fDegree = _fShootingDegree  + 180.f;
	m_fReflectValue = 3.f;

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
    // 생성될때 방향 설정.
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    m_eDir = dynamic_cast<CPlayer*>(pPlayer)->GetDirection();
}

int CCasing::Update(float _fDeltaTime )
{
	if (m_bIsCollide == true)
	{
		if (m_fSpeed >= 10.f)
		{
			this->SetIsValid(false);
			return 0;
		}

		// 반사각으로 보내기.
		//ActiveGravity();
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
				// 미니 점프
				ActiveMiniGravity();
				MiniJump();
			}
		}
	}

	return 0;
}

void CCasing::LateUpdate(void)
{
	// 일정 각도 회전하면 없어지기.
	if (m_fRotDegree >= 1000.f)
		this->SetIsValid(false);

	// 탄피 꼭지점을 회전시키기
	m_fRotDegree += 20.f;
	
	// 탄피의 중점을 기준으로 꼭지점들이 회전
	D3DXMATRIX matWorld, matRev, matParent;
	D3DXMatrixRotationZ(&matRev, m_fRotDegree);
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	matWorld = matRev * matParent;

	for(int i = 0; i < 2; i++)
		D3DXVec3TransformCoord(&m_vRealVertex[i], &m_vRotVertex[i], &matWorld);

	for (auto& pTile : GET_SINGLE(CMapManager)->GetWalls())
		CCollisionManager::CollideTileCasing(pTile, this);
}

void CCasing::Release(void)
{
}

void CCasing::Render(const HDC& _hdc)
{
	const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(m_strName);

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	if (this->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fRotDegree));
	D3DXMatrixTranslation(&matTrans, this->GetInfo()->vPos.x, this->GetInfo()->vPos.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CShadow::RenderCasing(this);
}

void CCasing::ActiveGravity(void)
{
	m_fGravity += 0.1f;
	// 일단 좌, 우일 경우
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
		
		m_vShadowPos.x += cosf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fJumpPower * 2.f + m_fAddValue;
		m_vShadowPos.y += sinf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fJumpPower * 2.f;
	}
	else if (m_eDir == DIRECTION::RIGHT)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fJumpPower * 2.f - m_fAddValue;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fJumpPower * 2.f;

		m_vShadowPos.x += cosf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fJumpPower * 2.f - m_fAddValue;
		m_vShadowPos.y += sinf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fJumpPower * 2.f;
	}
	else if (m_eDir == DIRECTION::UP)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fJumpPower * 2.f;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fJumpPower * 2.f - m_fAddValue;

		m_vShadowPos.x += cosf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fJumpPower * 2.f;
		m_vShadowPos.y += sinf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fJumpPower * 2.f - m_fAddValue;
	}
	else if (m_eDir == DIRECTION::DOWN)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fJumpPower * 2.f;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fJumpPower * 2.f + m_fAddValue;

		m_vShadowPos.x += cosf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fJumpPower * 2.f;
		m_vShadowPos.y += sinf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fJumpPower * 2.f + m_fAddValue;
	}
}

void CCasing::ActiveMiniGravity(void)
{
	m_fMiniGravity += 0.1f;
	// 일단 좌, 우일 경우
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

		m_vShadowPos.x += cosf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f + m_fAddValue;
		m_vShadowPos.y += sinf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f;
	}
	else if (m_eDir == DIRECTION::RIGHT)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f - m_fAddValue;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f;

		m_vShadowPos.x += cosf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f - m_fAddValue;
		m_vShadowPos.y += sinf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f;
	}
	else if (m_eDir == DIRECTION::UP)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f - m_fAddValue;

		m_vShadowPos.x += cosf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f;
		m_vShadowPos.y += sinf(D3DXToRadian(m_fDegree - 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f - m_fAddValue;
	}
	else if (m_eDir == DIRECTION::DOWN)
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f;
		m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f + m_fAddValue;

		m_vShadowPos.x += cosf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f;
		m_vShadowPos.y += sinf(D3DXToRadian(m_fDegree + 40.f + m_fAddAngle)) * m_fMiniJumpPower * 2.f + m_fAddValue;
	}
}

void CCasing::Reflection(void)
{
	if (m_fReflectValue <= 0.f)
		return;

	m_fSpeed += 0.1f;

	m_tInfo.vPos.x += cosf(D3DXToRadian(m_fReflectDegree)) * m_fReflectValue * 1.7f;
	m_tInfo.vPos.y += sinf(D3DXToRadian(m_fReflectDegree)) * m_fReflectValue * 0.8f;

	m_vShadowPos.x += cosf(D3DXToRadian(m_fReflectDegree)) * m_fReflectValue * 1.7f;
	m_vShadowPos.y += sinf(D3DXToRadian(m_fReflectDegree)) * m_fReflectValue * 0.8f;

	m_fReflectValue -= 0.1;
	m_tInfo.vPos.y += m_fSpeed;
	//m_vShadowPos.y += m_fSpeed;
}
