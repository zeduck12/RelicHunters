#pragma once
#include "CPlayerInfo.h"
#include "CBossHpBar.h"

class UICameraManager
{
	DECLARE_SINGLETON(UICameraManager)

public:
	bool  Ready(void);
	void  Update(void);
	void  LateUpdate(void);
	void  Render(void);

public:
	shared_ptr<CBossHpBar>& GetBossHpBar(void) { return m_pBossHpBar; }
	void SetBossHpBar(shared_ptr<CBossHpBar>& _pHpBar)
	{
		m_pBossHpBar = _pHpBar;
	}

private:
	UICameraManager() = default;
	~UICameraManager() = default;

private:
	void DrawStageTitle(void);
	void DrawFocusTitle(void);

private:
	int m_iDrawID = 0;
	bool m_bIsFinish = false;
	float m_fStackTime = 0.f;
	float m_fStartTime = 0.f;
	float m_fDeltaY = 0.f;
	wstring m_strName = L"";
	shared_ptr<CBossHpBar> m_pBossHpBar;
	unique_ptr<CPlayerInfo> m_pPlayerInfo;
};

