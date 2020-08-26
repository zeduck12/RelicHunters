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

typedef struct tagTile
{
	D3DXVECTOR3	vPos;		// 위치
	D3DXVECTOR3	vSize;		// 크기
	int	iDrawID;	// 드로우 아이디
	int	iOption;	// 옵션
	int	iFrameX;	// 출력 영역 X
	int	iFrameY;	// 출력 영역 Y
}TILE;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9 pTexture; // 이미지 한장을 제어하기 위한 컴객체. 
	D3DXIMAGE_INFO tImageInfo;   //이미지의 정보를 담아둘 구조체. 

}TEXINFO;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;  // 위치벡터 
	D3DXVECTOR3 vDir;  // 방향벡터 
	D3DXVECTOR3 vSize; // 크기 표현할 놈.
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vImageSize;
	D3DXMATRIX  mat;

}INFO;
