#include "pch.h"
#include "CRocket.h"
#include "CCameraManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CPlayerManager.h"
#include "CPlayer.h"
#include "CCollisionManager.h"
#include "CPlayerState.h"
#include "CTimeManager.h"


CRocket::CRocket(float _fX, float _fY, D3DXVECTOR3 _vDir, float _fSpeed, float _fShootingDegree, OBJ::ID _eID)
{
	m_fStackTime = 0.f;
	m_fDegree = _fShootingDegree;
	m_fSpeed = _fSpeed;

	m_tInfo.vPos = { _fX , _fY , 0.f };
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { cfDefaultBulletSizeX, cfDefaultBulletSizeY, 0.f };

	// ȸ���� ���ؽ� ��ǥ
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
		fAddAngle = GetNumberMinBetweenMax(-7.f, 7.f);

	D3DXMATRIX matWorld, matRotZ;
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(fAddAngle));
	matWorld = matRotZ;
	D3DXVec3TransformCoord(&m_vDir, &m_vDir, &matWorld);
}

CRocket::~CRocket()
{
    Release();
}

void CRocket::Ready(void)
{
}

int CRocket::Update(float _fDeltaTime)
{

	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return 0;

	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime <= 2.f)
	{
		float fDeltaX = pPlayer->GetX() - this->GetX();
		float fDeltaY = pPlayer->GetY() - this->GetY();
		float fDist = sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY);

		float fRadian = acosf(fDeltaX / fDist);
		if (pPlayer->GetY() < m_tInfo.vPos.y)
			fRadian = 2 * D3DX_PI - fRadian;
		m_fDegree = fRadian;

		D3DXVECTOR3 vPlayerPos = pPlayer->GetInfo()->vPos;
		D3DXVECTOR3 vBulletPos = this->GetInfo()->vPos;

		m_tInfo.vDir = vPlayerPos - vBulletPos;
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	}

	D3DXMATRIX matWorld, matRev, matParent;
	D3DXMatrixRotationZ(&matRev, m_fDegree);
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	matWorld = matRev * matParent;

	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&m_vRealVertex[i], &m_vRotVertex[i], &matWorld);

	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;

    return 0;
}

void CRocket::LateUpdate(void)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	if (CCollisionManager::CollideBullet(pObj, this) == true)
	{
		// �ǰ� �ִϸ��̼�
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
		pPlayer->SetState(GET_SINGLE(PlayerAttacked));
		pPlayer->SetIsAttacked(true);
	}
}

void CRocket::Render(const HDC& _hdc)
{
	MoveToEx(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y, nullptr);

	for (int i = 1; i < 4; i++)
		LineTo(_hdc, (int)m_vRealVertex[i].x, (int)m_vRealVertex[i].y);
	LineTo(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y);

	// Bullet �׸���
	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Bullet", L"Rocket", 0);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matWorld, matScale, matRev, matParent;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRev, m_fDegree);
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	matWorld = matScale * matRev * matParent;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CRocket::Release(void)
{
}