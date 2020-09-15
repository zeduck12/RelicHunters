#include "pch.h"
#include "CCameraManager.h"
#include "CObjManager.h"
#include "CPlayerManager.h"
#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CPlayer.h"
#include "CObj.h"

DEFINITION_SINGLETON(CCameraManager)

bool CCameraManager::Ready(void)
{
	// 스테이지가 GAME4일 때 처음 카메라 포커스를 보스로 잡아준다.
	if (GET_SINGLE(CSceneManager)->GetNextSceneID() == CSceneManager::SCENE_GAME4)
	{
		m_bIsFocusPlayer = false;
		for (auto& pObj : GET_SINGLE(CObjManager)->GetMonsters())
		{
			if (pObj->GetImageID() == IMAGE::BOSS)
			{
				m_vFocusPos.x = pObj->GetX();
				m_vFocusPos.y = pObj->GetY();
			}
		}
	}
	else
	{
		m_bIsFocusPlayer = true;
	}

	return true;
}

// API용 매트릭스(XFORM) 반환
XFORM CCameraManager::GetWorldMatrix(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();

	RECT rc = {};
	GetClientRect(g_hWND, &rc);
	int iCX = rc.right - rc.left;
	int iCY = rc.bottom - rc.top;

	// 카메라 확대
	// 확대한 만큼 dx, dy 확대 해주기.
	D3DXMATRIX matWorld, matScale, matMove, matParent;
	D3DXMatrixTranslation(&matMove, -pPlayer->GetX() + (iCX / 2) / m_fScale  + m_fDeltaX, -pPlayer->GetY() + (iCY / 2) / m_fScale + m_fDeltaY, 0.f);
	D3DXMatrixScaling(&matScale, 1.f * m_fScale, 1.f * m_fScale, 0.f);
	

	matWorld = matMove * matScale;
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

D3DXMATRIX CCameraManager::GetWorldD3DMatrix(void)
{
	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();

	RECT rc = {};
	GetClientRect(g_hWND, &rc);
	int iCX = rc.right - rc.left;
	int iCY = rc.bottom - rc.top;

	// 카메라 확대
	// 확대한 만큼 dx, dy 확대 해주기.
	D3DXMATRIX matWorld, matScale, matMove;

	if (GET_SINGLE(CSceneManager)->GetCurSceneID() == CSceneManager::SCENE_GAME4 && m_bIsFocusPlayer == false)
	{
		D3DXVECTOR3 vDist = pPlayer->GetInfo()->vPos - m_vFocusPos;
		D3DXVec3Normalize(&vDist, &vDist);

		m_fFocusTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		m_fFocusStartTime += GET_SINGLE(CTimeManager)->GetElapsedTime();

		if (m_fFocusStartTime >= 1.f)
		{
			if (m_fFocusTime >= 0.03f)
			{
				m_fFocusTime = 0.f;
				m_vFocusPos += vDist * 40.f;
			}
		}

		if (pPlayer->GetInfo()->vPos.x >= m_vFocusPos.x)
			m_bIsFocusPlayer = true;

		D3DXMatrixTranslation(&matMove, -m_vFocusPos.x + (iCX / 2), -m_vFocusPos.y + (iCY / 2), 0.f);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	}
	else
	{
		
		if (pPlayer->GetX() < (iCX / 2) && GET_SINGLE(CSceneManager)->GetCurSceneID() != CSceneManager::SCENE_TEST)
		{
			D3DXMatrixTranslation(&matMove, m_vTempPos.x -400.f + (iCX / 2) / m_fScale + m_fDeltaX, m_vTempPos.y -pPlayer->GetY() + (iCY / 2) / m_fScale + m_fDeltaY, 0.f);
			D3DXMatrixScaling(&matScale, 1.f * m_fScale, 1.f * m_fScale, 0.f);
		}
		else
		{
			D3DXMatrixTranslation(&matMove, -pPlayer->GetX() + (iCX / 2) / m_fScale + m_fDeltaX, -pPlayer->GetY() + (iCY / 2) / m_fScale + m_fDeltaY, 0.f);
			D3DXMatrixScaling(&matScale, 1.f * m_fScale, 1.f * m_fScale, 0.f);
		}
	}

	matWorld = matMove * matScale;

	return matWorld;
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


	if (m_bIsEnlargeScaleDash)
		ToEnlargeScaleDash();
	if (m_bIsReduceScaleDash)
		ToReduceScaleDash();
}

void CCameraManager::LateUpdate(void)
{
	// 총알을 쐈다면 true >> 카메라 진동. But 조준 중에는 이 함수 안들어오게 !
	if (m_bIsShooting)
		EarthquakeCamera();

	if (m_bIsBossDeath)
	{
		m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
		if (m_fStackTime >= 2.f)
		{
			m_fStackTime = 0.f;
			m_bIsBossDeath = false;
		}

		EarthquakeCamera2();
	}

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

	else if (pt.y < pPlayer->GetY() - 200.f && pt.y > pPlayer->GetY() - 300.f ||
		pt.y > pPlayer->GetY() + 200.f && pt.y < pPlayer->GetY() + 300.f)
	{
		float fDst = abs(pt.y - pPlayer->GetY());
		float fDeltaY = fDst - 200.f;
		if (pt.y < pPlayer->GetY() - 200.f && pt.y > pPlayer->GetY() - 300.f)
			m_fDeltaY = fDeltaY;
		else
			m_fDeltaY = -1.f * fDeltaY;
	}
	else
	{
		m_fDeltaX = 0.f;
		m_fDeltaY = 0.f;
	}
}

void CCameraManager::ToEnlargeScale(void)
{
	if (m_fScale >= 1.2f || m_bIsPressing == false)
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

void CCameraManager::ToEnlargeScaleDash(void)
{
	if (m_fScale >= 1.2f || m_bIsPressingDash == false)
	{
		m_bIsEnlargeScaleDash = false;
		return;
	}

	m_fScale += 0.1f;
}

void CCameraManager::ToReduceScaleDash(void)
{
	if (m_fScale <= 1.f)
	{
		m_bIsReduceScaleDash = false;
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
		if (pPlayer->GetX() < 400.f && GET_SINGLE(CSceneManager)->GetCurSceneID() != CSceneManager::SCENE_TEST)
		{
			m_vTempPos.x -= vDir.x * 10.f;
			m_vTempPos.y -= vDir.y * 10.f;
		}
		else
		{
			pPlayer->SetX(pPlayer->GetX() - vDir.x * 10.f);
			pPlayer->SetY(pPlayer->GetY() - vDir.y * 10.f);
		}
	}
	else if (m_iCount == 2)
	{
		if (pPlayer->GetX() < 400.f && GET_SINGLE(CSceneManager)->GetCurSceneID() != CSceneManager::SCENE_TEST)
		{
			m_vTempPos.x += vDir.x * 10.f;
			m_vTempPos.y += vDir.y * 10.f;
		}
		else
		{
			pPlayer->SetX(pPlayer->GetX() + vDir.x * 10.f);
			pPlayer->SetY(pPlayer->GetY() + vDir.y * 10.f);
		}
	}
}

void CCameraManager::EarthquakeCamera2(void)
{
	float fDelta = GetNumberMinBetweenMax(-30.f, 30.f);
	m_iStack++;

	CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
	if (m_iStack == 1)
	{
		m_fOldDelta = fDelta;
		pPlayer->SetX(pPlayer->GetX() + fDelta);
		pPlayer->SetY(pPlayer->GetY() + fDelta);
	}
	else if (m_iStack == 2)
	{
		m_iStack = 0;
		pPlayer->SetX(pPlayer->GetX() - m_fOldDelta);
		pPlayer->SetY(pPlayer->GetY() - m_fOldDelta);
	}
}

CCameraManager::CCameraManager()
	:
	m_fxWorld{},
	m_iCount{ 0 },
	m_fDeltaX{ 0.f },
	m_fDeltaY{ 0.f },
	m_fScale{ 1.f },
	m_iStack{ 0 },
	m_fOldDelta{ 0.f },
	m_fStackTime{ 0.f },
	m_fFocusTime{ 0.f },
	m_fFocusStartTime{ 0.f },
	m_bIsShooting{ false },
	m_bIsEnlargeScale{ false },
	m_bIsReduceScale{ false },
	m_bIsPressing{ false },
	m_bIsBossDeath{ false }
{
	m_vFocusPos = {0.f, 0.f, 0.f};
	m_vTempPos = { 0.f, 0.f, 0.f };
}

