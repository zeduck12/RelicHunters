#include "pch.h"
#include "Function.h"

float GetRandomFloat(void)
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

float GetNumberMinBetweenMax(float _fMin, float _fMax)
{
	if (_fMin > _fMax) abort();
	float fT = GetRandomFloat();
	return _fMin * (1.f - fT) + _fMax * fT;
}
