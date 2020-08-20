#pragma once
class CKeyManager
{
public:
	DECLARE_SINGLETON(CKeyManager)

public:
	void Update(void);
	bool Key_UP(DWORD dwKey);
	bool Key_DOWN(DWORD dwKey);
	bool Key_Pressing(DWORD dwKey);
private:
	CKeyManager() = default;
	~CKeyManager();
private:
	DWORD m_dwKey = 0;
	DWORD m_dwKeyUP = 0;
	DWORD m_dwKeyDOWN = 0;

};

