#pragma once
#include "CTexture.h"
class CSingleTexture :
    public CTexture
{
public:
    explicit CSingleTexture() = default;
    virtual ~CSingleTexture();

public:
    // CTexture��(��) ���� ��ӵ�
    virtual HRESULT Insert(const wstring& wstrFilePath, const wstring& wstrStateKey = L"", const DWORD& rCount = 0) override;
    virtual const TEXINFO* GetTexture(const wstring& wstrStateKey = L"", const DWORD& rIndex = 0) override;
    virtual void Release(void) override;

public:
    TEXINFO* m_pTextureInfo;
};

