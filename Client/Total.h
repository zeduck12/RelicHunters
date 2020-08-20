#pragma once

extern HWND g_hWND;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;  // 위치벡터 
	D3DXVECTOR3 vDir; // 방향벡터 
	D3DXVECTOR3 vSize; // 크기 표현할 놈. 
	D3DXVECTOR3 vLook; // 단위 벡터.

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
