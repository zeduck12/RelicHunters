#include "pch.h"
#include "CCameraManager.h"
#include "CObjManager.h"
#include "CPlayerManager.h"
#include "CPlayer.h"
#include "CObj.h"

DEFINITION_SINGLETON(CCameraManager)

bool CCameraManager::Ready(void)
{
	return true;
}

XFORM CCameraManager::GetWorldMatrix(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();

	// 카메라 확대
	// 확대한 만큼 dx, dy 확대 해주기.
	D3DXMATRIX matWorld, matScale, matMove;
	D3DXMatrixScaling(&matScale, 1.f * m_fScale, 1.f * m_fScale, 0.f);
	D3DXMatrixTranslation(&matMove, -pPlayer->GetX() * m_fScale + (WINCX >> 1) + m_fDeltaX, -pPlayer->GetY() * m_fScale + (WINCY >> 1) + m_fDeltaY, 0.f);

	matWorld = matScale * matMove;
	m_fxWorld =
	{
		matWorld._11, 0,
		0, matWorld._22,
		matWorld._41, matWorld._42
	};
	// 11 , 22 >> Scale
	// 41 , 42 >> Move

	return m_fxWorld;
}

void CCameraManager::Update(void)
{
	// 임의의 카메라 사각형을 두고 그 범위에 오면 조금더 카메라 움직이게 하기.
	MoveCameraToFreeSpace();

	// 카메라 확대
	if (m_bIsEnlargeScale)
		ToEnlargeScale();
	// 카메라 축소
	if (m_bIsReduceScale)
		ToReduceScale();
}

void CCameraManager::LateUpdate(void)
{
	// 총알을 쐈다면 true >> 카메라 진동. But 조준 중에는 이 함수 안들어오게 !
	if (m_bIsShooting && m_bIsPressing == false)
		EarthquakeCamera();

}

void CCameraManager::MoveCameraToFreeSpace(void)
{
	// 현재 X축으로만
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);
	pt.x += LONG(pPlayer->GetX() - (WINCX >> 1));
	pt.y += LONG(pPlayer->GetY() - (WINCY >> 1));

	// 최대 마우스 범위 400.f
	// 최소 마우스 범위 300.f 
	// 카메라 움직이는 범위
	if (pt.x < pPlayer->GetX() - 300.f && pt.x > pPlayer->GetX() - 400.f ||
		pt.x > pPlayer->GetX() + 300.f && pt.x < pPlayer->GetX() + 400.f)
	{
		float fDst = abs(pt.x - pPlayer->GetX());
		float fDeltaX = fDst - 300.f;
		if (pt.x < pPlayer->GetX() - 300.f && pt.x > pPlayer->GetX() - 400.f)
			m_fDeltaX = fDeltaX;
		else
			m_fDeltaX = -1.f * fDeltaX;
	}

	if (pt.y < pPlayer->GetY() - 200.f && pt.y > pPlayer->GetY() - 300.f ||
		pt.y > pPlayer->GetY() + 200.f && pt.y < pPlayer->GetY() + 300.f)
	{
		float fDst = abs(pt.y - pPlayer->GetY());
		float fDeltaY = fDst - 200.f;
		if (pt.y < pPlayer->GetY() - 200.f && pt.y > pPlayer->GetY() - 300.f)
			m_fDeltaY = fDeltaY;
		else
			m_fDeltaY = -1.f * fDeltaY;
	}
}

void CCameraManager::ToEnlargeScale(void)
{
	if (m_fScale >= 1.5f || m_bIsPressing == false)
	{
		m_bIsEnlargeScale = false;
		return;
	}

	m_fScale += 0.1f;
}

void CCameraManager::ToReduceScale(void)
{
	if (m_fScale <= 1.f)
	{
		m_bIsReduceScale = false;
		return;
	}

	m_fScale -= 0.1f;
}

void CCameraManager::EarthquakeCamera(void)
{
	if (m_iCount >= 2)
	{
		m_bIsShooting = false;
		m_iCount = 0;
		return;
	}

	m_iCount++;

	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	D3DXVECTOR3 vDir = dynamic_cast<CPlayer*>(pPlayer)->GetDirectionVector();

	if (m_iCount == 1)
	{
		pPlayer->SetX(pPlayer->GetX() - vDir.x * 10.f);
		pPlayer->SetY(pPlayer->GetY() - vDir.y * 10.f);
	}
	else if (m_iCount == 2)
	{
		pPlayer->SetX(pPlayer->GetX() + vDir.x * 10.f);
		pPlayer->SetY(pPlayer->GetY() + vDir.y * 10.f);
	}
}

CCameraManager::CCameraManager()
	:
	m_fxWorld{},
	m_iCount{ 0 },
	m_fDeltaX{ 0.f },
	m_fDeltaY{ 0.f },
	m_fScale{ 1.f },
	m_bIsShooting{ false },
	m_bIsEnlargeScale{ false },
	m_bIsReduceScale{ false },
	m_bIsPressing{ false }
{

}

