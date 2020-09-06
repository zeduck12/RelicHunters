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
		DEFAULT,			// 플레이어 기본총 - 연발 x
		FLAME,				// 몬스터   기본총
		HIGH_MAG,			// 부메랑 총
		ASSAULT,			// 3발 연속으로 나가는 총
		KEYTAR,				// 2발 연속으로 나가는 총
		MACHINEGUN,			// 연속발사 되는 총
		PISTOL_ASSUALT,		// 연속발사 되는 총 (총알만 다름)
		PISTOL_HEAVY,		// 데미지 쌘 연속발사 총
		PLASMA,				// 플라즈마 볼 총
		SHOTGUN,			// 샷건 
		SNIPER,				// 스나이퍼 총 단발 원샷총
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
		LOOP,		  // 주로 IDLE 상태 (Default 모션)
		ONCE_RETURN,  // 한번 돌고 Default 모션으로
		ONCE_DESTROY, // 이펙트 같은 경우 빵하고 없어져야함.
		TIME_RETURN,  // 일정시간 동안 반복하다 디폴트로 돌아가는 것
		TIME_DESTROY, // 버프같은거 일정시간 동안 지속될때 애니메이션이
	};
}