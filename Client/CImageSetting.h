#pragma once
#include "CObj.h"

class CImageSetting
{
public:
	explicit CImageSetting() = default;
	CImageSetting(CObj* _pOwner, const string& _rStrTag );
	~CImageSetting() = default;

public:
	bool Ready(void);

private:
	CObj* m_pOwner;
	string m_strTag;
	IMAGE::ID m_eImageID;

};

