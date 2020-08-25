#include "pch.h"
#include "CTextureManager.h"
#include "CTexture.h"
#include "CSingleTexture.h"
#include "CMultiTexture.h"

DEFINITION_SINGLETON(CTextureManager)

CTextureManager::~CTextureManager()
{
	Release();
}

void CTextureManager::Release(void)
{
	for (auto& rPair : m_mapTexture)
		Safe_Delete(rPair.second);
	m_mapTexture.clear();
}

const TEXINFO* CTextureManager::GetTextureInfo(const wstring& wstrObjectKey, const wstring& wstrStateKey, const DWORD& dwImgIndex)
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);

	if (m_mapTexture.end() == iter_find)
		return nullptr;
	return iter_find->second->GetTexture(wstrStateKey, dwImgIndex);
}

const vector<TEXINFO*> CTextureManager::GetVecTextureInfo(const wstring& wstrObjectKey, const wstring& wstrStateKey)
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);

	if (m_mapTexture.end() == iter_find)
		return vector<TEXINFO*>();

	return dynamic_cast<CMultiTexture*>(iter_find->second)->GetStateTexture(wstrStateKey);
}

HRESULT CTextureManager::Insert(const TEX_ID& eTexID, const wstring& wstrFilePath, const wstring& wstrObjectKey, const wstring& wstrStateKey, const DWORD& dwImgIndex)
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);

	if (iter_find == m_mapTexture.end())
	{
		CTexture* pTexture = nullptr;
		switch (eTexID)
		{
		case CTextureManager::TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;
		case CTextureManager::TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		case CTextureManager::TEXEND:
			break;
		default:
			break;
		}
		if (FAILED(pTexture->Insert(wstrFilePath, wstrStateKey, dwImgIndex)))
		{
			ERR_MSG(L"Texture Creating Failed");
			return E_FAIL;
		}
		m_mapTexture.emplace(wstrObjectKey, pTexture);
	}
	else if (TEX_MULTI == eTexID)
	{
		if (FAILED(m_mapTexture[wstrObjectKey]->Insert(wstrFilePath, wstrStateKey, dwImgIndex)))
		{
			ERR_MSG(L"MultiTexture Creating Failed");
			return E_FAIL;
		}
	}
	return S_OK;
}
