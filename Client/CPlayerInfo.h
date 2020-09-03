#pragma once
// �÷��̾� ���� ��� UI Info�� �������ִ� ��
class CPlayerInfo
{
public:
	CPlayerInfo() = default;
	CPlayerInfo(const wstring& _strName);
	~CPlayerInfo() = default;

public:
	bool Ready(void);
	void Update(void);
	void Render(void);

private:
	void DrawCharacterEmoticon(void);
	void DrawCharacterShieldBar(void);
	void DrawCharacterHpBar(void);
	void DrawCoinBar(void);
	void DrawIcons(void);
	void DrawWeapon(void);
	void DrawSubWeapon(void);
	void DrawBulletCount(void);
	void DrawBombsCount(void);
	void DrawDashBar(void);

private:
	wstring m_strCharacterName;
	// ĳ���� �̸��� ���� ���� �ٸ��� ����ֱ� ����
	float m_fAddValue = 0.f;

	bool m_bIsRecover = false;
	float m_fStackTime = 0.f;
};

