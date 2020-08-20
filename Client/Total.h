#pragma once

extern HWND g_hWND;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;  // ��ġ���� 
	D3DXVECTOR3 vDir; // ���⺤�� 
	D3DXVECTOR3 vSize; // ũ�� ǥ���� ��. 
	D3DXVECTOR3 vLook; // ���� ����.

}INFO;

namespace OBJ
{
	enum ID { MONSTER, PLAYER, BULLET, OBJ_UI, OBJ_MOUSE, OBJ_END };
}

namespace DIRECTION
{
	enum ID { LEFT, RIGHT, UP, DOWN, END };
}

namespace GUN
{
	enum ID { DEFAULT, SHOTGUN, BOOMERANG, END};
}
