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

	D3DXVECTOR3 m_vRealVertex[4];
	
public:
	void Ready()
	{
		m_vRealVertex[0].x = vPos.x -(128.f * 0.5f);
		m_vRealVertex[0].y = vPos.y -(128.f * 0.5f);
		m_vRealVertex[1].x = vPos.x +(128.f * 0.5f);
		m_vRealVertex[1].y = vPos.y -(128.f * 0.5f);
		m_vRealVertex[2].x = vPos.x +(128.f * 0.5f);
		m_vRealVertex[2].y = vPos.y +(128.f * 0.5f);
		m_vRealVertex[3].x = vPos.x -(128.f * 0.5f);
		m_vRealVertex[3].y = vPos.y +(128.f * 0.5f);
	}

public:
	LINEINFO* GetLinesInfo(void)
	{
		LINEINFO* pLineArray = new LINEINFO[4];

		// 플레이어 보유 선분 업데이트
		// 첫번째 선분
		LINEPOS tLeftPoint = { m_vRealVertex[0].x, m_vRealVertex[0].y };
		LINEPOS tRightPoint = { m_vRealVertex[1].x, m_vRealVertex[1].y };
		LINEINFO tInfo = { tLeftPoint, tRightPoint };
		pLineArray[0].tLPoint = tInfo.tLPoint;
		pLineArray[0].tRPoint = tInfo.tRPoint;

		// 두번째 선분
		tLeftPoint = { m_vRealVertex[1].x, m_vRealVertex[1].y };
		tRightPoint = { m_vRealVertex[2].x, m_vRealVertex[2].y };
		tInfo = { tLeftPoint, tRightPoint };
		pLineArray[1].tLPoint = tInfo.tLPoint;
		pLineArray[1].tRPoint = tInfo.tRPoint;

		// 세번째 선분
		tLeftPoint = { m_vRealVertex[2].x, m_vRealVertex[2].y };
		tRightPoint = { m_vRealVertex[3].x, m_vRealVertex[3].y };
		tInfo = { tLeftPoint, tRightPoint };
		pLineArray[2].tLPoint = tInfo.tLPoint;
		pLineArray[2].tRPoint = tInfo.tRPoint;

		// 네번째 선분
		tLeftPoint = { m_vRealVertex[3].x, m_vRealVertex[3].y };
		tRightPoint = { m_vRealVertex[0].x, m_vRealVertex[0].y };
		tInfo = { tLeftPoint, tRightPoint };
		pLineArray[3].tLPoint = tInfo.tLPoint;
		pLineArray[3].tRPoint = tInfo.tRPoint;

		return pLineArray;
	}
	

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
