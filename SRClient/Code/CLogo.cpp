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
#include "CParticleMgr.h"
#include "CTransitionFade.h"

CLogo::CLogo()
	: CScene()
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	
	Update_Camera();
	CColliderMgr::GetInstance()->Set_Render(false);
	if (FAILED(Ready_Environment_Layer(L"0_Environment_Layer")))
		return E_FAIL;

	CSoundMgr::GetInstance()->PlaySoundLoop(L"Sound_Logo_BGM", CSoundMgr::BGM, 1.f);



	CParticleMgr::GetInstance()->Clear_Particle();
	CParticleMgr::GetInstance()->Set_ParicleOn(true);
	CParticleMgr::GetInstance()->spwan_Particle(PARTICLE_BLOOMBUBBLE, { 0.f,0.f,5.f }, 200);

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	Update_Camera();
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	CParticleMgr::GetInstance()->Update_Particle(fTimeDelta);
	return iExit;
}

void CLogo::LateUpdate_Scene(const _float& fTimeDelta)
{
	Update_Camera();
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CLogo::Render_Scene()
{
#ifdef _DEBUG
	_vec2	vPos{ 0.f, 0.f };
	CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Default");
	pDefFont->Render_Font(L"Here is CLogo", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));



	ImGui::Begin("Curr Scene: CLogo");
	if (ImGui::Button("Go Ship Scene"))
	{
		//CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_LOGO, CTransition::SCENE_SHIP));
		CTransition::FadedTransition(CTransition::SCENE_LOGO, CTransition::SCENE_SHIP);
	}
	ImGui::End();


	// Black To Screen
	if (ImGui::Button("FadeIn"))
	{
		//CTransitionFade

		//CTransitionFade
		auto p = CManagement::GetInstance()->Get_Scene()->Get_Layer();
		if (auto pLayers = CManagement::GetInstance()->Get_Scene()->Get_Layer())
		{
			if (!pLayers->empty())
			{
				CTransitionFade* pFade = CTransitionFade::Create(0.f, 0.f, CTransitionFade::FADE_IN);

				for (auto& p : *pLayers)
				{
					p.second->Add_GameObject(L"99_FADE", pFade);
					break;
				}
			}
		}
	}

	// Screen To Black
	if (ImGui::Button("FadeOut"))
	{
		//CTransitionFade
		auto p = CManagement::GetInstance()->Get_Scene()->Get_Layer();
		if (auto pLayers = CManagement::GetInstance()->Get_Scene()->Get_Layer())
		{
			if (!pLayers->empty())
			{
				CTransitionFade* pFade = CTransitionFade::Create(0.f, 0.f, CTransitionFade::FADE_OUT);
				pFade->Set_OnEnd([]() {
					//CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_LOGO, CTransition::SCENE_SHIP));
					});
				for (auto& p : *pLayers)
				{
					p.second->Add_GameObject(L"99_FADE", pFade);
					break;
				}
			}
		}
	}

	if (ImGui::Button("Temp")) {

		CParticleMgr::GetInstance()->spwan_Particle(PARTICLE_BLOOD, { 0,0,0 }, 4);
	}
#endif // !_DEBUG


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

void CLogo::Update_Camera()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	D3DXMATRIX matView, matProj;
	D3DXVECTOR3 vEye(0.0f, 0.0f, -2.0f);
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, (float)WINCX / (float)WINCY, 0.1f, 1000.0f);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
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
	CParticleMgr::GetInstance()->Clear_Particle();
	CColliderMgr::GetInstance()->Clear_ColliderGroup();
	CSoundMgr::GetInstance()->StopAll();
}
