#pragma once
#include "CPlayerInfo.h"

class UICameraManager
{
	DECLARE_SINGLETON(UICameraManager)

public:
	bool  Ready(void);
	void  Update(void);
	void  LateUpdate(void);
	void  Render(void);

private:
	UICameraManager() = default;
	~UICameraManager() = default;

private:
	void DrawStageTitle(void);

private:
	int m_iDrawID = 0;
	bool m_bIsFinish = false;
	float m_fStackTime = 0.f;
	float m_fStartTime = 0.f;
	wstring m_strName = L"";
	unique_ptr<CPlayerInfo> m_pPlayerInfo;

};

