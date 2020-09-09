#include "pch.h"
#include "CKamikazeCage.h"
#include "CImageSetting.h"
#include "CMonsterState.h"
#include "CObjManager.h"

CKamikazeCage::CKamikazeCage(float _fX, float _fY, float _fWidth, float _fHeight)
{
	m_fAddY = 0.f;
	m_fDegree = 0.f;
	m_fMaxHp = 200.f;
	m_fCurHp = m_fMaxHp;
	m_fSpeed = 5.f;
	m_fAddSpeed = 0.f;
	m_fStackTime = 0.f;
	m_iActivationDrawID = 0;

	m_eImageID = IMAGE::KAMIKAZE_CAGE;

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

CKamikazeCage::~CKamikazeCage()
{
    Release();
}

void CKamikazeCage::Ready(void)
{
	// 局聪皋捞记 积己
	m_pImageSetting = make_unique<CImageSetting>(this, "MonsterAnimation");
	if (!m_pImageSetting->Ready())
		return;

	m_pNextState = new IdleState;
	m_eDir = DIRECTION::RIGHT;
}

int CKamikazeCage::Update(float _fDeltaTime)
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

void CKamikazeCage::LateUpdate(void)
{
}

void CKamikazeCage::Release(void)
{
	if (m_pImageSetting)
		m_pImageSetting.reset();
	Safe_Delete(m_pNextState);
}

void CKamikazeCage::Render(const HDC& _hdc)
{
	m_pNextState->Render(this);
}

void CKamikazeCage::CreateKamikazes(void)
{
	//1669 756
	shared_ptr<CObj> pMonster = make_shared<CMonster>(
		m_tInfo.vPos.x,
		m_tInfo.vPos.y,
		50.f, 50.f, cfMosterDefaultSpeed, cfMosterDefaultHp,IMAGE::KAMIKAZE);
	pMonster->Ready();
	GET_SINGLE(CObjManager)->GetMonsters().emplace_back(pMonster);

	pMonster = make_shared<CMonster>(
		m_tInfo.vPos.x - 30.f,
		m_tInfo.vPos.y - 30.f,
		50.f, 50.f, cfMosterDefaultSpeed, cfMosterDefaultHp, IMAGE::KAMIKAZE);
	pMonster->Ready();
	GET_SINGLE(CObjManager)->GetMonsters().emplace_back(pMonster);

	pMonster = make_shared<CMonster>(
		m_tInfo.vPos.x + 30.f,
		m_tInfo.vPos.y + 30.f,
		50.f, 50.f, cfMosterDefaultSpeed, cfMosterDefaultHp, IMAGE::KAMIKAZE);
	pMonster->Ready();
	GET_SINGLE(CObjManager)->GetMonsters().emplace_back(pMonster);

	pMonster = make_shared<CMonster>(
		m_tInfo.vPos.x - 40.f,
		m_tInfo.vPos.y,
		50.f, 50.f, cfMosterDefaultSpeed, cfMosterDefaultHp, IMAGE::KAMIKAZE);
	pMonster->Ready();
	GET_SINGLE(CObjManager)->GetMonsters().emplace_back(pMonster);

	pMonster = make_shared<CMonster>(
		m_tInfo.vPos.x,
		m_tInfo.vPos.y + 40.f,
		50.f, 50.f, cfMosterDefaultSpeed, cfMosterDefaultHp, IMAGE::KAMIKAZE);
	pMonster->Ready();
	GET_SINGLE(CObjManager)->GetMonsters().emplace_back(pMonster);
}
