#pragma once
#include "framework.h"

typedef struct tagLinePoint
{
	tagLinePoint()
	{
		ZeroMemory(this, sizeof(tagLinePoint));
	}
	tagLinePoint(float _fx, float _fy)
		:fX(_fx), fY(_fy)
	{}
	float fX; 
	float fY; 
}LINEPOS;

typedef struct tagLineInfo
{
	tagLineInfo()
	{
		ZeroMemory(this, sizeof(tagLineInfo)); 
	}
	tagLineInfo(const LINEPOS& rLeftPoint, const LINEPOS& rRightPoint)
		:tLPoint(rLeftPoint)
		, tRPoint(rRightPoint)
	{}
	LINEPOS tLPoint; 
	LINEPOS tRPoint; 
}LINEINFO;