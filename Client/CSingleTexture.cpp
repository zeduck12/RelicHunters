#include "pch.h"
#include "CSingleTexture.h"
#include "CGraphicDevice.h"

CSingleTexture::~CSingleTexture()
{
    Release();
}

HRESULT CSingleTexture::Insert(const wstring& wstrFilePath, const wstring& wstrStateKey, const DWORD& rCount)
{
    m_pTextureInfo = new TEXINFO;
    ZeroMemory(m_pTextureInfo, sizeof(TEXINFO));

    if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_pTextureInfo->tImageInfo)))
    {
        ERR_MSG(L"이미지 정보 불러오기 실패");
        return E_FAIL;
    }

    if (FAILED(D3DXCreateTextureFromFileEx(
        CGraphicDevice::Get_Instance()->GetDevice(),
        wstrFilePath.c_str(),
        m_pTextureInfo->tImageInfo.Width,
        m_pTextureInfo->tImageInfo.Height,
        m_pTextureInfo->tImageInfo.MipLevels,
        0,
        m_pTextureInfo->tImageInfo.Format,
        D3DPOOL_MANAGED,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        0,
        nullptr,
        nullptr,
        &m_pTextureInfo->pTexture)))
    {
        wstring wstrErrMessage = wstrFilePath + L"Create Texture Failed";
        ERR_MSG(wstrErrMessage.c_str());
        return E_FAIL;
    }

    return S_OK;
}

const TEXINFO* CSingleTexture::GetTexture(const wstring& wstrStateKey, const DWORD& rIndex)
{
    return m_pTextureInfo;
}

void CSingleTexture::Release(void)
{
    SAFE_DELETE(m_pTextureInfo);
}
