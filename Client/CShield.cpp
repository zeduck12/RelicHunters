#include "pch.h"
#include "CShield.h"
#include "CPlayerManager.h"
#include "CPlayer.h"
#include "CTimeManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"

void CShield::Ready(void)
{
	m_iDrawID = 0;
	m_fStackTime = 0.f;
	m_fShieldMaxHp = 100.f;
	m_fShieldCurHp = m_fShieldMaxHp;

	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 1.0f, 0.f, 0.f };
	m_tInfo.vSize = { 60.f, 60.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	// ȸ���� ���ؽ� ��ǥ
	m_vRotVertex[0].x = -(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[0].y = -(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[1].x = +(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[1].y = -(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[2].x = +(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[2].y = +(m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[3].x = -(m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[3].y = +(m_tInfo.vSize.y * 0.5f);
}

int CShield::Update(float _fDeltaTime )
{
	// ���尡 �������� �Դ´ٸ� ���� ���ݾ� ȸ��
	RecoverShield();
	UpdateShieldPos();
    return 0;
}

void CShield::LateUpdate(void)
{
}

void CShield::Release(void)
{
}

void CShield::Render(const HDC& _hdc)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(GET_SINGLE(CPlayerManager)->GetPlayer());
	if (pPlayer->IsAttacked() == true)
		DrawShieldActive(); // ������ ����ߵ�
	else
		m_iDrawID = 0;

	if (m_fShieldCurHp <= 0.f)
		DrawShieldExplosion();
}

void CShield::DrawShieldActive(void)
{
	if (m_fShieldCurHp <= 0.f)
		return;

	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 0.1f)
	{
		m_iDrawID++;
		m_fStackTime = 0.f;
	}

	if (m_iDrawID >= 6)
		m_iDrawID = 5;

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Shield", L"Active", m_iDrawID);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matWorld, matScale, matRev, matParent;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRev, D3DXToRadian(m_fDegree));
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y - 25.f, m_tInfo.vPos.z);

	matWorld = matScale * matRev * matParent;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CShield::DrawShieldExplosion(void)
{
	m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
	if (m_fStackTime >= 0.1f)
	{
		m_iDrawID++;
		m_fStackTime = 0.f;
	}

	if (m_iDrawID >= 9)
		m_iDrawID = 8;

	const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Shield", L"Explosion", m_iDrawID);
	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matWorld, matScale, matRev, matParent;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRev, D3DXToRadian(m_fDegree));
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y - 25.f, m_tInfo.vPos.z);

	matWorld = matScale * matRev * matParent;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CShield::RecoverShield(void)
{
	// �÷��̾ ���ݹ޴����̶�� ȸ�� x  or ���尡 Ǯ�ǿ��� ȸ�� x
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(GET_SINGLE(CPlayerManager)->GetPlayer());
	if (pPlayer->IsAttacked() == true || m_fShieldCurHp >= m_fShieldMaxHp)
		return;

	m_fShieldCurHp += 5.f * GET_SINGLE(CTimeManager)->GetElapsedTime();
}

void CShield::UpdateShieldPos(void)
{
	CObj* pObj = GET_SINGLE(CPlayerManager)->GetPlayer();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
	DO_IF_IS_NOT_VALID_OBJ(pObj) { return; }

	m_tInfo.vPos.x = pPlayer->GetX();
	m_tInfo.vPos.y = pPlayer->GetY();
}
