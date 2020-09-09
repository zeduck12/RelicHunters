#include "pch.h"
#include "CGenerator.h"
#include "CImageSetting.h"
#include "CMonsterState.h"


CGenerator::CGenerator(float _fX, float _fY, float _fWidth, float _fHeight)
{
	m_fAddY = 0.f;
	m_fDegree = 0.f;
	m_fMaxHp = 2000.f;
	m_fCurHp = m_fMaxHp;
	m_fSpeed = 5.f;
	m_fAddSpeed = 0.f;
	m_fStackTime = 0.f;
	m_iActivationDrawID = 0;

	m_eImageID = IMAGE::GENERATOR;

	m_tInfo.vPos = { _fX, _fY, 0.f };
	m_tInfo.vDir = { 1.0f, 0.f, 0.f };
	m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_fShootingDegree = 0.f;
	m_fShootingDist = 0.f;

	m_bIsDash = false;
	m_pNextState = nullptr;
	m_pImageSetting = nullptr;
}

CGenerator::~CGenerator()
{
	Release();
}

void CGenerator::Ready(void)
{
	// 局聪皋捞记 积己
	m_pImageSetting = make_unique<CImageSetting>(this, "MonsterAnimation");
	if (!m_pImageSetting->Ready())
		return;

	m_pNextState = new SpawnState;
	m_eDir = DIRECTION::RIGHT;
}

int CGenerator::Update(float _fDeltaTime)
{
	DetectDirection();

	CMonsterState* pCurState = m_pNextState->Update(this);
	if (pCurState != nullptr)
	{
		Safe_Delete(m_pNextState);
		m_pNextState = pCurState;
	}

	return 0;
}

void CGenerator::LateUpdate(void)
{
}

void CGenerator::Release(void)
{
	if (m_pImageSetting)
		m_pImageSetting.reset();
	Safe_Delete(m_pNextState);
}

void CGenerator::Render(const HDC& _hdc)
{
	m_pNextState->Render(this);
}
