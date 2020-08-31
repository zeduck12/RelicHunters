#pragma once
#include "CScene.h"
class CMenu :
    public CScene
{
public:
    CMenu();
    virtual ~CMenu();

public:
    // CScene��(��) ���� ��ӵ�
    virtual bool Ready(void) override;
    virtual void Update(void) override;
    virtual void LateUpdate(void) override;
    virtual void Render(const HDC& _hdc) override;
    virtual void Release(void) override;

public:
    list<CObj*>& GetButtons(void) { return m_listButtons; }

public:
    void DrawBackgorund(void);
    void DrawSelBackground(void);
    void DrawSceneTitle(void);


private:
    int m_iDrawID;
    float m_fStackTime;

    list<CObj*> m_listButtons;
    list<shared_ptr<CObj>> m_listStars; // ��� �ٸ��ִ� ����
};

