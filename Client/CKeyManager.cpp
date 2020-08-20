#include "pch.h"
#include "CKeyManager.h"

DEFINITION_SINGLETON(CKeyManager)

void CKeyManager::Update()
{
	m_dwKey = 0;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= KEY_RETURN;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= KEY_S;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN;
	if (GetAsyncKeyState('P') & 0x8000)
		m_dwKey |= KEY_P;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwKey |= KEY_D;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_dwKey |= KEY_SHIFT;
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwKey |= KEY_E;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= KEY_A;
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwKey |= KEY_W;
	if (GetAsyncKeyState('F') & 0x8000)
		m_dwKey |= KEY_F;
	if (GetAsyncKeyState('G') & 0x8000)
		m_dwKey |= KEY_G;
}

bool CKeyManager::Key_UP(DWORD dwKey)
{
	if (m_dwKey & dwKey)
	{
		m_dwKeyUP |= dwKey;
		return false;
	}
	else if (m_dwKeyUP & dwKey)
	{
		m_dwKeyUP ^= dwKey;
		return true;
	}

	return false;
}

bool CKeyManager::Key_DOWN(DWORD dwKey)
{
	if ((m_dwKey & dwKey) && !(m_dwKeyDOWN & dwKey))
	{
		m_dwKeyDOWN |= dwKey;
		return true;
	}
	else if (!(m_dwKey & dwKey) && (m_dwKeyDOWN & dwKey))
	{
		m_dwKeyDOWN ^= dwKey;
		return false;
	}
	return false;
}

bool CKeyManager::Key_Pressing(DWORD dwKey)
{
	if (m_dwKey & dwKey)
		return true;
	return false;
}

CKeyManager::~CKeyManager()
{
}
