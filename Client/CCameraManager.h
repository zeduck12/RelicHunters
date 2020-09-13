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
	const D3DXVECTOR3& GetFocusPos(void) const { return m_vFocusPos; }
	float GetCameraScale(void) const { return m_fScale; }
	float GetCameraDeltaX(void) const { return m_fDeltaX; }
	float GetCameraDeltaY(void) const { return m_fDeltaY; }

	bool IsBossDeath(void) const { return m_bIsBossDeath; }
	bool IsPressing(void) const { return m_bIsPressing; }
	bool IsReduceScale(void) const { return m_bIsReduceScale; }
	bool IsEnlargeScale(void) const { return m_bIsEnlargeScale; }
	bool IsShooting(void) const { return m_bIsShooting; }
	
	void SetIsBossDeath(bool _bIsBossDeath) { m_bIsBossDeath = _bIsBossDeath; }
	void SetIsPressing(bool _bIsPressing) { m_bIsPressing = _bIsPressing; }
	void SetIsReduceScale(bool _bIsReduce) { m_bIsReduceScale = _bIsReduce; }
	void SetIsEnlargeScale(bool _bIsEnLarge) { m_bIsEnlargeScale = _bIsEnLarge; }
	void SetIsShooting(bool _bIsShooting) { m_bIsShooting = _bIsShooting; }

	// 데시 전용
	bool IsFocusPlayer(void) const { return m_bIsFocusPlayer; }
	bool IsPressingDash(void) const { return m_bIsPressingDash; }
	bool IsReduceScaleDash(void) const { return m_bIsReduceScaleDash; }
	bool IsEnlargeScaleDash(void) const { return m_bIsEnlargeScaleDash; }

	void SetIsPressingDash(bool _bIsPressing) { m_bIsPressingDash = _bIsPressing; }
	void SetIsReduceScaleDash(bool _bIsReduce) { m_bIsReduceScaleDash = _bIsReduce; }
	void SetIsEnlargeScaleDash(bool _bIsEnLarge) { m_bIsEnlargeScaleDash = _bIsEnLarge; }

public:
	// Scale 관련
	void ToEnlargeScale(void);  // 확대
	void ToReduceScale(void);	// 축소

	void ToEnlargeScaleDash(void);  // 확대
	void ToReduceScaleDash(void);	// 축소

public:

	void MoveCameraToFreeSpace(void);  // 조금더 카메라 움직이게
	void EarthquakeCamera(void);	   // 카메라 진동
	void EarthquakeCamera2(void);

private:
	CCameraManager();
	~CCameraManager() = default;


private:

	XFORM m_fxWorld;

	D3DXVECTOR3 m_vFocusPos;
	// 카메라 진동 관련 변수
	int m_iCount;
	int m_iStack = 0;
	float m_fOldDelta = 0.f;
	float m_fStackTime = 0.f;
	float m_fFocusTime = 0.f;
	float m_fFocusStartTime = 0.f;

	float m_fDeltaX;
	float m_fDeltaY;
	float m_fScale;

	// bool 변수
	bool m_bIsShooting;
	bool m_bIsEnlargeScale;
	bool m_bIsReduceScale;
	bool m_bIsPressing;
	bool m_bIsBossDeath;

	bool m_bIsEnlargeScaleDash;
	bool m_bIsReduceScaleDash;
	bool m_bIsPressingDash;

	bool m_bIsFocusPlayer = true;

};

