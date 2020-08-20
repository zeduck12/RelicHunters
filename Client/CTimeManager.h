#pragma once
class CTimeManager
{
public:
	DECLARE_SINGLETON(CTimeManager)

public:
	void SetFrame(UINT lock = 60);						  //초기화 하지 않으면 프레임은 60프레임으로 고정
	float GetElapsedTime() const { return _timeElapsed; } //한 프레임 경과시간 가져오기 1/60
	float GetWorldTime() const { return _worldTime; }	  //전체 시간 경과량 가져오기
	unsigned long GetFrameRate() { return _frameRate; }   //현재 FPS 가져오기

private:
	CTimeManager();
	~CTimeManager();

private:
	//FPS:초당 프레임 수(frames per second)
	bool _isHardware;				//고성능 타이머를 지원하냐?
	__int64 _curTime;				//현재시간
	__int64 _lastTime;				//마지막시간

	float _FPStimeElapsed;			//FPS 마지막시간과 현재 시간의 경과량
	float _timeScale;				//시간 경과량
	DWORD _FPSFrameCount;			//FPS 카운트
	//리턴값
	DWORD _frameRate;				//FPS
	float _timeElapsed;				//한 프레임당 경과량
	float _worldTime;
};

