#pragma once
#include "CTexture.h"
class CSingleTexture :
    public CTexture
{
public:
    explicit CSingleTexture() = default;
    virtual ~CSingleTexture();

public:
    // CTexture을(를) 통해 상속됨
    virtual HRESULT Insert(const wstring& wstrFilePath, const wstring& wstrStateKey = L"", const DWORD& rCount = 0) override;
    virtual const TEXINFO* GetTexture(const wstring& wstrStateKey = L"", const DWORD& rIndex = 0) override;
    virtual void Release(void) override;

public:
    TEXINFO* m_pTextureInfo;
};

