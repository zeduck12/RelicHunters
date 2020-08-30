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

void CollectGarbageObjects(list<shared_ptr<CObj>>& _list)
{
	auto iter = _list.begin();
	for (; iter != _list.end(); )
	{
		DO_IF_IS_NOT_VALID_OBJ((*iter).get())
		{
			(*iter).reset();
			iter = _list.erase(iter);
		}
		else
		iter++;
	}
}

void CollectGarbageObjects(list<CObj*>& _list)
{
	for (auto& pObj : _list) { DO_IF_IS_NOT_VALID_OBJ(pObj) { Safe_Delete(pObj); } }
	_list.remove_if([](auto& pObj) { return pObj == nullptr; });
}
