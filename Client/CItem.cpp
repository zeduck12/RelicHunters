#include "pch.h"
#include "CItem.h"
#include "CItemState.h"
#include "CImageSetting.h"
#include "CInteractionManager.h"
#include "CPlayerManager.h"
#include "CTimeManager.h"

CPickUpLight::CPickUpLight(float _fX, float _fY, float _fWidth, float _fHeight, IMAGE::ID _eID)
{
    m_tInfo.vPos = { _fX, _fY, 0.f };
    m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
	m_tInfo.vDir = { 1.0f, 0.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

    m_eImageID = _eID;
    m_pNextState = nullptr;
    m_pImageSetting = nullptr;
}

CPickUpLight::~CPickUpLight()
{
    Release();
}

void CPickUpLight::Ready(void)
{
    m_iDrawID = 0;
    m_pImageSetting = make_unique<CImageSetting>(this, "PickUpLightAnimation");
    if (!m_pImageSetting->Ready())
        return;

    m_pNextState = new CItemIdleState;
}

int CPickUpLight::Update(float _fDeltaTime)
{
    // Interaction Manager 만들어서 근처에 오면 E누르면 아이템 획득할수 있게하기
    // 그리고 아이템 획득
   
    CItemState* pCurState = m_pNextState->Update(this);
    if (pCurState != nullptr)
    {
        Safe_Delete(m_pNextState);
        m_pNextState = pCurState;
    }

    return 0;
}

void CPickUpLight::Release(void)
{
    Safe_Delete(m_pNextState);
}

void CPickUpLight::Render(const HDC& _hdc)
{
    m_pNextState->Render(this);
    // 상호작용
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    if (CInteractionManager::InteractPlayerItem(pPlayer, this) == true)
    {
        m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
        if (m_fStackTime >= m_fCoolTime)
        {
            m_fStackTime = 0.f;
            this->SetDrawID(this->GetDrawID() + 1);
        }

        CInteractionManager::Render(this);
    }

}

CPickUpMedium::CPickUpMedium(float _fX, float _fY, float _fWidth, float _fHeight, IMAGE::ID _eID)
{
    m_tInfo.vPos = { _fX, _fY, 0.f };
    m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
    m_tInfo.vDir = { 1.0f, 0.f, 0.f };
    m_tInfo.vLook = { 1.f, 0.f, 0.f };

    m_eImageID = _eID;
    m_pNextState = nullptr;
    m_pImageSetting = nullptr;
}

CPickUpMedium::~CPickUpMedium()
{
    Release();
}

void CPickUpMedium::Ready(void)
{
    m_iDrawID = 0;
    m_pImageSetting = make_unique<CImageSetting>(this, "PickUpMediumAnimation");
    if (!m_pImageSetting->Ready())
        return;

    m_pNextState = new CItemIdleState;
}

int CPickUpMedium::Update(float _fDeltaTime)
{
    // Interaction Manager 만들어서 근처에 오면 E누르면 아이템 획득할수 있게하기
   // 그리고 아이템 획득

    CItemState* pCurState = m_pNextState->Update(this);
    if (pCurState != nullptr)
    {
        Safe_Delete(m_pNextState);
        m_pNextState = pCurState;
    }

    return 0;
}

void CPickUpMedium::Release(void)
{
    Safe_Delete(m_pNextState);
}

void CPickUpMedium::Render(const HDC& _hdc)
{
    m_pNextState->Render(this);
    // 상호작용
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    if (CInteractionManager::InteractPlayerItem(pPlayer, this) == true)
    {
        m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
        if (m_fStackTime >= m_fCoolTime)
        {
            m_fStackTime = 0.f;
            this->SetDrawID(this->GetDrawID() + 1);
        }

        CInteractionManager::Render(this);
    }
}

CPickUpHeavy::CPickUpHeavy(float _fX, float _fY, float _fWidth, float _fHeight, IMAGE::ID _eID)
{
    m_tInfo.vPos = { _fX, _fY, 0.f };
    m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
    m_tInfo.vDir = { 1.0f, 0.f, 0.f };
    m_tInfo.vLook = { 1.f, 0.f, 0.f };

    m_eImageID = _eID;
    m_pNextState = nullptr;
    m_pImageSetting = nullptr;
}

CPickUpHeavy::~CPickUpHeavy()
{
    Release();
}

void CPickUpHeavy::Ready(void)
{
    m_iDrawID = 0;
    m_pImageSetting = make_unique<CImageSetting>(this, "PickUpMediumAnimation");
    if (!m_pImageSetting->Ready())
        return;

    m_pNextState = new CItemIdleState;
}

int CPickUpHeavy::Update(float _fDeltaTime)
{
    // Interaction Manager 만들어서 근처에 오면 E누르면 아이템 획득할수 있게하기
   // 그리고 아이템 획득

    CItemState* pCurState = m_pNextState->Update(this);
    if (pCurState != nullptr)
    {
        Safe_Delete(m_pNextState);
        m_pNextState = pCurState;
    }

    return 0;
}

void CPickUpHeavy::Release(void)
{
    Safe_Delete(m_pNextState);
}

void CPickUpHeavy::Render(const HDC& _hdc)
{
    m_pNextState->Render(this);
    // 상호작용
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    if (CInteractionManager::InteractPlayerItem(pPlayer, this) == true)
    {
        m_fStackTime += GET_SINGLE(CTimeManager)->GetElapsedTime();
        if (m_fStackTime >= m_fCoolTime)
        {
            m_fStackTime = 0.f;
            this->SetDrawID(this->GetDrawID() + 1);
        }

        CInteractionManager::Render(this);
    }
}
