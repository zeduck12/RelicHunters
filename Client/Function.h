#pragma once

template<class T>
void Safe_Delete(T& rObj)
{
	if (rObj)
	{
		delete rObj;
		rObj = nullptr;
	}
}
template<class T>
void Safe_Delete_Array(T& rObj)
{
	if (rObj)
	{
		delete[] rObj;
		rObj = nullptr;
	}
}
template<class T>
void Safe_Release(T& rObj)
{
	if (rObj)
		rObj->Release();
}

float GetRandomFloat(void);
float GetNumberMinBetweenMax(float _fMin, float _fMax);

