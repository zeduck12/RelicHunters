#pragma once
#include "CPlayerInfo.h"
#include "CBossHpBar.h"
#include "CCard.h"

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
	bool IsShowKeyGuide(void) const { return m_bIsShowKeyGuide; }
	void SetIsShowKeyGuide(bool _bIsValue) { m_bIsShowKeyGuide = _bIsValue; }
	bool IsCardGameClear(void) const { return m_bIsCardGameClear; }
	void SetIsCardGameClear(bool _bIsValue) { m_bIsCardGameClear = _bIsValue; }

private:
	UICameraManager() = default;
	~UICameraManager() = default;

private:
	void DrawStageTitle(void);
	void DrawFocusTitle(void);
	/// ±Ã±Ø±â Àü¿ë
	void DrawSpecialFocus(void);
	void DrawSpecialCard(void);
	void DrawSpecialMidCard(void);
	void DrawSpecialText(void);
	void DrawDescBoard(void);

public:
	void DrawCardGameBoard(void);
	void DrawCards(void);
	void DrawCardGameText(void);
	void DrawClearText(void);
	void ReadyCards(void);
	void ReadyCards2(void);
	void ReadyCards3(void);

public:
	// Ä«µå¹¶Ä¡ ³Ñ±â±â
	list<shared_ptr<CCard>>& GetCards(void) { return m_listCards; }

private:
	int   m_iDrawID = 0;
	float m_fDeltaY = 0.f;
	float m_fDeltaX = 0.f;
	float m_fStackTime = 0.f;
	float m_fStartTime = 0.f;
	float m_fSpecialCheckTime = 0.f;
	float m_fSpecialCoolTime = 0.f;
	float m_fCardCheckTime = 0.f;
	bool  m_bIsFinish       = false;
	bool  m_bIsShowMiniMap  = false;
	bool  m_bIsShowKeyGuide = false;
	bool  m_bIsCardGameClear;
	bool  m_bIsPlayingSFX   = false;

	float m_fMoveX = 0.f;
	float m_fClearCoolTime = 0.f;
	float m_fClearCheckTime = 0.f;

	wstring m_strName = L"";
	shared_ptr<CBossHpBar> m_pBossHpBar;
	unique_ptr<CPlayerInfo> m_pPlayerInfo;

private:
	list<shared_ptr<CCard>> m_listCards;
};

