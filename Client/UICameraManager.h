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

public:
	void RenderMiniMap(void);
	void DrawPlayer(void);
	void Set_Ratio(D3DXMATRIX& matWorld, const float& fRatioX, const float& fRatioY);
	bool IsShowMiniMap(void) const { return m_bIsShowMiniMap; }
	void SetIsShowMiniMap(bool _bIsShowMiniMap) { m_bIsShowMiniMap = _bIsShowMiniMap; }

private:
	UICameraManager() = default;
	~UICameraManager() = default;

private:
	void DrawStageTitle(void);
	void DrawFocusTitle(void);

private:
	int   m_iDrawID = 0;
	float m_fDeltaY = 0.f;
	float m_fStackTime = 0.f;
	float m_fStartTime = 0.f;
	bool m_bIsFinish      = false;
	bool m_bIsShowMiniMap = false;

	wstring m_strName = L"";
	shared_ptr<CBossHpBar> m_pBossHpBar;
	unique_ptr<CPlayerInfo> m_pPlayerInfo;
};

