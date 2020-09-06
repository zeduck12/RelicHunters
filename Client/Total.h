#pragma once

extern HWND g_hWND;

namespace OBJ
{
	enum ID { MONSTER, PLAYER, BULLET, STRUCTURE, OBJ_UI, OBJ_MOUSE, OBJ_END };
}

namespace PLAYER
{
	enum ID { JIMMY, PINKY, RAFF,};
}

namespace IMAGE
{
	enum ID {
		DUCK, KAMIKAZE, KAMIKAZE_FLY, TURTLE, BOSS,
		PICKUP_LIGHT, PICKUP_MEDIUM, PICKUP_HEAVY, PICKUP_SHIELD, PICKUP_HEALTH, PROMPT_E,
		COIN, PLAYER, TELEPORTER, UI_LOBBY_BUTTON, END
	};
}

namespace TELEPORTER
{
	enum TYPE { IDLE, SPAWN, CHANGE };
}

namespace DIRECTION
{
	enum ID { LEFT, RIGHT, UP, DOWN, END };
}

namespace GUN
{
	enum ID 
	{
		DEFAULT,			// �÷��̾� �⺻�� - ���� x
		FLAME,				// ����   �⺻��
		HIGH_MAG,			// �θ޶� ��
		ASSAULT,			// 3�� �������� ������ ��
		KEYTAR,				// 2�� �������� ������ ��
		MACHINEGUN,			// ���ӹ߻� �Ǵ� ��
		PISTOL_ASSUALT,		// ���ӹ߻� �Ǵ� �� (�Ѿ˸� �ٸ�)
		PISTOL_HEAVY,		// ������ �� ���ӹ߻� ��
		PLASMA,				// �ö�� �� ��
		SHOTGUN,			// ���� 
		SNIPER,				// �������� �� �ܹ� ������
		END
	};
}

namespace BUTTON
{
	enum ID { START, CHANGE, CLOSE ,END};
	enum SEL_ID { JIMMY, PINKY, RAFF, BIU, PUNNY, ASS, RANDOM, SEL_END};
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