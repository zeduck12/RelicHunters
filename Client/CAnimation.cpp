#include "pch.h"
#include "CAnimation.h"
#include "CTextureManager.h"

CAnimation::CAnimation(const CAnimation& _rOther)
{
	*this = _rOther;

	// 기존 클립정보 날려줌
	m_mapClip.clear();
	map<string, ANIMATION_CLIP*>::const_iterator iter;
	map<string, ANIMATION_CLIP*>::const_iterator iterEnd = _rOther.m_mapClip.end();

	for (iter = _rOther.m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		ANIMATION_CLIP* pClip = new ANIMATION_CLIP;

		*pClip = *iter->second;
	}

	m_pCurClip = nullptr;
	m_strCurClip = "";
	SetCurrentClip(_rOther.m_strCurClip);
}

CAnimation::~CAnimation()
{
	for (auto& rPair : m_mapClip)
		Safe_Delete(rPair.second);
	m_mapClip.clear();
}

void CAnimation::SetCurrentClip(const string& _strCurClip)
{
	ChangeClip(_strCurClip);
}

void CAnimation::SetDefaultClip(const string& _strDefaultClip)
{
	m_strDefaultClip = _strDefaultClip;
}

void CAnimation::ChangeClip(const string& _strClip)
{
	if (m_strCurClip == _strClip)
		return;

	m_strCurClip = _strClip;

	if (m_pCurClip)
	{
		m_pCurClip->iFrame = m_pCurClip->iStart;
		m_pCurClip->fAnimationTime = 0.f;
		m_pCurClip->fOptionTime = 0.f;
	}

	m_pCurClip = FindClip(_strClip);

	m_pOwner->SetTextureInfo(m_pCurClip->vecTexture);
}

ANIMATION_CLIP* CAnimation::FindClip(const string& _strName)
{
	map<string, ANIMATION_CLIP*>::iterator iter = m_mapClip.find(_strName);

	if (iter == m_mapClip.end())
		return nullptr;

	return iter->second;
}

bool CAnimation::Init(void)
{
	return true;
}

void CAnimation::Update(float _fTime)
{
	m_pCurClip->fAnimationTime += _fTime;

	while (m_pCurClip->fAnimationTime >= m_pCurClip->fAnimationFrameTime)
	{
		m_pCurClip->fAnimationTime -= m_pCurClip->fAnimationFrameTime;
		++m_pCurClip->iFrame;

		if (m_pCurClip->iFrame - m_pCurClip->iStart == m_pCurClip->iLength)
		{
			m_pCurClip->iFrame = m_pCurClip->iStart;

			// 전부다 돌았다면 애니매이션을
			switch (m_pCurClip->eOption)
			{
			case ANIMATION::ONCE_RETURN:
				ChangeClip(m_strDefaultClip);
				break;
			case ANIMATION::ONCE_DESTROY:
				m_pOwner->SetIsValid(false);
				break;
			case ANIMATION::TIME_RETURN:
				break;
			case ANIMATION::TIME_DESTROY:
				break;
			}
		}
	}
}

CAnimation* CAnimation::Clone(void)
{
	return new CAnimation(*this);
}

bool CAnimation::AddClip(const string& strName, ANIMATION::OPTION eOption, float fAnimationLimitTime, 
	int iFrameMax, int iStart, int iLength, float fOptionLimitTime,
	const wstring& strObjectKey, const wstring& strStateKey, const TCHAR* pFilePath)
{
	ANIMATION_CLIP* pClip = new ANIMATION_CLIP;

	pClip->eOption = eOption;
	pClip->fAnimationLimitTime = fAnimationLimitTime;
	pClip->iFrameMax = iFrameMax;
	pClip->iStart = iStart;
	pClip->iLength = iLength;
	pClip->fOptionLimitTime = fOptionLimitTime;
	pClip->fAnimationFrameTime = fAnimationLimitTime / iLength; // 한사진당 머무르는 시간.

	if (E_FAIL == FAILED(CTextureManager::Get_Instance()->Insert(CTextureManager::TEX_MULTI, pFilePath, strObjectKey, strStateKey, iLength)))
		return false;

	const vector<TEXINFO*> vecTexture = CTextureManager::Get_Instance()->GetVecTextureInfo(strObjectKey, strStateKey);

	// 로드한 텍스쳐 넣어주기.
	pClip->vecTexture = vecTexture;

	pClip->fAnimationTime = 0.f;
	pClip->iFrame = iStart;
	pClip->fOptionTime = 0.f;

	m_mapClip.insert(make_pair(strName, pClip));

	if (m_strDefaultClip.empty())
		SetDefaultClip(strName);

	if (m_strCurClip.empty())
		SetCurrentClip(strName);

	return true;
}
