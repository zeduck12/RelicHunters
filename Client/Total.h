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
		LOOP,		  // 주로 IDLE 상태 (Default 모션)
		ONCE_RETURN,  // 한번 돌고 Default 모션으로
		ONCE_DESTROY, // 이펙트 같은 경우 빵하고 없어져야함.
		TIME_RETURN,  // 일정시간 동안 반복하다 디폴트로 돌아가는 것
		TIME_DESTROY, // 버프같은거 일정시간 동안 지속될때 애니메이션이
	};
}