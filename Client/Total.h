#pragma once

extern HWND g_hWND;

namespace OBJ
{
	enum ID { MONSTER, PLAYER, BULLET, STRUCTURE, OBJ_UI, OBJ_MOUSE, OBJ_END };
}

namespace IMAGE
{
	enum ID { PLAYER, DUCK, KAMIKAZE, KAMIKAZE_FLY, END};
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