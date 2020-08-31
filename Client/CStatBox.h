#pragma once
#include "CObj.h"

class CStatBoxButton;
class CSelButton;
class CStatBox :
    public CObj
{
public:
    explicit CStatBox() = default;
    CStatBox(CSelButton* _pOwner);
    virtual ~CStatBox();

public:
    // CObj��(��) ���� ��ӵ�
    virtual void Ready(void) override;
    virtual int  Update(float _fDeltaTime = 0.f) override;
    virtual void LateUpdate(void) override;
    virtual void Release(void) override;
    virtual void Render(const HDC& _hdc) override;

public:
    CSelButton* GetOwner(void) { return m_pOwner; }

public:
    void DrawBackground(void);
    void DrawStatBackground(void);
    void DrawCharacterCard(void);
    void DrawStarBar(void);

public:

    int m_iDrawID;
    float m_fStackTime = 0.f;
    BUTTON::SEL_ID m_eID;
    // ������
    CSelButton* m_pOwner;
    // ��ư�� ��� ����Ʈ
    CStatBoxButton* m_pStartBtn;
    CStatBoxButton* m_pChangeBtn;

};

