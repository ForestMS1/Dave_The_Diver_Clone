#include "CDive.h"
#include "CAssetMgr.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CTransition.h"
#include "CDiveDave.h"
#include "CAttackReadyArm.h"
#include "CCameraMgr.h"
#include "CFreeCam.h"
#include "CColliderMgr.h"

CDive::CDive()
	: CScene()
{
}
CDive::~CDive()
{
}

HRESULT CDive::Ready_Scene()
{
	if (FAILED(Ready_GameLogic_Layer(L"0_GameLogic_Layer")))
		return E_FAIL;

	_vec3	vEye{ 0.f, 0.f, -10.f };
	_vec3	vAt{ 0.f, 0.f, 0.f };
	_vec3	vUp{ 0.f, 1.f, 0.f };
	_matrix	matView, matProj;

	// ChaseToPlayerCam
	CCamera* pCamera = CFreeCam::Create(&vEye, &vAt, &vUp, D3DXToRadian(60.f), (_float)WINCX / WINCY, 1.f, 1000.f);
	if (nullptr == pCamera)
		return E_FAIL;
	CCameraMgr::GetInstance()->Set_Camera(L"ChaseToPlayerCam", pCamera);

	return S_OK;
}

_int CDive::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	ImGui::Begin("Curr Scene: CDive");
	if (ImGui::Button("Go Ship Scene"))
	{
		CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_DIVE, CTransition::SCENE_SHIP));
	}
	ImGui::End();
	return iExit;
}

void CDive::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CDive::Render_Scene()
{
	_vec2	vPos{ 0.f, 0.f };
	CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Default");
	pDefFont->Render_Font(L"Here is CDive", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CDive::Ready_GameLogic_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	CGameObject* pDiveDave = CDiveDave::Create();
	if (nullptr == pDiveDave)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveDave", pDiveDave)))
		return E_FAIL;

	pGameObject = CAttackReadyArm::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"AttackReadyArm", pGameObject)))
		return E_FAIL;
	pGameObject->Set_Parent(pDiveDave);

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

CDive* CDive::Create()
{
	CDive* pDive = new CDive;

	if (FAILED(pDive->Ready_Scene()))
	{
		MSG_BOX("CDive Create Failed");
		Safe_Release(pDive);
		return nullptr;
	}
	return pDive;
}

void CDive::Free()
{
	CScene::Free();
}
