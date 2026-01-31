#include "CGraphicDev.h"

IMPLEMENT_SINGLETON(CGraphicDev)

CGraphicDev::CGraphicDev() : m_pSDK(nullptr), m_pGraphicDev(nullptr)
{

}

CGraphicDev::~CGraphicDev()
{
	Free();
}

HRESULT CGraphicDev::Ready_GraphicDev(HWND hWnd, 
										WINMODE eMode, 
										const _uint& iSizeX, 
										const _uint& iSizeY, 
										CGraphicDev** ppGraphicDev)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (nullptr == m_pSDK)
		return E_FAIL;

	D3DCAPS9	DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									&DeviceCaps)))
	{
		return E_FAIL;
	}

	_ulong	dwFlag(0);

	// 버텍스 프로세싱 = 정점 변환 + 조명 연산

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		dwFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		dwFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;


	D3DPRESENT_PARAMETERS			d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth  = iSizeX;
	d3dpp.BackBufferHeight = iSizeY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount  = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// D3DSWAPEFFECT_DISCARD : 스왑 체인 방식
	// D3DSWAPEFFECT_FLIP : 하나의 버퍼를 뒤집어가면서 사용
	// D3DSWAPEFFECT_COPY : 더블 버퍼링과 유사

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = hWnd;
	
	d3dpp.Windowed = eMode;		// FALSE : 전체 화면, TRUE : 창 모드

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hWnd, dwFlag, &d3dpp, &m_pGraphicDev)))
	{
		return E_FAIL;
	}

	*ppGraphicDev = this;

	return S_OK;
}

// 후면 버퍼 역할
// 1. 지우기
// 2. 다음 화면 그릴 것을 준비
// 3. 버퍼를 교환
void CGraphicDev::Render_Begin(D3DXCOLOR Color)
{
	m_pGraphicDev->Clear(0,		// 렉트의 개수
						NULL,	// 렉트의 첫 번째 주소
						D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
						Color,  // 백버퍼 텍스처 색상
						1.f,	// 깊이 버퍼 초기화 값
						0);		// 스텐실 버퍼 초기화 값

	m_pGraphicDev->BeginScene();

}

void CGraphicDev::Render_End()
{
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, NULL, NULL);

	// 1, 2 : 렉트의 주소
	// 3 : 핸들(NULL을 넣을 경우, 	d3dpp.hDeviceWindow = hWnd이 반영)
	// 4 : 복사 옵션

	// 1, 2, 4를 사용하려면 d3dpp.SwapEffect 의 옵션이 D3DSWAPEFFECT_COPY만 가능
}

void CGraphicDev::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pSDK);
}
