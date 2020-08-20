#include "pch.h"
#include "CTimeManager.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

DEFINITION_SINGLETON(CTimeManager)

void CTimeManager::SetFrame(UINT lock)
{
	if (_isHardware)
	{
		QueryPerformanceCounter((reinterpret_cast<LARGE_INTEGER*>(&_curTime)));
	}
	else
	{
		_curTime = timeGetTime();
	}

	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	if (lock > 0.0f)
	{
		while (_timeElapsed < (1.0f / lock)) //lock은 고정프레임
		{
			if (_isHardware) QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_curTime));
			else _curTime = timeGetTime();
			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}

	_lastTime = _curTime;				//마지막 시간을 기록
	_FPSFrameCount++;					//자동으로 프레임 카운트 증가
	_FPStimeElapsed += _timeElapsed;	//초당 프레임 시간 경과량 증가
	_worldTime += _timeElapsed;			//전체 시간 경과량 증가

	if (_FPStimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPStimeElapsed = 0.0f;
	}
}

CTimeManager::CTimeManager()
{
	__int64 _periodFrequency;
	if (QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_periodFrequency)))
	{
		_isHardware = true;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_lastTime));
		_timeScale = 1.0f / _periodFrequency;
	}
	else
	{
		_isHardware = false;
		_lastTime = timeGetTime();
		_timeScale = 0.001f;
	}

	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPStimeElapsed = 0.0f;
	_worldTime = 0;
}

CTimeManager::~CTimeManager()
{
}
