#pragma once
class CGraphicDevice
{
public:
	DECLARE_SINGLETON(CGraphicDevice)

private:
	CGraphicDevice() = default;
	~CGraphicDevice();

public:
	HRESULT Ready(void);
	void	Release(void);

public:
	void RenderBegin(void);
	void RenderEnd(HWND _hWnd = nullptr);

public:
	LPDIRECT3DDEVICE9 GetDevice(void) const { return m_pDevice; }
	LPD3DXSPRITE	  GetSprite(void) const { return m_pSprite; }
	LPD3DXFONT		  GetFont(void)	  const { return m_pFont; }


public:
	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3D9		  m_pSDK;

	LPD3DXSPRITE	  m_pSprite;
	LPD3DXFONT		  m_pFont;

};

