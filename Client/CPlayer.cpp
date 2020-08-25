#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CBoomerang.h"
#include "CShotGun.h"
#include "CGrenade.h"
#include "CObjManager.h"
#include "CCollisionManager.h"
#include "CKeyManager.h"
#include "CMonster.h"
#include "CCasing.h"
#include "CInventory.h"
#include "CPlayerManager.h"
#include "CWeapon.h"
#include "CTimeManager.h"
#include "CCameraManager.h"
#include "CGraphicDevice.h"
#include "CTextureManager.h"
#include "CAnimation.h"

CPlayer::CPlayer()
	:
	m_vRotVertex{},
	m_vRealVertex{},
	m_fShootingDegree{ 0.f },
	m_fShootingDist{ 0.f },
	m_fAddSpeed{ 1.f },
	m_bIsDash{ false },
	m_pWeapon{ nullptr },
	m_fStackTime{ 0.f }
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tPosin, sizeof(LINEINFO));
	ZeroMemory(&m_tCrossPos, sizeof(LINEPOS));
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Ready()
{
	m_tInfo.vPos = {400.f, 300.f, 0.f};
	m_tInfo.vDir = {1.0f, 0.f, 0.f};
	m_tInfo.vSize = { 50.f, 50.f, 0.f };
	m_tInfo.vLook = {1.f, 0.f, 0.f};

	m_fSpeed = 4.f;
	m_fDegree = 0.f;

	// ȸ���� ���ؽ� ��ǥ
	m_vRotVertex[0].x = - (m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[0].y = - (m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[1].x = + (m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[1].y = - (m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[2].x = + (m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[2].y = + (m_tInfo.vSize.y * 0.5f);
	m_vRotVertex[3].x = - (m_tInfo.vSize.x * 0.5f);
	m_vRotVertex[3].y = + (m_tInfo.vSize.y * 0.5f);
	
	m_eDir = DIRECTION::END;

	// ���⼭ ���� �����ϰ� ����
	GET_SINGLE(CPlayerManager)->GetInventory()->GainWeapon(GUN::DEFAULT);
	GET_SINGLE(CPlayerManager)->GetInventory()->GainWeapon(GUN::SHOTGUN);

	// ���� ����
	m_pWeapon = make_unique<CWeapon>();
	m_pWeapon->Ready();

	// �ִϸ��̼� ����
	CAnimation* pAni = CreateAnimation("PlayerAnimation");
	AddAnimationClip("Idle", ANIMATION::LOOP, 1.2f, 12, 0, 12, 0.f, L"Player", L"Idle", L"../Texture/idle_%d.png");
	
	if (pAni)
		pAni = nullptr; 
}

int CPlayer::Update(float _fDeltaTime)
{
	// ������ǥ ����
	UpdatePosinInfo();

	// ���Ⱘ��
	DetectDirection();
	TrackMousePos();
	CheckKeyState();

	if(m_bIsDash == true)
		Dash();

	m_pWeapon->Update();

	if (m_pAnimation)
		m_pAnimation->Update(GET_SINGLE(CTimeManager)->GetElapsedTime());

	m_pAnimation->ChangeClip("Idle");

	return 0;
}

void CPlayer::LateUpdate()
{
	// ���� �浹 ������ (����� ����)
	LINEINFO* pLineArray = nullptr;
	for (auto& pMonster : GET_SINGLE(CObjManager)->GetMonsters())
	{
		DO_IF_IS_NOT_VALID_OBJ(pMonster)
			continue;
		
		auto& pObj = static_cast<CMonster&>(*pMonster);
		
		pLineArray = pObj.GetLinesInfo();
		for (int i = 0; i < 3; i++)
		{
			if (CCollisionManager::CollideLineToLineReturn(pLineArray[i], m_tPosin, &m_tCrossPos))
			{
				m_tPosin.tRPoint.fX = m_tCrossPos.fX;
				m_tPosin.tRPoint.fY = m_tCrossPos.fY;
			}
		}
	}

	delete[] pLineArray;

}

void CPlayer::Render(const HDC& _hdc)
{
	if (m_bIsDash)
		ShowSpectrum(_hdc);

	// Line���� �׸���
	MoveToEx(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y, nullptr);
	for (int i = 1; i < 4; i++)
		LineTo(_hdc, (int)m_vRealVertex[i].x, (int)m_vRealVertex[i].y);
	LineTo(_hdc, (int)m_vRealVertex[0].x, (int)m_vRealVertex[0].y);

	
	// Test Idle �ִϸ��̼�
	int iFrame = 0;
	if (m_pAnimation)
	{
		ANIMATION_CLIP* pClip = m_pAnimation->GetCurrentClip();
		iFrame = pClip->iFrame; // �ε���
	}
	
	TEXINFO* pTexInfo = m_pVecTextureInfo[iFrame];

	float fCenterX = float(pTexInfo->tImageInfo.Width  * 0.5f);
	float fCenterY = float(pTexInfo->tImageInfo.Height * 0.5f);

	D3DXMATRIX matScale, matTrans, matWorld;
	if(m_eDir == DIRECTION::LEFT)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	// 20�� ��Ʈ �߽ɿ� �̹��� ���߱� ����.
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y - 20, 0.f);
	matWorld = matScale * matTrans ;

	CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



	// ���� �׸���
	m_pWeapon->Render(_hdc);

	if (GET_SINGLE(CCameraManager)->IsPressing() == true)
	{
		HPEN myPen, oldPen;
		HBRUSH myBrush, oldBrush;
		myPen = CreatePen(PS_DASHDOTDOT, 1, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(_hdc, myPen);
		myBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(_hdc, myBrush);

		// ���ű׸���
		MoveToEx(_hdc, (int)m_tPosin.tLPoint.fX, (int)m_tPosin.tLPoint.fY, nullptr);
		LineTo(_hdc, (int)m_tPosin.tRPoint.fX, (int)m_tPosin.tRPoint.fY);
		// ���� �Ӹ���
		Ellipse(_hdc, (int)m_tPosin.tRPoint.fX - 5, (int)m_tPosin.tRPoint.fY - 5,
			(int)m_tPosin.tRPoint.fX + 5, (int)m_tPosin.tRPoint.fY + 5);


		SelectObject(_hdc, oldPen);
		SelectObject(_hdc, oldBrush);
		//���� ���� �������ش�.
		DeleteObject(myPen);
		DeleteObject(myBrush);

	}

	TCHAR str[10] = {};
	wsprintf(str, TEXT("%d"), int(m_fShootingDegree));
	TextOut(_hdc, (int)m_tInfo.vPos.x, (int)m_tInfo.vPos.y, str, lstrlen(str));

}

void CPlayer::Release()
{
}

void CPlayer::ShootBoomerang(void)
{
	shared_ptr<CBullet> pBoomerang = make_shared<CBoomerang>(m_tInfo.vPos.x + m_tInfo.vDir.x * 100.f,
		m_tInfo.vPos.y + m_tInfo.vDir.y * 100.f, m_tInfo.vDir);
	pBoomerang->Ready();
	GET_SINGLE(CObjManager)->GetBullets().emplace_back(pBoomerang);
	GET_SINGLE(CCameraManager)->SetIsShooting(true);

	// ź�� ����
	shared_ptr<CCasing> pCasing = make_shared<CCasing>(m_tInfo.vPos.x,
		m_tInfo.vPos.y, m_tInfo.vDir, 10.f, m_fShootingDegree);
	pCasing->Ready();
	GET_SINGLE(CObjManager)->GetCasings().emplace_back(pCasing);
}

void CPlayer::TrackMousePos(void)
{
	// ������ - �����
	// ���콺 ��ġ�� �ް� �� ���콺 �������� ���� �����̰�
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	D3DXVECTOR3 vMouse = { float(pt.x) + m_tInfo.vPos.x - (WINCX>>1), float(pt.y) + m_tInfo.vPos.y - (WINCY >> 1), 0.f };
	m_tInfo.vDir = vMouse - m_tInfo.vPos;

	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
}

void CPlayer::CheckKeyState(void)
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matParent;
	// ��� �ʱ�ȭ
	D3DXMatrixIdentity(&matWorld);

	// �÷��̾� �̵�
	if (GET_SINGLE(CKeyManager)->Key_Pressing(KEY_A))
		m_tInfo.vPos.x -= m_fSpeed;
	if (GET_SINGLE(CKeyManager)->Key_Pressing(KEY_D))
		m_tInfo.vPos.x += m_fSpeed;
	if (GET_SINGLE(CKeyManager)->Key_Pressing(KEY_W))
		m_tInfo.vPos.y -= m_fSpeed;
	if (GET_SINGLE(CKeyManager)->Key_Pressing(KEY_S))
		m_tInfo.vPos.y += m_fSpeed;

	// ���߿� ũ�� * ���� * �̵� * ���� * �θ�
	D3DXMatrixTranslation(&matParent, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matParent;

	// ���� ��� �����ؼ� �÷��̾� ������.
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&m_vRealVertex[i], &m_vRotVertex[i], &matWorld);


	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_G))
		ShootBoomerang();

	// ����ź
	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_F))
	{
		shared_ptr<CGrenade> pGrenade = make_shared<CGrenade>(m_tInfo.vPos.x + m_tInfo.vDir.x * 100.f,
			m_tInfo.vPos.y + m_tInfo.vDir.y * 100.f, m_tInfo.vDir, 20.f, m_fShootingDegree, m_fShootingDist);
		pGrenade->Ready();
		GET_SINGLE(CObjManager)->GetGrenades().emplace_back(pGrenade);
	}

	// ����
	if (GET_SINGLE(CKeyManager)->Key_Pressing(KEY_LBUTTON))
	{
		m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fStackTime >= 0.15f)
		{
			m_fStackTime = 0.f;
			// ���⿡�� �Ѿ� �߻�
			m_pWeapon->Shoot();
		}
		
	}

	// �ܹ�
	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_LBUTTON))
		m_pWeapon->Shoot();

	// ���� ��ü
	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_E))
		GET_SINGLE(CPlayerManager)->GetInventory()->ChangeWeapon();

	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_RBUTTON)) // ī�޶� Ȯ��
	{
		GET_SINGLE(CCameraManager)->SetIsPressing(true);
		GET_SINGLE(CCameraManager)->SetIsEnlargeScale(true);
	}

	if (GET_SINGLE(CKeyManager)->Key_UP(KEY_RBUTTON)) // ī�޶� ���
	{
		GET_SINGLE(CCameraManager)->SetIsPressing(false);
		GET_SINGLE(CCameraManager)->SetIsReduceScale(true);
	}


	if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_SHIFT)) // �÷��̾� �뽬
		m_bIsDash = true;

}

void CPlayer::DetectDirection(void)
{
	// ���콺 ��ġ�� �������� ���� ����
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	// ���콺 ��ǥ
	D3DXVECTOR3 vMouse = { float(pt.x) + m_tInfo.vPos.x - (WINCX >> 1), float(pt.y) + m_tInfo.vPos.y - (WINCY >> 1), 0.f };

	float fDeltaX = vMouse.x - this->GetX();
	float fDeltaY = vMouse.y - this->GetY();

	float fDist = sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY);

	float fRadian = acosf(fDeltaX / fDist);
	if (vMouse.y < m_tInfo.vPos.y)
		fRadian = 2 * D3DX_PI - fRadian;
	float fDegree = D3DXToDegree(fRadian);
	m_fShootingDegree = fDegree;
	m_fShootingDist = fDist;

	if (135.f < fDegree && fDegree < 225.f)
		m_eDir = DIRECTION::LEFT;
	else if (315.f <= fDegree && fDegree <= 360.f ||
		0 <= fDegree && fDegree <= 45.f)
		m_eDir = DIRECTION::RIGHT;
	else if (45.f < fDegree && fDegree <= 135.f)
		m_eDir = DIRECTION::UP;
	else if (225.f < fDegree && fDegree < 315.f)
		m_eDir = DIRECTION::DOWN;

}

void CPlayer::UpdatePosinInfo(void)
{
	// ���ż��� ��ǥ
	m_tPosin.tLPoint.fX = m_tInfo.vPos.x;
	m_tPosin.tLPoint.fY = m_tInfo.vPos.y;
	m_tPosin.tRPoint.fX = m_tInfo.vPos.x + m_tInfo.vDir.x * 300.f;
	m_tPosin.tRPoint.fY = m_tInfo.vPos.y + m_tInfo.vDir.y * 300.f;
}

void CPlayer::Dash(void)
{
	if (m_fAddSpeed >= 20.f)
	{
		m_fAddSpeed = 0.f;
		m_bIsDash = false;
		return;
	}

	m_fAddSpeed += 0.5f;
	m_tInfo.vPos += m_tInfo.vDir * m_fAddSpeed;
}

// �뽬 �ܻ�
void CPlayer::ShowSpectrum(const HDC& _hdc)
{

	if (m_fAddSpeed >= 5.f && m_fAddSpeed <= 6.f)
	{
		if (m_eDir == DIRECTION::RIGHT)
			Rectangle(_hdc, GetLeft() - 150, GetTop(), GetRight() - 150, GetBottom());
		else if (m_eDir == DIRECTION::LEFT)
			Rectangle(_hdc, GetLeft() + 150, GetTop(), GetRight() + 150, GetBottom());
		else if (m_eDir == DIRECTION::UP)
			Rectangle(_hdc, GetLeft(), GetTop() - 150, GetRight(), GetBottom() - 150);
		else if (m_eDir == DIRECTION::DOWN)
			Rectangle(_hdc, GetLeft(), GetTop() + 150, GetRight(), GetBottom() + 150);
	}
	
	if (m_fAddSpeed >= 3.f && m_fAddSpeed <= 8.f)
	{
		if (m_eDir == DIRECTION::RIGHT)
			Rectangle(_hdc, GetLeft() - 100, GetTop(), GetRight() - 100, GetBottom());
		else if (m_eDir == DIRECTION::LEFT)
			Rectangle(_hdc, GetLeft() + 100, GetTop(), GetRight() + 100, GetBottom());
		else if (m_eDir == DIRECTION::UP)
			Rectangle(_hdc, GetLeft(), GetTop() - 100, GetRight(), GetBottom() - 100);
		else if (m_eDir == DIRECTION::DOWN)
			Rectangle(_hdc, GetLeft(), GetTop() + 100, GetRight(), GetBottom() + 100);
	}

	if (m_fAddSpeed >= 1.f && m_fAddSpeed <= 10.f)
	{
		if (m_eDir == DIRECTION::RIGHT)
			Rectangle(_hdc, GetLeft() - 50, GetTop(), GetRight() - 50, GetBottom());
		else if (m_eDir == DIRECTION::LEFT)
			Rectangle(_hdc, GetLeft() + 50, GetTop(), GetRight() + 50, GetBottom());
		else if (m_eDir == DIRECTION::UP)
			Rectangle(_hdc, GetLeft(), GetTop() - 50, GetRight(), GetBottom() - 50);
		else if (m_eDir == DIRECTION::DOWN)
			Rectangle(_hdc, GetLeft(), GetTop() + 50, GetRight(), GetBottom() + 50);
	}

}

