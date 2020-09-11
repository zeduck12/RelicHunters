#include "pch.h"
#include "CHitParticle.h"
#include "CTimeManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CShadow.h"

CHitParticle::CHitParticle(float _fX, float _fY)
{
	m_pOwner = nullptr;
	m_eID = CParticle::SMOKE;

	m_iDrawID = 0;
	m_iDrawMax = 1;
	m_fStackTime = 0.f;
	m_fRotDegree = 0.f;

	m_tInfo.vPos = { _fX, _fY, 0.f };
	m_tInfo.vSize = { 10.f, 10.f, 1.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, 0.f, 0.f };
	m_tInfo.vImageSize = { 0.f, 0.f, 0.f };

	m_fDegree = GetNumberMinBetweenMax(180.f, 360.f);
	m_fSpeed = GetNumberMinBetweenMax(0.f, 3.f);
	m_fScale = GetNumberMinBetweenMax(0.1f, 0.8f);

	m_vShadowPos = { _fX, _fY + 0.1f, 0.f };

	m_fGravity = 0.f;
	m_fJumpPower = 0.f;
	m_fJumpPowerOrigin = 4.f;

	m_fMiniGravity = 0.f;
	m_fMiniJumpPower = 0.f;
	m_fMiniJumpPowerOrigin = 1.5f;
}

void CHitParticle::Ready(void)
{
	m_fJumpPower = m_fJumpPowerOrigin;
	m_fMiniJumpPower = m_fMiniJumpPowerOrigin;
}

int CHitParticle::Update(float _fDeltaTime)
{
	m_fRotDegree += 20.f;
	if (m_vShadowPos.y > m_tInfo.vPos.y)
	{
		MoveParticle();
		ActiveJump();
		ActiveGravity();
	}
	else
	{
		m_fCoolTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fCoolTime <= 0.2f)
		{
			m_fGravity = 0.f;
			m_bIsJump = true;
			ActiveMiniJump();
		}
		else if (m_fCoolTime >= 0.6f)
			this->SetIsValid(false);
	}
		


	return 0;
}

void CHitParticle::LateUpdate(void)
{
}

void CHitParticle::Release(void)
{
}

void CHitParticle::Render(const HDC& _hdc)
{
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"AttackedParticle");
	if (pTexInfo == nullptr)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, m_fScale, m_fScale, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fRotDegree));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 0, 0));
	CShadow::RenderParticle(this);
}

void CHitParticle::MoveParticle(void)
{
	m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree)) * m_fSpeed;
	m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree)) * m_fSpeed;

	if (m_vShadowPos.y - 1.f == m_tInfo.vPos.y)
		return;

	m_vShadowPos.x += cosf(D3DXToRadian(m_fDegree)) * m_fSpeed;
	m_vShadowPos.y += sinf(D3DXToRadian(m_fDegree)) * m_fSpeed;
}

void CHitParticle::ActiveGravity(void)
{
	if (m_vShadowPos.y - 1.f == m_tInfo.vPos.y)
		return;

	m_fGravity += 0.5f;
	m_tInfo.vPos.y += m_fGravity;
}

void CHitParticle::ActiveJump(void)
{
	if(m_bIsJump == false)
		m_tInfo.vPos.y -= 10.f;
	else
		m_tInfo.vPos.y -= 3.f;
}

void CHitParticle::ActiveMiniJump(void)
{
	m_tInfo.vPos.y -= 1.1f;
	m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree)) * m_fSpeed;
	m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree)) * m_fSpeed;

	m_vShadowPos.x += cosf(D3DXToRadian(m_fDegree)) * m_fSpeed;
	m_vShadowPos.y += sinf(D3DXToRadian(m_fDegree)) * m_fSpeed;
}

void CHitParticle::ActiveMiniGravity(void)
{

	if (m_vShadowPos.y < m_tInfo.vPos.y)
		return;

	m_fMiniGravity += 0.7f;
	m_tInfo.vPos.y += m_fMiniGravity;
}
