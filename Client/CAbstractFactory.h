#pragma once
#include "CObj.h"
#include <memory>

template<typename T>
static std::unique_ptr<CObj> CreateUnique()
{
	std::unique_ptr<CObj> pObj = std::make_unique<T>();
	pObj->Ready();
	return std::move(pObj);
}

template<typename T>
static CObj* Create()
{
	CObj* pObj = new T;
	pObj->Ready();
	return pObj;
}