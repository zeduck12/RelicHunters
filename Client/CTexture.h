#pragma once
class CTexture
{
public:
	explicit CTexture() = default;
	virtual ~CTexture() = default;
public:
	virtual HRESULT Insert(const wstring& wstrFilePath, const wstring& wstrStateKey = L"",
		const DWORD& rCount = 0) = 0;
	virtual const TEXINFO* GetTexture(const wstring& wstrStateKey = L"", const DWORD& rIndex = 0) = 0;
	virtual void Release(void) = 0;

};

