#pragma once

class CInteractionManager
{

public:
	CInteractionManager()  = default;
	~CInteractionManager() = default;


public:
	static bool InteractPlayerItem(CObj* _pPlayer, CObj* _pItem); // 플레이어와 아이템 상호작용
	static void Render(CObj* _pItem);



private:
	static float m_fX			;
	static float m_fY			;
	static int   m_iDrawID		;
	static int   m_iMaxDrawID	;
	static float m_fStackTime	;
	static float m_fCoolTime	;

};

