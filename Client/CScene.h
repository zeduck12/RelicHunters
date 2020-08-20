#pragma once
class CScene abstract
{
public:
	explicit CScene() = default;
	virtual ~CScene() = default;
public:
	virtual bool Ready(void)			 = 0;
	virtual void Update(void)			 = 0;
	virtual void LateUpdate(void)		 = 0;
	virtual void Render(const HDC& _hdc) = 0;
	virtual void Release(void)			 = 0;
};

