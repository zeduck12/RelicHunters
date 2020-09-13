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

template<typename T>
void DeleteListSafe(list<T>& _list) 
{
	for (auto& pObj : _list) { Safe_Delete(pObj); }
	_list.clear();
}

template<typename T>
void DeleteListSafe(list<shared_ptr<T>>& _list)
{
	for (auto& pObj : _list) { pObj.reset(); }
	_list.clear();
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

void CollectGarbageObjects(list<CObj*>& _list);
void CollectGarbageObjects(list<shared_ptr<CObj>>& _list);


