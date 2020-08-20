#pragma once
class CTimeManager
{
public:
	DECLARE_SINGLETON(CTimeManager)

public:
	void SetFrame(UINT lock = 60);						  //�ʱ�ȭ ���� ������ �������� 60���������� ����
	float GetElapsedTime() const { return _timeElapsed; } //�� ������ ����ð� �������� 1/60
	float GetWorldTime() const { return _worldTime; }	  //��ü �ð� ����� ��������
	unsigned long GetFrameRate() { return _frameRate; }   //���� FPS ��������

private:
	CTimeManager();
	~CTimeManager();

private:
	//FPS:�ʴ� ������ ��(frames per second)
	bool _isHardware;				//���� Ÿ�̸Ӹ� �����ϳ�?
	__int64 _curTime;				//����ð�
	__int64 _lastTime;				//�������ð�

	float _FPStimeElapsed;			//FPS �������ð��� ���� �ð��� �����
	float _timeScale;				//�ð� �����
	DWORD _FPSFrameCount;			//FPS ī��Ʈ
	//���ϰ�
	DWORD _frameRate;				//FPS
	float _timeElapsed;				//�� �����Ӵ� �����
	float _worldTime;
};

