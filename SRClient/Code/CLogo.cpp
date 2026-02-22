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
#include "CLogoBG.h"
#include "CLogoTitle.h"
#include "CLogoBtnArea.h"
#include "CColliderMgr.h"
#include "CGraphicDev.h"

CLogo::CLogo()
	: CScene()
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	D3DXMATRIX matView, matProj;
	D3DXVECTOR3 vEye(0.0f, 0.0f, -2.0f);    
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f);     
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);   
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4.0f, (float)WINCX / (float)WINCY , 0.1f, 1000.0f);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);


	CColliderMgr::GetInstance()->Set_Render(false);
	if (FAILED(Ready_Environment_Layer(L"0_Environment_Layer")))
		return E_FAIL;
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


	CLogoBG* pLogoBG = CLogoBG::Create(0.f, 0.f);
	if (nullptr == pLogoBG)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"CLogoBG", pLogoBG)))
		return E_FAIL;

	CLogoTitle* pLogoTitle = CLogoTitle::Create(0.f, 0.3f);
	if (nullptr == pLogoTitle)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"CLogoTitle", pLogoTitle)))
		return E_FAIL;

	
	CLogoBtnArea* pLogoBtnArea = CLogoBtnArea::Create(0.f, -0.4f);
	if (nullptr == pLogoBtnArea)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"pLogoBtnArea", pLogoBtnArea)))
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
	CColliderMgr::GetInstance()->Clear_ColliderGroup();
}
