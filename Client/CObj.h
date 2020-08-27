#pragma once
#include "Struct.h"

class CAnimation;
class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Ready(void)					 = 0;
	virtual int  Update(float _fDeltaTime = 0.f) = 0;
	virtual void LateUpdate(void)				 = 0;
	virtual void Release(void)					 = 0;
	virtual void Render(const HDC& _hdc)		 = 0;

public:
	INFO* GetInfo(void) { return &m_tInfo ; }
	void  SetInfo(const INFO& _rOther) { m_tInfo = _rOther; }
	RECT  GetRect(void) const
	{ 
		RECT rc =
		{
			LONG(m_tInfo.vPos.x - (m_tInfo.vSize.x * 0.5f)),
			LONG(m_tInfo.vPos.y - (m_tInfo.vSize.y * 0.5f)),
			LONG(m_tInfo.vPos.x + (m_tInfo.vSize.x * 0.5f)),
			LONG(m_tInfo.vPos.y + (m_tInfo.vSize.y * 0.5f))
		};
		return rc;
	}

	int GetLeft(void)     const { return static_cast<int>(m_tInfo.vPos.x - (m_tInfo.vSize.x * 0.5f)); }
	int GetTop(void)	  const { return static_cast<int>(m_tInfo.vPos.y - (m_tInfo.vSize.y * 0.5f)); }
	int GetRight(void)    const { return static_cast<int>(m_tInfo.vPos.x + (m_tInfo.vSize.x * 0.5f)); }
	int GetBottom(void)   const { return static_cast<int>(m_tInfo.vPos.y + (m_tInfo.vSize.y * 0.5f)); }
	float GetWidth(void)  const { return m_tInfo.vSize.x; }
	float GetHeight(void) const { return m_tInfo.vSize.y; }
	void SetWidth(float _fWidth)   { m_tInfo.vSize.x = _fWidth ; }
	void SetHeight(float _fHeight) { m_tInfo.vSize.y = _fHeight; }
	float GetX(void) const { return m_tInfo.vPos.x; }
	float GetY(void) const { return m_tInfo.vPos.y; }
	void SetX(float _fX) { m_tInfo.vPos.x = _fX; }
	void SetY(float _fY) { m_tInfo.vPos.y = _fY; }
	bool IsValid(void) const { return m_bIsValid; }
	void SetIsValid(bool _bIsValid) { m_bIsValid = _bIsValid ; }
	float GetSpeed(void) const { return m_fSpeed ; }
	void  SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed ; }
	float GetDegree(void) const { return m_fDegree ; }
	void  SetDegree(float _fDegree) { m_fDegree = _fDegree ; }
	D3DXVECTOR3 GetPosVector(void) const { return m_tInfo.vPos; }
	void SetPosVector(D3DXVECTOR3 _rVecPos) { m_tInfo.vPos = _rVecPos; }
	IMAGE::ID GetImageID(void) const { return m_eImageID; }


	DIRECTION::ID GetDirection(void) const { return m_eDir; }
	void SetDirection(DIRECTION::ID _eDir) { m_eDir = _eDir; }


public:
	CAnimation* CreateAnimation(const string& _strTag);
	bool AddAnimationClip(const string& strName,
		ANIMATION::OPTION eOption, float fAnimationLimitTime,
		int iFrameMax, int iStart,
		int iLength, float fOptionLimitTime,
		const wstring& strObjectKey, const wstring& strStateKey, const TCHAR* pFilePath);

	CAnimation* GetAnimation(void) const { return m_pAnimation; }

public:
	void SetTextureInfo(vector<TEXINFO*> _pVecTexInfo) { m_pVecTextureInfo = _pVecTexInfo; }
	vector<TEXINFO*> GetTextureInfo(void) const { return m_pVecTextureInfo; }

protected:
	// 현재 방향
	DIRECTION::ID m_eDir;

	INFO m_tInfo;
	bool m_bIsValid;
	float m_fSpeed;
	float m_fDegree;

	// 이미지 셋팅 ID
	IMAGE::ID	  m_eImageID;

	CAnimation* m_pAnimation    = nullptr;
	vector<TEXINFO*>    m_pVecTextureInfo ;

};

