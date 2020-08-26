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
	D3DXVECTOR3	vPos;		// ��ġ
	D3DXVECTOR3	vSize;		// ũ��
	int	iDrawID;	// ��ο� ���̵�
	int	iOption;	// �ɼ�
	int	iFrameX;	// ��� ���� X
	int	iFrameY;	// ��� ���� Y
}TILE;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9 pTexture; // �̹��� ������ �����ϱ� ���� �İ�ü. 
	D3DXIMAGE_INFO tImageInfo;   //�̹����� ������ ��Ƶ� ����ü. 

}TEXINFO;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;  // ��ġ���� 
	D3DXVECTOR3 vDir;  // ���⺤�� 
	D3DXVECTOR3 vSize; // ũ�� ǥ���� ��.
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vImageSize;
	D3DXMATRIX  mat;

}INFO;
