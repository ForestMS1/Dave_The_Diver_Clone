#include "pch.h"
#include "CImguiMgr.h"
#include "CMainApp.h"
#include "CTimerMgr.h"
#include "CFrameMgr.h"
#include "CProtoMgr.h"
#include "CFontMgr.h"
#include "CDInputMgr.h"
#include "CInfoMgr.h"
#include "CSoundMgr.h"
#include "CLightMgr.h"

CMainApp::CMainApp()
	: m_pDeviceClass(nullptr)
	, m_pManagement(CManagement::GetInstance())
	, m_pGraphicDev(nullptr)
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
	if (FAILED(Ready_DefaultSetting(&m_pGraphicDev)))
		return E_FAIL;

	if (FAILED(Ready_Scene(m_pGraphicDev)))
		return E_FAIL;

	if (FAILED(CImguiMgr::GetInstance()->Ready_Imgui(g_hWnd, m_pGraphicDev)))
		return E_FAIL;

	if(FAILED(CSoundMgr::GetInstance()->Ready_SoundMgr()))
		return E_FAIL;

	return S_OK;
}

int CMainApp::Update_MainApp(const float& fTimeDelta)
{
	CDInputMgr::GetInstance()->Update_InputDev();
	
	CImguiMgr::GetInstance()->Update_Imgui();
	
	m_pManagement->Update_Scene(fTimeDelta);

	return 0;
}

void CMainApp::LateUpdate_MainApp(const float& fTimeDelta)
{
	m_pManagement->LateUpdate_Scene(fTimeDelta);
}

void CMainApp::Render_MainApp()
{
	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	m_pManagement->Render_Scene(m_pGraphicDev);

	CImguiMgr::GetInstance()->Render_Imgui(m_pGraphicDev);

	m_pDeviceClass->Render_End();
}

HRESULT CMainApp::Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	if (FAILED(CGraphicDev::GetInstance()->Ready_GraphicDev(g_hWnd, MODE_WIN,
		CInfoMgr::GetInstance()->Get_WINCX(),
		CInfoMgr::GetInstance()->Get_WINCY(),
		&m_pDeviceClass)))
	{
		MSG_BOX("GraphicDev Ready Failed");
		return E_FAIL;
	}

	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);

	(*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);		// Z 버퍼에 무조건 기록, Z버퍼 자동정렬을 할 지를 묻는 옵션
	(*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);	// Z 버퍼에 Z값을 보관할 것인지 묻는 옵션


	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// 폰트 추가

	if (FAILED(CFontMgr::GetInstance()->Ready_Font(m_pGraphicDev, L"Font_Default", L"바탕", 15, 20, FW_HEAVY)))
		return E_FAIL;

	if (FAILED(CFontMgr::GetInstance()->Ready_Font(m_pGraphicDev, L"Font_Jinji", L"궁서", 30, 30, FW_HEAVY)))
		return E_FAIL;

	// Dinput

	if (FAILED(CDInputMgr::GetInstance()->Ready_InputDev(g_hInst, g_hWnd)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::CScene* pLogo = CLogo::Create(pGraphicDev);

	if (nullptr == pLogo)
		return E_FAIL;

	if (FAILED(m_pManagement->Set_Scene(pLogo)))
	{
		Safe_Release(pLogo);
		MSG_BOX("SetScene Failed");
		return E_FAIL;
	}

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		Engine::Safe_Release(pInstance);
		MSG_BOX("MainApp Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pGraphicDev);

	CLightMgr::GetInstance()->DestroyInstance();
	CInfoMgr::GetInstance()->DestroyInstance();
	CDInputMgr::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	CImguiMgr::GetInstance()->DestroyInstance();
	m_pManagement->DestroyInstance();
	m_pDeviceClass->DestroyInstance();
}
