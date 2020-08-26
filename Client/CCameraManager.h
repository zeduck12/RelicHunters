#pragma once
class CCameraManager
{
public:
	DECLARE_SINGLETON(CCameraManager)

public:
	bool  Ready(void);
	void  Update(void);
	void  LateUpdate(void);

	XFORM GetWorldMatrix(void); // 모든 카메라 움직임 Matrix 반환
	D3DXMATRIX GetWorldD3DMatrix(void);

public:

	float GetCameraDeltaX(void) const { return m_fDeltaX; }
	float GetCameraDeltaY(void) const { return m_fDeltaY; }

	bool IsPressing(void) const { return m_bIsPressing; }
	bool IsReduceScale(void) const { return m_bIsReduceScale; }
	bool IsEnlargeScale(void) const { return m_bIsEnlargeScale; }
	bool IsShooting(void) const { return m_bIsShooting; }
	
	void SetIsPressing(bool _bIsPressing) { m_bIsPressing = _bIsPressing; }
	void SetIsReduceScale(bool _bIsReduce) { m_bIsReduceScale = _bIsReduce; }
	void SetIsEnlargeScale(bool _bIsEnLarge) { m_bIsEnlargeScale = _bIsEnLarge; }
	void SetIsShooting(bool _bIsShooting) { m_bIsShooting = _bIsShooting; }

public:
	// Scale 관련
	void ToEnlargeScale(void);  // 확대
	void ToReduceScale(void);	// 축소
public:

	void MoveCameraToFreeSpace(void);  // 조금더 카메라 움직이게
	void EarthquakeCamera(void);	   // 카메라 진동

private:
	CCameraManager();
	~CCameraManager() = default;


private:

	XFORM m_fxWorld;

	// 카메라 진동 관련 변수
	int m_iCount;

	float m_fDeltaX;
	float m_fDeltaY;
	float m_fScale;

	// bool 변수
	bool m_bIsShooting;
	bool m_bIsEnlargeScale;
	bool m_bIsReduceScale;
	bool m_bIsPressing;


};

