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
	unique_ptr<CPlayerInfo> m_pPlayerInfo;

};

