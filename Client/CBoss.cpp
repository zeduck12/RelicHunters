#include "pch.h"
#include "CBoss.h"
#include "CImageSetting.h"
#include "CGraphicDevice.h"
#include "CTextureManager.h"
#include "CBossState.h"
#include "CPlayer.h"
#include "CPlayerManager.h"
#include "CCollisionManager.h"
#include "CPlayerState.h"
#include "CBullet.h"
#include "CRocket.h"
#include "CObjManager.h"
#include "CShotGun.h"
#include "CMapManager.h"
#include "UICameraManager.h"
#include "CBossHpBar.h"

CBoss::CBoss(float _fX, float _fY, float _fWidth, float _fHeight, float _fSpeed, float _fHp, IMAGE::ID _eID)
	:
	m_pBossNextState{ nullptr }
{
	m_fAddSpeed = 0.f;
	m_bIsDash = false;
	m_bIsCrack = false;
	m_bIsPhase2 = false;
	m_fDegree = 0.f;
	m_fMaxHp = _fHp;
	m_fStackTime = 0.f;

	m_tInfo.vPos = { _fX, _fY, 0.f };
	m_tInfo.vDir = { 1.0f, 0.f, 0.f };
	m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_fShootingDegree = 0.f;
	m_fShootingDist = 0.f;

	m_fSpeed = _fSpeed;
	m_fCurHp = m_fMaxHp;

	m_eImageID = _eID;
	m_pImageSetting = nullptr;

	// ����
	m_vRotVertex[0].x = m_tInfo.vSize.x * 0.5f;
	m_vRotVertex[0].y = 0.f;

	// �������� ������ �Ǵ� ���� ȸ���ؼ�
	D3DXMATRIX matWorld, matRot;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(120.f));
	// �ϴ� ȸ��
	matWorld = matRot;
	D3DXVec3TransformCoord(&m_vRotVertex[1], &m_vRotVertex[0], &matWorld);
	D3DXVec3TransformCoord(&m_vRotVertex[2], &m_vRotVertex[1], &matWorld);
	// �ﰢ�� ���ؽ� ��ġ �Ϸ�
}

CBoss::~CBoss()
{
	Release();
}

void CBoss::Ready(void)
{
	// �̹��� ����
	m_pImageSetting = make_unique<CImageSetting>(this, "BossAnimation");
	if (!m_pImageSetting->Ready())
		return;

	m_pBossNextState = new EggIdleState;
	//m_pBossNextState = new BossIdleState;
	m_eDir = DIRECTION::RIGHT;

	shared_ptr<CBossHpBar> pBossHpBar = make_shared<CBossHpBar>(this);
	pBossHpBar->Ready();
	GET_SINGLE(UICameraManager)->SetBossHpBar(pBossHpBar);

}

int CBoss::Update(float _fDeltaTime)
{
	DetectDirection();

	CBossState* pCurState = m_pBossNextState->Update(this);
	if (pCurState != nullptr)
	{
		delete	m_pBossNextState;
		m_pBossNextState = nullptr;

		m_pBossNextState = pCurState;
	}

	// ���� ���� �÷��̾� �������� ����. ���߿� �̹��� ������ �׶��� �¿츸
	UpdateMonsterDirection();

	if (m_bIsDash == true)
		Dash();

	return 0;
}

void CBoss::LateUpdate(void)
{
	for (auto& pTile : GET_SINGLE(CMapManager)->GetWalls())
		CCollisionManager::CollideCharacterTile(this, pTile);

	for (auto& pStruc : GET_SINGLE(CMapManager)->GetStructures())
		CCollisionManager::CollideCharacterStructure(this, pStruc.get());

	// �÷��̾� �⵿ ���� �� �浹
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	LINEINFO* pPlayerLineArray = dynamic_cast<CPlayer*>(pPlayer)->GetLinesInfo();
	LINEINFO* pMonsterLineArray = GetLinesInfo();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (CCollisionManager::CollideLineToLine(pPlayerLineArray[i], pMonsterLineArray[j]))
			{
				// �浹�� �Ͼ�ٸ� ���⺤�� ������ �б�
				pPlayer->SetX(pPlayer->GetX() + m_tInfo.vDir.x);
				pPlayer->SetY(pPlayer->GetY() + m_tInfo.vDir.y);
				dynamic_cast<CPlayer*>(pPlayer)->SetState(GET_SINGLE(PlayerAttacked));
			}
		}
	}

	// �پ� �������� ����
	delete[] pPlayerLineArray;
	delete[] pMonsterLineArray;



	if (m_fCurHp <= m_fMaxHp * 0.5f && m_bIsPhase2 == false)
	{
		m_bIsPhase2 = true;
		//������ 2���� ����
	}



}

void CBoss::Render(const HDC& _hdc)
{
	if (m_bIsDash == true)
		ShowBossSpectrum();
	m_pBossNextState->Render(this);
	
	if(m_bIsCrack == true)
		EquipWeapon(); // �� ����

	// �ﰢ�� �׸���
	MoveToEx(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y, nullptr);
	for (int i = 1; i < 3; i++)
		LineTo(_hdc, (int)m_vRealVertex[i].x, (int)m_vRealVertex[i].y);
	LineTo(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y);


	// �� ����Ʈ
	RECT rcEllipse = { LONG(m_vRealVertex[0].x - 10), LONG(m_vRealVertex[0].y - 10),
		LONG(m_vRealVertex[0].x + 10) , LONG(m_vRealVertex[0].y + 10) };
	Ellipse(_hdc, rcEllipse.left, rcEllipse.top, rcEllipse.right, rcEllipse.bottom);
}

void CBoss::Release(void)
{
	SAFE_DELETE(m_pBossNextState);
}

void CBoss::ShootRocket(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return;

	// ���ð��� ���
	float fDeltaX = pPlayer->GetX() - this->GetX();
	float fDeltaY = pPlayer->GetY() - this->GetY();
	float fDist = sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY);

	float fRadian = acosf(fDeltaX / fDist);
	if (pPlayer->GetY() < m_tInfo.vPos.y)
		fRadian = 2 * D3DX_PI - fRadian;

	D3DXVECTOR3 vDeltaPos = pPlayer->GetInfo()->vPos - this->GetInfo()->vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &vDeltaPos);

	if (m_eDir == DIRECTION::LEFT)
	{
		shared_ptr<CObj> pBullet = make_shared<CRocket>(
			this->GetX() + m_tInfo.vDir.x * 50.f - 25.f, this->GetY() + m_tInfo.vDir.y * 50.f + 25.f,
			m_tInfo.vDir, 5.f, D3DXToDegree(fRadian),
			OBJ::MONSTER);
		pBullet->Ready();
		GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBullet);
	}
	else
	{
		shared_ptr<CObj> pBullet = make_shared<CRocket>(
			this->GetX() + m_tInfo.vDir.x * 50.f + 25.f, this->GetY() + m_tInfo.vDir.y * 50.f + 25.f,
			m_tInfo.vDir, 5.f, D3DXToDegree(fRadian),
			OBJ::MONSTER);
		pBullet->Ready();
		GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBullet);
	}

}

void CBoss::ShootShotgun(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return;

	GET_SINGLE(CSoundManager)->StopSound(CSoundManager::MONSTER);
	GET_SINGLE(CSoundManager)->PlaySound((TCHAR*)L"sfx_shotgun.wav", CSoundManager::MONSTER);

	// ���ð��� ���
	float fDeltaX = pPlayer->GetX() - this->GetX();
	float fDeltaY = pPlayer->GetY() - this->GetY();
	float fDist = sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY);

	float fRadian = acosf(fDeltaX / fDist);
	if (pPlayer->GetY() < m_tInfo.vPos.y)
		fRadian = 2 * D3DX_PI - fRadian;

	D3DXVECTOR3 vDeltaPos = pPlayer->GetInfo()->vPos - this->GetInfo()->vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &vDeltaPos);

	// Shot Gun �߻�
	shared_ptr<CBullet> pShotGun = nullptr;
	for (int i = 0; i < 3; i++)
	{
		pShotGun = make_shared<CShotGun>(this->GetX() + this->GetDirectionVector().x * 10.f,
			this->GetY() + this->GetDirectionVector().y * 10.f,
			this->GetDirectionVector(), -7.f * (i - 1), 15.f, D3DXToDegree(fRadian), OBJ::MONSTER, L"Blue");
		pShotGun->Ready();
		GET_SINGLE(CObjManager)->GetBullets().emplace_back(pShotGun);
	}

}

void CBoss::FullRangeAttack(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_NOT_VALID_OBJ(pPlayer)
		return;

	GET_SINGLE(CSoundManager)->StopSound(CSoundManager::MONSTER);
	GET_SINGLE(CSoundManager)->PlaySound((TCHAR*)L"sfx_shotgun.wav", CSoundManager::MONSTER);

	// ���ð��� ���
	float fDeltaX = pPlayer->GetX() - this->GetX();
	float fDeltaY = pPlayer->GetY() - this->GetY();
	float fDist = sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY);

	float fRadian = acosf(fDeltaX / fDist);
	if (pPlayer->GetY() < m_tInfo.vPos.y)
		fRadian = 2 * D3DX_PI - fRadian;

	D3DXVECTOR3 vDeltaPos = pPlayer->GetInfo()->vPos - this->GetInfo()->vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &vDeltaPos);

	shared_ptr<CBullet> pShotGun = nullptr;
	for (int i = 0; i < 8; i++)
	{
		pShotGun = make_shared<CShotGun>(this->GetX() + this->GetDirectionVector().x * 10.f,
			this->GetY() + this->GetDirectionVector().y * 10.f,
			this->GetDirectionVector(), -45.f * (i - 4), 15.f, D3DXToDegree(fRadian), OBJ::MONSTER, L"Blue");
		pShotGun->Ready();
		GET_SINGLE(CObjManager)->GetBullets().emplace_back(pShotGun);
	}
}

void CBoss::ShowBossSpectrum(void)
{
	if (m_fAddSpeed >= 6.f && m_fAddSpeed <= 8.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Boss", L"Move", 5);

		float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
		float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

		D3DXMATRIX matScale, matTrans, matWorld;
		if (this->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos - (this->GetInfo()->vDir * 60.f);
		vSpectrumPos.y -= 20.f;

		// 20�� ��Ʈ �߽ɿ� �̹��� ���߱� ����.
		D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}

	if (m_fAddSpeed >= 5.f && m_fAddSpeed <= 9.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Boss", L"Move", 4);

		float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
		float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

		D3DXMATRIX matScale, matTrans, matWorld;
		if (this->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos - (this->GetInfo()->vDir * 50.f);
		vSpectrumPos.y -= 20.f;

		// 20�� ��Ʈ �߽ɿ� �̹��� ���߱� ����.
		D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}

	if (m_fAddSpeed >= 4.f && m_fAddSpeed <= 10.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Boss", L"Move", 3);

		float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
		float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

		D3DXMATRIX matScale, matTrans, matWorld;
		if (this->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos - (this->GetInfo()->vDir * 40.f);
		vSpectrumPos.y -= 20.f;

		// 20�� ��Ʈ �߽ɿ� �̹��� ���߱� ����.
		D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}

	if (m_fAddSpeed >= 3.f && m_fAddSpeed <= 11.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Boss", L"Move", 2);

		float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
		float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

		D3DXMATRIX matScale, matTrans, matWorld;
		if (this->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos - (this->GetInfo()->vDir * 30.f);
		vSpectrumPos.y -= 20.f;

		D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}

	if (m_fAddSpeed >= 2.f && m_fAddSpeed <= 12.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Boss", L"Move", 1);

		float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
		float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

		D3DXMATRIX matScale, matTrans, matWorld;
		if (this->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos - (this->GetInfo()->vDir * 20.f);
		vSpectrumPos.y -= 20.f;

		D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}

	if (m_fAddSpeed >= 1.f && m_fAddSpeed <= 13.f)
	{
		const TEXINFO* pTexInfo = GET_SINGLE(CTextureManager)->GetTextureInfo(L"Boss", L"Move", 0);

		float fCenterX = float(pTexInfo->tImageInfo.Width * 0.5f);
		float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

		D3DXMATRIX matScale, matTrans, matWorld;
		if (this->GetDirection() == DIRECTION::LEFT)
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		D3DXVECTOR3 vSpectrumPos = this->GetInfo()->vPos - (this->GetInfo()->vDir * 10.f);
		vSpectrumPos.y -= 20.f;

		// 20�� ��Ʈ �߽ɿ� �̹��� ���߱� ����.
		D3DXMatrixTranslation(&matTrans, vSpectrumPos.x, vSpectrumPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}
}

bool CBoss::IsDetectPlayerBossVersion(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_VALID_OBJ(pPlayer)
	{
		// �������� �÷��̾ �ִٸ� true or ���ٸ� false.
		if (pPlayer->GetX() > this->GetX() - 600.f &&
			pPlayer->GetX() < this->GetX() + 600.f &&
			pPlayer->GetY() > this->GetY() - 600.f &&
			pPlayer->GetY() < this->GetY() + 600.f)
		{
			return true;
		}
	}

	return false;
}

bool CBoss::IsInAttackRangePlayerBossVersion(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	DO_IF_IS_VALID_OBJ(pPlayer)
	{
		// �������� �÷��̾ �ִٸ� true or ���ٸ� false.
		if (pPlayer->GetX() > this->GetX() - 400.f &&
			pPlayer->GetX() < this->GetX() + 400.f &&
			pPlayer->GetY() > this->GetY() - 400.f &&
			pPlayer->GetY() < this->GetY() + 400.f)
		{
			return true;
		}
	}

	return false;
}

