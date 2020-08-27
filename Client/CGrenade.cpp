#include "pch.h"
#include "CGrenade.h"
#include "CPlayerManager.h"
#include "CPlayer.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CObjManager.h"
#include "CCollisionManager.h"

CGrenade::CGrenade(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed, float _fShootingDegree, float _fShootingDist, bool _bIsReverse /*= false*/)
	:
	m_fBombX{ 0.f },
	m_fBombY{ 0.f },
	m_fRotZAngle{ 0.f },
	m_bIsReverse{ _bIsReverse },
	m_iCollideCount{ 0 }

{
	m_bIsCollide = false;
	m_tInfo.vPos = { _fX , _fY , 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 10.f, 10.f, 0.f };

	m_vDir = _vDir;

	m_fDegree = _fShootingDegree;
	m_fSaveDegree = _fShootingDegree;
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
	if (m_fRotZAngle <= 300.f)
		m_fRotZAngle += 4.6f;

	if (m_bIsCollide == true)
		MiniJump();
	else
		ShootGrenade();

	return 0;
}

void CGrenade::LateUpdate(void)
{
	for (auto& pGrenade : CObjManager::Get_Instance()->GetGrenades())
	{
		DO_IF_IS_NOT_VALID_OBJ(pGrenade)
			continue;

		if (this->m_bIsReverse == false && dynamic_cast<CGrenade*>(pGrenade.get())->IsReverse() == true)
		{
			if (CCollisionManager::CollideGrenade(this, pGrenade.get()) == true)
			{
				m_bIsCollide = true;
				dynamic_cast<CGrenade*>(pGrenade.get())->SetIsCollide(true);
				m_iCollideCount++;
				dynamic_cast<CGrenade*>(pGrenade.get())->SetCollideCount(1);
			}
			
		}
	}

	if(m_iCollideCount >= 3)
		this->SetIsValid(false);
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


	const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Grenade");

	float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	if (this->GetDirection() == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fRotZAngle));
	D3DXMatrixTranslation(&matTrans, this->GetInfo()->vPos.x, this->GetInfo()->vPos.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);

	
	if(m_bIsReverse == true)
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 100, 100, 100));
	else
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CGrenade::BombGrenade(void)
{
	// 폭파 범위
	if (m_fBombX >= 160.f)
		this->SetIsValid(false);

	m_fBombX += 8.f;
	m_fBombY += 4.f;
}


void CGrenade::ShootGrenade(void)
{
	m_fJumpPower -= 0.1f;
	if (m_fShootingDist > 120.f)
		m_fShootingDist = 120.f;
	
	if (m_bIsReverse == false)
	{
		if (m_eDir == DIRECTION::LEFT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 20.f)) * 2.f; 
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 20.f)) * 2.f; 

			m_tInfo.vPos -= D3DXVECTOR3(m_vDir.y, -m_vDir.x, 0.f) * m_fJumpPower  * 2.f;
			m_tInfo.vPos += m_vDir * 3.5f * (m_fShootingDist / 100);
		}
		else if (m_eDir == DIRECTION::RIGHT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 20.f)) * 2.f; 
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 20.f)) * 2.f; 

			m_tInfo.vPos += D3DXVECTOR3(m_vDir.y, -m_vDir.x, 0.f) * m_fJumpPower * 2.f;
			m_tInfo.vPos += m_vDir * 3.5f * (m_fShootingDist / 100);

		}
	
	}
	else 
	{
		if (m_eDir == DIRECTION::LEFT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 20.f)) * 2.f;
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 20.f)) * 2.f;

			m_tInfo.vPos -= D3DXVECTOR3(-m_vDir.y, m_vDir.x, 0.f) * m_fJumpPower * 2.f;
			m_tInfo.vPos += m_vDir * 3.5f * (m_fShootingDist / 100);
		}
		else if (m_eDir == DIRECTION::RIGHT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 20.f)) * 2.f; 
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 20.f)) * 2.f; 

			m_tInfo.vPos += D3DXVECTOR3(-m_vDir.y, m_vDir.x, 0.f) * m_fJumpPower * 2.f;
			m_tInfo.vPos += m_vDir * 3.5f * (m_fShootingDist / 100);
		}

	}
}

void CGrenade::MiniJump(void)
{
	m_fMiniJumpPower -= 0.1f;
	if (m_fShootingDist > 170.f)
		m_fShootingDist = 170.f;

	if (m_bIsReverse == false)
	{
		if (m_eDir == DIRECTION::LEFT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fSaveDegree + 40.f)) * 2.f;
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fSaveDegree + 40.f)) * 2.f;

			m_tInfo.vPos += D3DXVECTOR3(-m_vDir.y, m_vDir.x, 0.f) * m_fMiniJumpPower * 3.f;
			m_tInfo.vPos += m_vDir * 3.5f * (m_fShootingDist / 100);
		}
		else if (m_eDir == DIRECTION::RIGHT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree - 40.f)) * 2.f; 
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree - 40.f)) * 2.f; 

			m_tInfo.vPos += D3DXVECTOR3(m_vDir.y, -m_vDir.x, 0.f) * m_fMiniJumpPower * 3.f;
			m_tInfo.vPos += m_vDir * 3.5f * (m_fShootingDist / 100);
		}
	}
	else
	{
		if (m_eDir == DIRECTION::LEFT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fSaveDegree - 40.f)) * 2.f; 
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fSaveDegree - 40.f)) * 2.f; 
			
			m_tInfo.vPos += D3DXVECTOR3(m_vDir.y, -m_vDir.x, 0.f) * m_fMiniJumpPower * 3.f;
			m_tInfo.vPos += m_vDir * 3.5f * (m_fShootingDist / 100);
		}
		else if (m_eDir == DIRECTION::RIGHT)
		{
			m_tInfo.vPos.x += cosf(D3DXToRadian(m_fDegree + 40.f)) * 2.f; 
			m_tInfo.vPos.y += sinf(D3DXToRadian(m_fDegree + 40.f)) * 2.f; 


			m_tInfo.vPos += D3DXVECTOR3(-m_vDir.y, m_vDir.x, 0.f) * m_fMiniJumpPower * 3.f;
			m_tInfo.vPos += m_vDir * 3.5f * (m_fShootingDist / 100);
		}
	}
}

void CGrenade::Reflection(void)
{
	m_fShootingDist -= 0.1f;
	m_fSpeed -= 3.f;

	m_tInfo.vPos.y += sinf(D3DXToRadian(m_fReflectDegree )) * m_fShootingDist / 100  ;
	m_tInfo.vPos.x += cosf(D3DXToRadian(m_fReflectDegree )) * m_fShootingDist / 100 ;

}



