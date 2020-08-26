#include "pch.h"
#include "CObj.h"
#include "CAnimation.h"

CObj::CObj()
	:
	m_bIsValid{true},
	m_fSpeed{ 0.f },
	m_fDegree{ 0.f },
	m_eImageID{ IMAGE::END } 
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
}

CObj::~CObj()
{
	if (m_pAnimation)
	{
		delete m_pAnimation;
		m_pAnimation = nullptr;
	}

	for (auto& pTexInfo : m_pVecTextureInfo)
		Safe_Delete(pTexInfo);

	m_pVecTextureInfo.clear();
	m_pVecTextureInfo.shrink_to_fit();

}

CAnimation* CObj::CreateAnimation(const string& _strTag)
{
	if (m_pAnimation)
	{
		delete m_pAnimation;
		m_pAnimation = nullptr;
	}

	m_pAnimation = new CAnimation;

	m_pAnimation->SetTag(_strTag);
	m_pAnimation->SetOwner(this);
	if (!m_pAnimation->Init())
		return nullptr;

	return nullptr;
}

bool CObj::AddAnimationClip(const string& strName, ANIMATION::OPTION eOption, float fAnimationLimitTime, int iFrameMax, int iStart, int iLength, float fOptionLimitTime, const wstring& strObjectKey, const wstring& strStateKey, const TCHAR* pFilePath)
{
	if (!m_pAnimation)
		return false;

	m_pAnimation->AddClip(strName, eOption, fAnimationLimitTime, iFrameMax,
		iStart, iLength, fOptionLimitTime, strObjectKey, strStateKey, pFilePath);

	return true;
}
