#include "pch.h"
#include "CBoss.h"
#include "CImageSetting.h"
#include "CGraphicDevice.h"
#include "CTextureManager.h"
#include "CBossState.h"

CBoss::CBoss(float _fX, float _fY, float _fWidth, float _fHeight, float _fSpeed, float _fHp, IMAGE::ID _eID)
	:
	m_pNextState{ nullptr }
{
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
}

CBoss::~CBoss()
{
	Release();
}

void CBoss::Ready(void)
{
	// 이미지 셋팅
	m_pImageSetting = make_unique<CImageSetting>(this, "BossAnimation");
	if (!m_pImageSetting->Ready())
		return;

	m_pNextState = new BossIdleState;
	m_eDir = DIRECTION::RIGHT;
}

int CBoss::Update(float _fDeltaTime)
{
	DetectDirection();

	CBossState* pCurState = m_pNextState->Update(this);
	if (pCurState != nullptr)
	{
		delete	m_pNextState;
		m_pNextState = nullptr;

		m_pNextState = pCurState;
	}

	if (m_pNextState != nullptr)
		m_pNextState->Update(this);

	// 몬스터 방향 플레이어 기준으로 설정. 나중에 이미지 넣으면 그때는 좌우만
	UpdateMonsterDirection();

	return 0;
}

void CBoss::LateUpdate(void)
{
}

void CBoss::Render(const HDC& _hdc)
{
	m_pNextState->Render(this);

	EquipWeapon(); // 총 장착
}

void CBoss::Release(void)
{
	SAFE_DELETE(m_pNextState);
}

