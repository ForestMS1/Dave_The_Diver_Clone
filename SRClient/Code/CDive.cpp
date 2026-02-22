#include "CDive.h"
#include "CAssetMgr.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CTransition.h"
#include "CDiveDave.h"
#include "CAttackReadyArm.h"
#include "CCameraMgr.h"
#include "CFreeCam.h"
#include "CDiveDaveCam.h"
#include "CColliderMgr.h"
#include "CShipBoat.h"
#include "CTargetCurveStart.h"
#include "CTargetArrow.h"
#include "CHarpoon.h"
#include "CHarpoonProjectile.h"
#include "CDiveDaveGun.h"
#include "CMapMgr.h"
#include "CTestGlb.h"
#include "CSkyBox.h"
#include "CTestFish.h"
#include "CDiveItemBox.h"
#include "CBackGroundSea.h"

CDive::CDive()
	: CScene()
{
}
CDive::~CDive()
{
}

HRESULT CDive::Ready_Scene()
{
	CMapMgr::GetInstance()->SetScene(this);


	if (FAILED(Ready_Environment_Layer(L"0_Environment_Layer")))
		return E_FAIL;

	if (FAILED(Ready_GameLogic_Layer(L"0_GameLogic_Layer")))
		return E_FAIL;

	//카메라
	_vec3	vEye{ 0.f, 0.f, -10.f };
	_vec3	vAt{ 0.f, 0.f, 0.f };
	_vec3	vUp{ 0.f, 1.f, 0.f };
	_matrix	matView, matProj;

	// ChaseToPlayerCam
	CCamera* pCamera = CDiveDaveCam::Create(&vEye, &vAt, &vUp, D3DXToRadian(60.f), (_float)WINCX / WINCY, 1.f, 1000.f);
	if (nullptr == pCamera)
		return E_FAIL;
	CCameraMgr::GetInstance()->Set_Camera(L"ChaseToPlayerCam", pCamera);
	CGameObject* pDiveDave = m_mapLayer[L"0_GameLogic_Layer"]->Get_GameObjectFirst(L"DiveDave");
	CTransform* pDaveTransform = static_cast<CTransform*>(pDiveDave->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	static_cast<CDiveDaveCam*>(pCamera)->Set_Target(&pDaveTransform->m_vInfo[INFO_POS]);

	// 개발용 FreeCam
	pCamera = CFreeCam::Create(&vEye, &vAt, &vUp, D3DXToRadian(60.f), (_float)WINCX / WINCY, 1.f, 1000.f);
	if (nullptr == pCamera)
		return E_FAIL;
	CCameraMgr::GetInstance()->Set_Camera(L"FreeCam", pCamera);



	CMapMgr::GetInstance()->Load();
	return S_OK;
}

_int CDive::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);


	ImGui::Begin("Curr Scene: CDive");
	if (ImGui::Button("Go Ship Scene"))
	{
		CMapMgr::GetInstance()->SetScene(nullptr);
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
	CCameraMgr::GetInstance()->Render_Camera();
	CMapMgr::GetInstance()->Render_Map();

}

HRESULT CDive::Ready_Environment_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;




	CGameObject* pBackGroundSea = CBackGroundSea::Create();
	if (nullptr == pBackGroundSea)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"CBackGroundSea", pBackGroundSea)))
		return E_FAIL;


	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
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

	pGameObject = CTargetCurveStart::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"TargetCurveStart", pGameObject)))
		return E_FAIL;
	pGameObject->Set_Parent(pDiveDave);

	pGameObject = CTargetArrow::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"TargetArrow", pGameObject)))
		return E_FAIL;
	pGameObject->Set_Parent(pDiveDave);

	pGameObject = CHarpoon::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Harpoon", pGameObject)))
		return E_FAIL;
	pGameObject->Set_Parent(pDiveDave);

	pGameObject = CHarpoonProjectile::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"HarpoonProjectile", pGameObject)))
		return E_FAIL;
	pGameObject->Set_Parent(pDiveDave);

	pGameObject = CDiveDaveGun::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveDaveGun", pGameObject)))
		return E_FAIL;
	pGameObject->Set_Parent(pDiveDave);

	//테스트용
	pGameObject = CTestGlb::Create(L"GLB_File");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"TestGlb", pGameObject)))
		return E_FAIL;

	//테스트용
	pGameObject = CTestGlb::Create(L"BackGround_GLB_File");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"BackGroundGlb", pGameObject)))
		return E_FAIL;


	//테스트용
	pGameObject = CTestFish::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"TestFish", pGameObject)))
		return E_FAIL;


	pGameObject = CDiveItemBox::Create(ITEMBOXTEX::CHEST_A, 3, 3);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveItemBox", pGameObject)))
		return E_FAIL;

	pGameObject = CDiveItemBox::Create(ITEMBOXTEX::CHEST_BOX, 0, 3);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveItemBox", pGameObject)))
		return E_FAIL;

	pGameObject = CDiveItemBox::Create(ITEMBOXTEX::CHEST_WEAPON, -3, 3);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveItemBox", pGameObject)))
		return E_FAIL;
	

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
	CColliderMgr::GetInstance()->Clear_ColliderGroup();
}
