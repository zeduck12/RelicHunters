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
		while (_timeElapsed < (1.0f / lock)) //lock�� ����������
		{
			if (_isHardware) QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_curTime));
			else _curTime = timeGetTime();
			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}

	_lastTime = _curTime;				//������ �ð��� ���
	_FPSFrameCount++;					//�ڵ����� ������ ī��Ʈ ����
	_FPStimeElapsed += _timeElapsed;	//�ʴ� ������ �ð� ����� ����
	_worldTime += _timeElapsed;			//��ü �ð� ����� ����

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
