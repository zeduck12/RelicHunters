#pragma once

class CInteractionManager
{

public:
	CInteractionManager()  = default;
	~CInteractionManager() = default;


public:
	static bool InteractPlayerItem(CObj* _pPlayer, CObj* _pItem);     // �÷��̾�� ������ ��ȣ�ۿ�
	static bool InteractPlayerHologram(CObj* _pPlayer, CObj* _pHologram); // �÷��̾�� Ȧ�α׷� ��ȣ�ۿ�
	static void Render(CObj* _pItem);
	static void RenderHologramVer(CObj* _pHologram);
	static void RenderActivation(CObj* _pMonster); // �÷��̾�� ���� ��ȣ�ۿ�



private:
	static float m_fX			;
	static float m_fY			;
	static int   m_iDrawID		;
	static int   m_iMaxDrawID	;
	static float m_fStackTime	;
	static float m_fCoolTime	;
	static int	 m_iActivationDrawID;

};

