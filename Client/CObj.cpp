#include "pch.h"
#include "CObj.h"

CObj::CObj()
	:
	m_bIsValid{true},
	m_fSpeed{ 0.f },
	m_fDegree{ 0.f }
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
}

CObj::~CObj()
{
}
