#pragma once
#include "CTexture.h"
class CMultiTexture :
    public CTexture
{
public:
    explicit CMultiTexture() = default;
    virtual ~CMultiTexture();

public:
    // CTexture��(��) ���� ��ӵ�
    virtual HRESULT Insert(const wstring& wstrFilePath, const wstring& wstrStateKey = L"", const DWORD& rCount = 0) override;
    virtual const TEXINFO* GetTexture(const wstring& wstrStateKey = L"", const DWORD& rIndex = 0) override;
    virtual void Release(void) override;

private:
    // StateKey 
    // �ϳ��� STateKey(�ൿ)�� �������� �׸��� �� �� �ֱ� ����. 
    // ���� ��� �����̶� StateKey ���� 20���� �׸��� �ְ� 
    //���� �ε����� ������ �������� ��ġ �ִϸ��̼� ���ư��� ��� �� ��. 
    map<wstring, vector<TEXINFO*>> m_mapMultiTexture;
};

