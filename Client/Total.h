#pragma once

extern HWND g_hWND;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;  // ��ġ���� 
	D3DXVECTOR3 vDir;  // ���⺤�� 
	D3DXVECTOR3 vSize; // ũ�� ǥ���� ��.
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vImageSize;
	D3DXMATRIX  mat;

}INFO;

namespace OBJ
{
	enum ID { MONSTER, PLAYER, BULLET, STRUCTURE, OBJ_UI, OBJ_MOUSE, OBJ_END };
}

namespace DIRECTION
{
	enum ID { LEFT, RIGHT, UP, DOWN, END };
}

namespace GUN
{
	enum ID { DEFAULT, SHOTGUN, BOOMERANG, END};
}
