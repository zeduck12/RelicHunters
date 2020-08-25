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

namespace ANIMATION
{
	enum TYPE { ATLAS, FRAME, TYPE_END };
	enum OPTION
	{
		LOOP,		  // �ַ� IDLE ���� (Default ���)
		ONCE_RETURN,  // �ѹ� ���� Default �������
		ONCE_DESTROY, // ����Ʈ ���� ��� ���ϰ� ����������.
		TIME_RETURN,  // �����ð� ���� �ݺ��ϴ� ����Ʈ�� ���ư��� ��
		TIME_DESTROY, // ���������� �����ð� ���� ���ӵɶ� �ִϸ��̼���
	};
}