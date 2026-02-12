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
#include "CTransition.h"


CLogo::CLogo()
	: CScene()
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	ImGui::Begin("Curr Scene: CLogo");
	if (ImGui::Button("Go Ship Scene"))
	{
		CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_LOGO, CTransition::SCENE_SHIP));
	}
	ImGui::End();

	return iExit;
}

void CLogo::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CLogo::Render_Scene()
{
	_vec2	vPos{ 0.f, 0.f };
	CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Default");
	pDefFont->Render_Font(L"Here is CLogo", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CLogo::Ready_Environment_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

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
	CScene::Free();
}
