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
#include "CParticleMgr.h"
#include "CCameraMgr.h"
#include "CColliderMgr.h"
#include "CAssetMgr.h"
#include "CAssetDefaultFont.h"
#include "CAssetFmodSound.h"

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
	if (FAILED(CSoundMgr::GetInstance()->Ready_SoundMgr()))
		return E_FAIL;
	if (FAILED(Ready_Scene(m_pGraphicDev)))
		return E_FAIL;
	if (FAILED(CParticleMgr::GetInstance()->Ready_Particle(g_hWnd)))
		return E_FAIL;
	if (FAILED(CImguiMgr::GetInstance()->Ready_Imgui(g_hWnd, m_pGraphicDev)))
		return E_FAIL;

	return S_OK;
}

int CMainApp::Update_MainApp(const float& fTimeDelta)
{
	CDInputMgr::GetInstance()->Update_InputDev();
	
	CImguiMgr::GetInstance()->Update_Imgui();

	CParticleMgr::GetInstance()->Update_Particle(fTimeDelta);

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
	CParticleMgr::GetInstance()->Render_Particle();

	CColliderMgr::GetInstance()->Render();

	m_pDeviceClass->Render_End();

	// 프레임의 맨마지막에 호출하고싶은데 여기가 적당한듯
	CColliderMgr::GetInstance()->Clear_ColliderGroup();
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
	CAssetMgr::GetInstance()->AddAsset(L"Font_Default", CAssetDefaultFont::Create(L"바탕", 15, 20, FW_HEAVY));
	CAssetMgr::GetInstance()->AddAsset(L"Font_Jinji", CAssetDefaultFont::Create(L"궁서", 15, 20, FW_HEAVY));
	CAssetMgr::GetInstance()->LoadAsset(L"Font_Default");
	CAssetMgr::GetInstance()->LoadAsset(L"Font_Jinji");


	// Dinput

	if (FAILED(CDInputMgr::GetInstance()->Ready_InputDev(g_hInst, g_hWnd)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::CScene* pLogo = CLogo::Create();

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

	CColliderMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CInfoMgr::GetInstance()->DestroyInstance();
	CDInputMgr::GetInstance()->DestroyInstance();
	//CFontMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CImguiMgr::GetInstance()->DestroyInstance();
	CParticleMgr::GetInstance()->DestroyInstance();
	CCameraMgr::GetInstance()->DestroyInstance();
	m_pManagement->DestroyInstance();
	CAssetMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	m_pDeviceClass->DestroyInstance();
}
