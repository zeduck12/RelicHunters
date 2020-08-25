#pragma once

class CTexture;
class CTextureManager
{
public:
	DECLARE_SINGLETON(CTextureManager)

public:
	enum TEX_ID { TEX_SINGLE, TEX_MULTI, TEXEND };

private:
	CTextureManager() = default;
	~CTextureManager();

public:
	void Release(void);

public:
	const TEXINFO* GetTextureInfo(const wstring& wstrObjectKey, const wstring& wstrStateKey = L"", const DWORD& dwImgIndex = 0);
	HRESULT Insert(const TEX_ID& eTexID, const wstring& wstrFilePath, const wstring& wstrObjectKey, const wstring& wstrStateKey = L"", const DWORD& dwImgIndex = 0);

private:
	map<wstring, CTexture*> m_mapTexture;

};

