#pragma once
#include "CPlayer.h"

class CMainApp
{
public:
	explicit CMainApp();
	~CMainApp();

public:
	void Ready();
	void Update();
	void LateUpdate();
	void Render();
	void Release();

public:
	void SetImages(void);

private:
	HDC m_hDC;

};

