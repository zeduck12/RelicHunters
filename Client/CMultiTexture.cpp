#include "pch.h"
#include "CMultiTexture.h"
#include "CGraphicDevice.h"

CMultiTexture::~CMultiTexture()
{
    Release();
}

HRESULT CMultiTexture::Insert(const wstring& wstrFilePath, const wstring& wstrStateKey , const DWORD& rCount )
{
    auto iter_find = m_mapMultiTexture.find(wstrStateKey);
    TCHAR szFilePath[MAX_PATH] = L"";
    if (m_mapMultiTexture.end() == iter_find)
    {
        for (DWORD i = 0; i < rCount ; i++)
        {
            wsprintf(szFilePath, wstrFilePath.c_str(), i);
            TEXINFO* pTextureInfo = new TEXINFO;
            ZeroMemory(pTextureInfo, sizeof(TEXINFO));

            if (FAILED(D3DXGetImageInfoFromFile(szFilePath, &pTextureInfo->tImageInfo)))
            {
                ERR_MSG(L"이미지 정보 불러오기 실패");
                Safe_Delete(pTextureInfo);
                return E_FAIL;
            }

            if (FAILED(D3DXCreateTextureFromFileEx(
                CGraphicDevice::Get_Instance()->GetDevice(),
                szFilePath,
                pTextureInfo->tImageInfo.Width,
                pTextureInfo->tImageInfo.Height,
                pTextureInfo->tImageInfo.MipLevels,
                0,
                pTextureInfo->tImageInfo.Format,
                D3DPOOL_MANAGED,
                D3DX_DEFAULT,
                D3DX_DEFAULT,
                0,
                nullptr,
                nullptr,
                &pTextureInfo->pTexture)))
            {
                wstring wstrErrMessage = wstrFilePath + L"Create Texture Failed";
                ERR_MSG(wstrErrMessage.c_str());
                Safe_Delete(pTextureInfo);
                return E_FAIL;
            }

            m_mapMultiTexture[wstrStateKey].emplace_back(pTextureInfo);

        }
    }

    return S_OK;
}

const TEXINFO* CMultiTexture::GetTexture(const wstring& wstrStateKey , const DWORD& rIndex )
{
    auto iter_Find = m_mapMultiTexture.find(wstrStateKey);
    if (iter_Find == m_mapMultiTexture.end())
        return nullptr;

    return iter_Find->second[rIndex];
}

void CMultiTexture::Release(void)
{
    for (auto& rPair : m_mapMultiTexture)
    {
        for (auto& pTexInfo : rPair.second)
            Safe_Delete(pTexInfo);

        rPair.second.clear();
        rPair.second.shrink_to_fit(); 
    }
}

vector<TEXINFO*> CMultiTexture::GetStateTexture(const wstring& wstrStateKey)
{
    // TODO: 여기에 return 문을 삽입합니다.
    auto iter_Find = m_mapMultiTexture.find(wstrStateKey);
    if (iter_Find == m_mapMultiTexture.end())
        return vector<TEXINFO*>();

    return iter_Find->second;
}
