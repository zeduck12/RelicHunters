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

		// �÷��̾� ���� ���� ������Ʈ
		// ù��° ����
		LINEPOS tLeftPoint = { m_vRealVertex[0].x, m_vRealVertex[0].y };
		LINEPOS tRightPoint = { m_vRealVertex[1].x, m_vRealVertex[1].y };
		LINEINFO tInfo = { tLeftPoint, tRightPoint };
		pLineArray[0].tLPoint = tInfo.tLPoint;
		pLineArray[0].tRPoint = tInfo.tRPoint;

		// �ι�° ����
		tLeftPoint = { m_vRealVertex[1].x, m_vRealVertex[1].y };
		tRightPoint = { m_vRealVertex[2].x, m_vRealVertex[2].y };
		tInfo = { tLeftPoint, tRightPoint };
		pLineArray[1].tLPoint = tInfo.tLPoint;
		pLineArray[1].tRPoint = tInfo.tRPoint;

		// ����° ����
		tLeftPoint = { m_vRealVertex[2].x, m_vRealVertex[2].y };
		tRightPoint = { m_vRealVertex[3].x, m_vRealVertex[3].y };
		tInfo = { tLeftPoint, tRightPoint };
		pLineArray[2].tLPoint = tInfo.tLPoint;
		pLineArray[2].tRPoint = tInfo.tRPoint;

		// �׹�° ����
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
