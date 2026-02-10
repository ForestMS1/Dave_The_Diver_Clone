#include "pch.h"
#include "CLogo.h"
#include "CProtoMgr.h"
#include "CManagement.h"
#include "CFontMgr.h"
#include "CDInputMgr.h"
#include "CLog.h"
#include "CSoundMgr.h"
#include "CHelper.h"
#include "CAssetTexture.h"
#include "CAssetMgr.h"
#include "CAssetDefaultFont.h"


CLogo::CLogo()
	: CScene(), m_pLoading(NULL)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	if (FAILED(Ready_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
		return E_FAIL;

	//if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
	//	return E_FAIL;

	m_pLoading = CLoading::Create( CLoading::LOADING_MAPEDITOR);
	if (nullptr == m_pLoading)
		return E_FAIL;


	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	if (m_pLoading->Get_Finish())
	{
		
		//ImGui::Button("asdf");
		if (GetAsyncKeyState(VK_RETURN))
		{
			Engine::CScene* pMapEditor{};
			//Engine::CScene* pStage = CMapEditor::Create(m_pGraphicDev);

			if (nullptr == pMapEditor)
				return -1;

			if (FAILED(CManagement::GetInstance()->Set_Scene(pMapEditor)))
			{
				MSG_BOX("MapEditor Scene Change Failed");
				return -1;
			}
		}

	}


	return iExit;
}

void CLogo::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CLogo::Render_Scene()
{
	// debug ¿ë Ãâ·Â

	_vec2	vPos{ 0.f, 0.f };

	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_pLoading->Get_String(), &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Jinji");
	pDefFont->Render_Font(m_pLoading->Get_String(), &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

}

HRESULT CLogo::Ready_Environment_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;


	
	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

HRESULT CLogo::Ready_Prototype()
{
	


	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcTex", Engine::CRcTex::Create())))
		return E_FAIL;

	CAssetMgr::GetInstance()->AddAsset(L"Tex_LOGO", CAssetTexture::Create(L"../Bin/Resource/Texture/Logo/IU.jpg"));
	CAssetMgr::GetInstance()->LoadAsset();
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_LogoTexture", Engine::CTexture::Create(L"Tex_LOGO"))))
		return E_FAIL;

	return S_OK;
}

CLogo* CLogo::Create()
{
	CLogo* pLogo = new CLogo;

	if (FAILED(pLogo->Ready_Scene()))
	{
		Safe_Release(pLogo);
		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pLogo;
}

void CLogo::Free()
{
	Safe_Release(m_pLoading);

	CScene::Free();
}
