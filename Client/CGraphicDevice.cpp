#include "pch.h"
#include "CGraphicDevice.h"

DEFINITION_SINGLETON(CGraphicDevice)

CGraphicDevice::~CGraphicDevice()
{
}

HRESULT CGraphicDevice::Ready(void)
{
	// 1. SDK를 먼저 만든다.
	// 2. 장치의 지원 수준을 조사한다. SDK
	// 3. 지원 수준에 맞는 COM 객체 (실질적으로 장치를 제어할 객체) 생성

	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// 1. 장치 수준을 조사할 COM객체 생성
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		ERR_MSG(L"Get DeviceCaps Failed");
		return E_FAIL;
	}

	DWORD vp = 0;
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	//백 버퍼의 가로 세로 크기. 
	d3dpp.BackBufferWidth  = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount  = 1;

	d3dpp.MultiSampleType	 = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect	= D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hWND;
	// 거짓이면 전체화면, 참이면  창모드을 사용하겠다. 
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil	 = TRUE;
	d3dpp.AutoDepthStencilFormat	 = D3DFMT_D24S8;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval		 = D3DPRESENT_INTERVAL_DEFAULT;

	//3. 조사하고 세팅한 데이터를 가지고 이제 장치를 제어할 컴객체를 생성하자!
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWND, vp, &d3dpp, &m_pDevice)))
	{
		ERR_MSG(L"Failed Creating Device");
		return E_FAIL;
	}
	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		ERR_MSG(L"Failed Creating Sprite");
		return E_FAIL;
	}

	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));
	tFontInfo.Height  = 20;
	tFontInfo.Width   = 10;
	tFontInfo.Weight  = FW_HEAVY;
	tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"메이플스토리");
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"폰트 생성 실패");
		return E_FAIL;
	}


	return S_OK;
}

void CGraphicDevice::Release(void)
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
	Safe_Release(m_pSDK);
}

void CGraphicDevice::RenderBegin(void)
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 255, 255, 255), // BackBuffer의 색깔을 정함. 보통은 D3DCOLOR_ARGB(255, 0, 0, 255) 파랑색
		0.f, 0);
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGraphicDevice::RenderEnd(HWND _hWnd)
{
	m_pSprite->End();
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, _hWnd, nullptr);
}
