#pragma once

typedef struct tagAnimationClip
{
	ANIMATION::OPTION	eOption;
	vector<TEXINFO*>	vecTexture; // 이미지를 들고 있음

	float fAnimationTime;
	float fAnimationLimitTime;
	float fAnimationFrameTime;
	int iFrame;
	int iFrameMax;
	int iStart;
	int iLength;
	float fOptionTime;
	float fOptionLimitTime;
}ANIMATION_CLIP;

class CAnimation
{
private:
	friend class CObj;

private:
	CAnimation() = default;
	CAnimation(const CAnimation& _rOther);
	~CAnimation();

private:
	map<string, ANIMATION_CLIP*> m_mapClip;
	string m_strTag;

	ANIMATION_CLIP* m_pCurClip = nullptr;
	string			m_strCurClip;
	string			m_strDefaultClip;
	CObj*			m_pOwner = nullptr;

public:
	ANIMATION_CLIP* GetCurrentClip() const { return m_pCurClip; }

	void SetOwner(CObj* _pObj) { m_pOwner = _pObj; }
	void SetCurrentClip(const string& _strCurClip);
	void SetDefaultClip(const string& _strDefaultClip);
	void ChangeClip(const string& _strClip);
	ANIMATION_CLIP* FindClip(const string& _strName);

	bool Init(void);
	void Update(float _fTime);
	CAnimation* Clone(void);

public:
	void SetTag(const string& _rStrTag) { m_strTag = _rStrTag; }
	bool AddClip(const string& strName,
		ANIMATION::OPTION eOption, float fAnimationLimitTime,
		int iFrameMax, int iStart, int iLength, float fOptionLimitTime,
		const wstring& strObjectKey, const wstring& strStateKey, const TCHAR* pFilePath);

};

