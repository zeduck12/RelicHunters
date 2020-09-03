#pragma once
// 플레이어 관련 모든 UI Info들 관리해주는 곳
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
	// 캐릭터 이름에 따라 사진 다르게 띄워주기 위해
	float m_fAddValue = 0.f;

	bool m_bIsRecover = false;
	float m_fStackTime = 0.f;
};

