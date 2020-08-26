#pragma once
class CCameraManager
{
public:
	DECLARE_SINGLETON(CCameraManager)

public:
	bool  Ready(void);
	void  Update(void);
	void  LateUpdate(void);

	XFORM GetWorldMatrix(void); // ��� ī�޶� ������ Matrix ��ȯ
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
	// Scale ����
	void ToEnlargeScale(void);  // Ȯ��
	void ToReduceScale(void);	// ���
public:

	void MoveCameraToFreeSpace(void);  // ���ݴ� ī�޶� �����̰�
	void EarthquakeCamera(void);	   // ī�޶� ����

private:
	CCameraManager();
	~CCameraManager() = default;


private:

	XFORM m_fxWorld;

	// ī�޶� ���� ���� ����
	int m_iCount;

	float m_fDeltaX;
	float m_fDeltaY;
	float m_fScale;

	// bool ����
	bool m_bIsShooting;
	bool m_bIsEnlargeScale;
	bool m_bIsReduceScale;
	bool m_bIsPressing;


};

