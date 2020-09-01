#include "pch.h"
#include "CItem.h"
#include "CItemState.h"
#include "CImageSetting.h"
#include "CInteractionManager.h"
#include "CPlayerManager.h"
#include "CTimeManager.h"
#include "CTextureManager.h"
#include "CGraphicDevice.h"
#include "CKeyManager.h"
#include "CPlayerManager.h"
#include "CInventory.h"

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

DefaultGun::DefaultGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID)
{
    m_iDrawID = 0;
    m_pAnimation = nullptr;
    m_pNextState = nullptr;
    m_pImageSetting = nullptr;

    m_tInfo.vPos = { _fX, _fY, 0.f };
    m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
    m_tInfo.vDir = { 1.0f, 0.f, 0.f };
    m_tInfo.vLook = { 1.f, 0.f, 0.f };

    m_eImageID = IMAGE::END;
    m_eGunID   = _eID;
}

DefaultGun::~DefaultGun()
{
    Release();
}

int DefaultGun::Update(float _fDeltaTime)
{
    return 0;
}

void DefaultGun::Render(const HDC& _hdc)
{
    const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Pistol", 0);
    if (nullptr == pTexInfo)
        return;
    float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
    float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

    D3DXMATRIX matScale, matTrans, matWorld;
    D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
    D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
    matWorld = matScale * matTrans;

    CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
    CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

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


FlameGun::FlameGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID)
{
    m_iDrawID = 0;
    m_pAnimation = nullptr;
    m_pNextState = nullptr;
    m_pImageSetting = nullptr;

    m_tInfo.vPos = { _fX, _fY, 0.f };
    m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
    m_tInfo.vDir = { 1.0f, 0.f, 0.f };
    m_tInfo.vLook = { 1.f, 0.f, 0.f };

    m_eImageID = IMAGE::END;
    m_eGunID = _eID;
}

FlameGun::~FlameGun()
{
    Release();
}

int FlameGun::Update(float _fDeltaTime)
{
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    if (CInteractionManager::InteractPlayerItem(pPlayer, this) == true)
    {
        // 상호작용 상태라면 만약에 E를 눌렀을때 총이 샷건으로 바뀌게
        if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_E))
        {
            GET_SINGLE(CPlayerManager)->GetInventory()->GainWeapon(GUN::FLAME);
            this->SetIsValid(false);
        }
    }
    return 0;
}

void FlameGun::Render(const HDC& _hdc)
{
    const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Flame", 0);
    if (nullptr == pTexInfo)
        return;
    
    float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
    float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

    D3DXMATRIX matScale, matTrans, matWorld;
    D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
    D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
    matWorld = matScale * matTrans;

    CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
    CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

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

HighMagGun::HighMagGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID)
{
    m_iDrawID = 0;
    m_pAnimation = nullptr;
    m_pNextState = nullptr;
    m_pImageSetting = nullptr;

    m_tInfo.vPos = { _fX, _fY, 0.f };
    m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
    m_tInfo.vDir = { 1.0f, 0.f, 0.f };
    m_tInfo.vLook = { 1.f, 0.f, 0.f };

    m_eImageID = IMAGE::END;
    m_eGunID = _eID;
}

HighMagGun::~HighMagGun()
{
    Release();
}

int HighMagGun::Update(float _fDeltaTime)
{
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    if (CInteractionManager::InteractPlayerItem(pPlayer, this) == true)
    {
        // 상호작용 상태라면 만약에 E를 눌렀을때 총이 샷건으로 바뀌게
        if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_E))
        {
            GET_SINGLE(CPlayerManager)->GetInventory()->GainWeapon(GUN::HIGH_MAG);
            this->SetIsValid(false);
        }
    }

    return 0;
}

void HighMagGun::Render(const HDC& _hdc)
{
    const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"HighMag", 0);
    if (nullptr == pTexInfo)
        return;

    float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
    float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

    D3DXMATRIX matScale, matTrans, matWorld;
    D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
    D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
    matWorld = matScale * matTrans;

    CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
    CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

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

AssaultGun::AssaultGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID)
{
    m_iDrawID = 0;
    m_pAnimation = nullptr;
    m_pNextState = nullptr;
    m_pImageSetting = nullptr;

    m_tInfo.vPos = { _fX, _fY, 0.f };
    m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
    m_tInfo.vDir = { 1.0f, 0.f, 0.f };
    m_tInfo.vLook = { 1.f, 0.f, 0.f };

    m_eImageID = IMAGE::END;
    m_eGunID = _eID;
}

AssaultGun::~AssaultGun()
{
    Release();
}

int AssaultGun::Update(float _fDeltaTime)
{
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    if (CInteractionManager::InteractPlayerItem(pPlayer, this) == true)
    {
        // 상호작용 상태라면 만약에 E를 눌렀을때 총이 샷건으로 바뀌게
        if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_E))
        {
            GET_SINGLE(CPlayerManager)->GetInventory()->GainWeapon(GUN::ASSAULT);
            this->SetIsValid(false);
        }
    }
    return 0;
}

void AssaultGun::Render(const HDC& _hdc)
{
    const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Assault", 0);
    if (nullptr == pTexInfo)
        return;

    float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
    float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

    D3DXMATRIX matScale, matTrans, matWorld;
    D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
    D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
    matWorld = matScale * matTrans;

    CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
    CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

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

KeytarGun::KeytarGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID)
{
    m_iDrawID = 0;
    m_pAnimation = nullptr;
    m_pNextState = nullptr;
    m_pImageSetting = nullptr;

    m_tInfo.vPos = { _fX, _fY, 0.f };
    m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
    m_tInfo.vDir = { 1.0f, 0.f, 0.f };
    m_tInfo.vLook = { 1.f, 0.f, 0.f };

    m_eImageID = IMAGE::END;
    m_eGunID = _eID;
}

KeytarGun::~KeytarGun()
{
    Release();
}

int KeytarGun::Update(float _fDeltaTime)
{
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    if (CInteractionManager::InteractPlayerItem(pPlayer, this) == true)
    {
        // 상호작용 상태라면 만약에 E를 눌렀을때 총이 샷건으로 바뀌게
        if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_E))
        {
            GET_SINGLE(CPlayerManager)->GetInventory()->GainWeapon(GUN::KEYTAR);
            this->SetIsValid(false);
        }
    }
    return 0;
}

void KeytarGun::Render(const HDC& _hdc)
{
    const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Keytar", 0);
    if (nullptr == pTexInfo)
        return;

    float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
    float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

    D3DXMATRIX matScale, matTrans, matWorld;
    D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
    D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
    matWorld = matScale * matTrans;

    CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
    CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

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

MachineGun::MachineGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID)
{
    m_iDrawID = 0;
    m_pAnimation = nullptr;
    m_pNextState = nullptr;
    m_pImageSetting = nullptr;

    m_tInfo.vPos = { _fX, _fY, 0.f };
    m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
    m_tInfo.vDir = { 1.0f, 0.f, 0.f };
    m_tInfo.vLook = { 1.f, 0.f, 0.f };

    m_eImageID = IMAGE::END;
    m_eGunID = _eID;
}

MachineGun::~MachineGun()
{
    Release();
}

int MachineGun::Update(float _fDeltaTime)
{
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    if (CInteractionManager::InteractPlayerItem(pPlayer, this) == true)
    {
        // 상호작용 상태라면 만약에 E를 눌렀을때 총이 샷건으로 바뀌게
        if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_E))
        {
            GET_SINGLE(CPlayerManager)->GetInventory()->GainWeapon(GUN::MACHINEGUN);
            this->SetIsValid(false);
        }
    }

    return 0;
}

void MachineGun::Render(const HDC& _hdc)
{
    const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"MachineGun", 0);
    if (nullptr == pTexInfo)
        return;

    float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
    float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

    D3DXMATRIX matScale, matTrans, matWorld;
    D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
    D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
    matWorld = matScale * matTrans;

    CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
    CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

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

PistolAssualtGun::PistolAssualtGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID)
{
    m_iDrawID = 0;
    m_pAnimation = nullptr;
    m_pNextState = nullptr;
    m_pImageSetting = nullptr;

    m_tInfo.vPos = { _fX, _fY, 0.f };
    m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
    m_tInfo.vDir = { 1.0f, 0.f, 0.f };
    m_tInfo.vLook = { 1.f, 0.f, 0.f };

    m_eImageID = IMAGE::END;
    m_eGunID = _eID;
}

PistolAssualtGun::~PistolAssualtGun()
{
    Release();
}

int PistolAssualtGun::Update(float _fDeltaTime)
{
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    if (CInteractionManager::InteractPlayerItem(pPlayer, this) == true)
    {
        // 상호작용 상태라면 만약에 E를 눌렀을때 총이 샷건으로 바뀌게
        if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_E))
        {
            GET_SINGLE(CPlayerManager)->GetInventory()->GainWeapon(GUN::PISTOL_ASSUALT);
            this->SetIsValid(false);
        }
    }
    return 0;
}

void PistolAssualtGun::Render(const HDC& _hdc)
{
    const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Pistol_Assualt", 0);
    if (nullptr == pTexInfo)
        return;

    float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
    float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

    D3DXMATRIX matScale, matTrans, matWorld;
    D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
    D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
    matWorld = matScale * matTrans;

    CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
    CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

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

PistolHeavyGun::PistolHeavyGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID)
{
    m_iDrawID = 0;
    m_pAnimation = nullptr;
    m_pNextState = nullptr;
    m_pImageSetting = nullptr;

    m_tInfo.vPos = { _fX, _fY, 0.f };
    m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
    m_tInfo.vDir = { 1.0f, 0.f, 0.f };
    m_tInfo.vLook = { 1.f, 0.f, 0.f };

    m_eImageID = IMAGE::END;
    m_eGunID = _eID;
}

PistolHeavyGun::~PistolHeavyGun()
{
    Release();
}

int PistolHeavyGun::Update(float _fDeltaTime)
{
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    if (CInteractionManager::InteractPlayerItem(pPlayer, this) == true)
    {
        // 상호작용 상태라면 만약에 E를 눌렀을때 총이 샷건으로 바뀌게
        if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_E))
        {
            GET_SINGLE(CPlayerManager)->GetInventory()->GainWeapon(GUN::PISTOL_HEAVY);
            this->SetIsValid(false);
        }
    }
    return 0;
}

void PistolHeavyGun::Render(const HDC& _hdc)
{
    const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Pistol_Heavy", 0);
    if (nullptr == pTexInfo)
        return;

    float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
    float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

    D3DXMATRIX matScale, matTrans, matWorld;
    D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
    D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
    matWorld = matScale * matTrans;

    CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
    CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

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

PlasmaGun::PlasmaGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID)
{
    m_iDrawID = 0;
    m_pAnimation = nullptr;
    m_pNextState = nullptr;
    m_pImageSetting = nullptr;

    m_tInfo.vPos = { _fX, _fY, 0.f };
    m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
    m_tInfo.vDir = { 1.0f, 0.f, 0.f };
    m_tInfo.vLook = { 1.f, 0.f, 0.f };

    m_eImageID = IMAGE::END;
    m_eGunID = _eID;
}

PlasmaGun::~PlasmaGun()
{
    Release();
}

int PlasmaGun::Update(float _fDeltaTime)
{
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    if (CInteractionManager::InteractPlayerItem(pPlayer, this) == true)
    {
        // 상호작용 상태라면 만약에 E를 눌렀을때 총이 샷건으로 바뀌게
        if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_E))
        {
            GET_SINGLE(CPlayerManager)->GetInventory()->GainWeapon(GUN::PLASMA);
            this->SetIsValid(false);
        }
    }
    return 0;
}

void PlasmaGun::Render(const HDC& _hdc)
{
    const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Plasma", 0);
    if (nullptr == pTexInfo)
        return;

    float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
    float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

    D3DXMATRIX matScale, matTrans, matWorld;
    D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
    D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
    matWorld = matScale * matTrans;

    CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
    CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

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

ShotGun::ShotGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID)
{
    m_iDrawID = 0;
    m_pAnimation = nullptr;
    m_pNextState = nullptr;
    m_pImageSetting = nullptr;

    m_tInfo.vPos = { _fX, _fY, 0.f };
    m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
    m_tInfo.vDir = { 1.0f, 0.f, 0.f };
    m_tInfo.vLook = { 1.f, 0.f, 0.f };

    m_eImageID = IMAGE::END;
    m_eGunID = _eID;
}

ShotGun::~ShotGun()
{
    Release();
}

int ShotGun::Update(float _fDeltaTime)
{
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    if (CInteractionManager::InteractPlayerItem(pPlayer, this) == true)
    {
        // 상호작용 상태라면 만약에 E를 눌렀을때 총이 샷건으로 바뀌게
        if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_E))
        {
            GET_SINGLE(CPlayerManager)->GetInventory()->GainWeapon(GUN::SHOTGUN);
            this->SetIsValid(false);
        }
    }

    return 0;
}

void ShotGun::Render(const HDC& _hdc)
{
    const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Shotgun", 0);
    if (nullptr == pTexInfo)
        return;

    float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
    float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

    D3DXMATRIX matScale, matTrans, matWorld;
    D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
    D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
    matWorld = matScale * matTrans;

    CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
    CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

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

SniperGun::SniperGun(float _fX, float _fY, float _fWidth, float _fHeight, GUN::ID _eID)
{
    m_iDrawID = 0;
    m_pAnimation = nullptr;
    m_pNextState = nullptr;
    m_pImageSetting = nullptr;

    m_tInfo.vPos = { _fX, _fY, 0.f };
    m_tInfo.vSize = { _fWidth, _fHeight, 0.f };
    m_tInfo.vDir = { 1.0f, 0.f, 0.f };
    m_tInfo.vLook = { 1.f, 0.f, 0.f };

    m_eImageID = IMAGE::END;
    m_eGunID = _eID;
}

SniperGun::~SniperGun()
{
    Release();
}

int SniperGun::Update(float _fDeltaTime)
{
    CObj* pPlayer = GET_SINGLE(CPlayerManager)->GetPlayer();
    if (CInteractionManager::InteractPlayerItem(pPlayer, this) == true)
    {
        // 상호작용 상태라면 만약에 E를 눌렀을때 총이 샷건으로 바뀌게
        if (GET_SINGLE(CKeyManager)->Key_DOWN(KEY_E))
        {
            GET_SINGLE(CPlayerManager)->GetInventory()->GainWeapon(GUN::SNIPER);
            this->SetIsValid(false);
        }
    }

    return 0;
}

void SniperGun::Render(const HDC& _hdc)
{
    const TEXINFO* pTexInfo = CTextureManager::Get_Instance()->GetTextureInfo(L"Weapon", L"Sniper", 0);
    if (nullptr == pTexInfo)
        return;

    float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
    float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

    D3DXMATRIX matScale, matTrans, matWorld;
    D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
    D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
    matWorld = matScale * matTrans;

    CGraphicDevice::Get_Instance()->GetSprite()->SetTransform(&matWorld);
    CGraphicDevice::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

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
