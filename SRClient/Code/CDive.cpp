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
#include "CTerrian.h"
#include "CSkyBox.h"
#include "CTestFish.h"
#include "CDiveItemBox.h"
#include "CBackGroundSea.h"
#include "CDiveDaveUI.h"
#include "CGaugeBarUI.h"
#include "CDaggerBoxUI.h"
#include "CDaggerThumbnailUI.h"
#include "CDaggerBoxMouseLKeyUI.h"
#include "CItemBoxUI.h"
#include "CChangeTab.h"
#include "CRKeyUI.h"
#include "CCKeyUI.h"
#include "CWPBoxUI.h"
#include "CTabKeyUI.h"
#include "CO2UI.h"
#include "CO2StrokeUI.h"
#include "CO2Text.h"
#include "CJohn.h"

#include "CFishHQ.h"
#include "CBackToShipUI.h"
#include "FishInclude.h"

#include "CParticleMgr.h"
#include "CJohn2.h"

#include "CGameMemMgr.h"
#include "CO2TxT.h"
#include "CDepthText.h"
#include "CWeightIcon.h"
#include "CWeightText.h"
#include "COverloadedIcon.h"
#include "CDiveItemDescUI.h"
#include "CCommonItemWood.h"
#include "CCommonItemWoodPlate.h"
#include "CCommonItemWatch.h"
#include "CCommonItemBone.h"
#include "CCommonItemDeepseaCoral.h"
#include "CCommonItemFragment.h"
#include "CCommonItemThurible.h"
#include "CCommonItemRope.h"
#include "CCommonItemUmb.h"
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
	if (FAILED(Ready_UI_Layer(L"0_UI_Layer")))
		return E_FAIL;

	if (FAILED(Ready_Fish_Layer(L"2_Fish_Layer")))
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
	//CTransform* pDaveTransform = static_cast<CTransform*>(pDiveDave->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	//static_cast<CDiveDaveCam*>(pCamera)->Set_Target(&pDaveTransform->m_vInfo[INFO_POS]);

	// 개발용 FreeCam
	pCamera = CFreeCam::Create(&vEye, &vAt, &vUp, D3DXToRadian(60.f), (_float)WINCX / WINCY, 1.f, 1000.f);
	if (nullptr == pCamera)
		return E_FAIL;
	CCameraMgr::GetInstance()->Set_Camera(L"FreeCam", pCamera);



	CMapMgr::GetInstance()->Load();

	CParticleMgr::GetInstance()->Set_Player(pDiveDave);
	CParticleMgr::GetInstance()->Ready_Particle(CInfoMgr::GetInstance()->Get_HWND());


	// [LSY] 데이브 아이다이버 수치 연동
	if (const auto& pDave = m_mapLayer[L"0_GameLogic_Layer"]->Get_GameObjectFirst<CDiveDave>(L"DiveDave"))
	{
		const auto daveInfo = CGameMemMgr::GetInstance()->Get_DaveInfo();
		pDave->Set_MaxHp(daveInfo.Get_GonggiVolume());
		pDave->Set_MaxDepth(daveInfo.Get_JamsuDepth());
		pDave->Set_MaxStorageWeight(daveInfo.Get_JeokjaeWeight());
	}

	// [LSY] 다이브씬 시작하면 다이브 인포 기록 시작
	CGameMemMgr::CDiveInfo info{};
	info.DiveStart();
	CGameMemMgr::GetInstance()->Get_DiveInfos().push_back(info);
	return S_OK;
}

_int CDive::Update_Scene(const _float& fTimeDelta)
{
	CColliderMgr::GetInstance()->Set_Render(false);

	if (ImGui::Button("BackToShipUI"))
	{
		if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
		{
			if (auto pUI = pLayer->Get_GameObjectFirst(L"BackToShipUI"))
			{
				pUI->Set_DeadCascade();
			}
			else
			{
				auto pBackToShipUI = CBackToShipUI::Create(0.f, 0.f);
				pLayer->Add_GameObject(L"BackToShipUI", pBackToShipUI);
			}
		}
	}
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	CParticleMgr::GetInstance()->Update_Particle(fTimeDelta);
	ImGui::Begin("Curr Scene: CDive");
	if (ImGui::Button("Go Ship Scene"))
	{
		CMapMgr::GetInstance()->SetScene(nullptr);
		//CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_DIVE, CTransition::SCENE_SHIP));
		CTransition::FadedTransition(CTransition::SCENE_DIVE, CTransition::SCENE_SHIP);
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

HRESULT CDive::Ready_Fish_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();

	CFishHQ* pFishHQ = CFishHQ::Create();
	pLayer->Add_GameObject(L"FishHQ", pFishHQ);

	//{
	//	Fish::CBlueTang* pFish = Fish::CBlueTang::Create(0.f, 0.f, 0.05f * 0.3f);
	//	pFish->Set_Parent(pFishHQ);
	//	pLayer->Add_GameObject(L"FishBlueTang", pFish);

	//	_vec3 vPos = { 0.0f, 0.f, 0.f };
	//	_vec3 vExt = { 1.1f, 0.6f, 0.01f };
	//	vExt *= 0.3f;

	//	CFishHitCollider* pCollider = CFishHitCollider::Create(&vPos, &vExt);
	//	pCollider->Set_Parent(pFish);
	//	pLayer->Add_GameObject(L"FishColl", pCollider);
	//}


	//{
	//	Fish::CBlueTang* pFish = Fish::CBlueTang::Create(1.f, 1.f, 0.05f * 0.3f);
	//	pFish->Set_Parent(pFishHQ);
	//	pLayer->Add_GameObject(L"FishBlueTang", pFish);

	//	_vec3 vPos = { 0.0f, 0.f, 0.f };
	//	_vec3 vExt = { 1.1f, 0.6f, 0.01f };
	//	vExt *= 0.3f;

	//	CFishHitCollider* pCollider = CFishHitCollider::Create(&vPos, &vExt);
	//	pCollider->Set_Parent(pFish);
	//	pLayer->Add_GameObject(L"FishColl", pCollider);
	//}
	//if (FAILED(Fish::AddLayer_BlueTang(pLayer, 0.f, 0.f, 0.3f)))
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(Fish::AddLayer_BlueTang(pLayer, 1.f, 1.f, 0.3f)))
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(Fish::AddLayer_BlueTang(pLayer, 2.f, 2.f, 0.3f)))
	//{
	//	return E_FAIL;
	//}

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
	m_pDive = pDiveDave;

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

	pGameObject = CDiveDaveGun::Create(CGameMemMgr::CDaveInfo::GUN_DEFAULT);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveDaveGun", pGameObject)))
		return E_FAIL;
	pGameObject->Set_Parent(pDiveDave);



	//테스트용
	pGameObject = CTestFish::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"TestFish", pGameObject)))
		return E_FAIL;



	//-----------------------------------------------------ItemBox---------------------------------------------------------------------
	// ItemBox 생성
	// [KDS] : 박스의 종류와 위치를 인자로 받습니다. DROPITEM의 종류도 넣어 주세요.
	// 추가로 z좌표는 0.1f 해줘야 플레이어 뒤에 보입니다.
	// Add_GameObject 태그를 다르게 해줘야 트랜스폼이 각자마다 적용됩니다.
	pGameObject = CDiveItemBox::Create(ITEMBOXTEX::CHEST_A, 3, 3, 0.1f);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveItemBox1", pGameObject)))
		return E_FAIL;

	pGameObject = CDiveItemBox::Create(ITEMBOXTEX::CHEST_BOX, 0, 3, 0.1f);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveItemBox2", pGameObject)))
		return E_FAIL;

	pGameObject = CDiveItemBox::Create(ITEMBOXTEX::CHEST_WEAPON, -3, 3, 0.1f, DROPITEM::TRIPLEAXEL);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveItemBox3", pGameObject)))
		return E_FAIL;

	pGameObject = CDiveItemBox::Create(ITEMBOXTEX::CHEST_WEAPON, -6, 3, 0.1f, DROPITEM::PENTAAXEL);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveItemBox4", pGameObject)))
		return E_FAIL;
	//-----------------------------------------------------ItemBox---------------------------------------------------------------------

	// [LSY] test item
	{
		_vec3 vtmp{ -10, 3, 0.1f };
		pGameObject = CCommonItemWood::Create(vtmp);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Item", pGameObject)))
			return E_FAIL;

		vtmp = { -10, 5, 0.1f };
		pGameObject = CCommonItemWoodPlate::Create(vtmp);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Item", pGameObject)))
			return E_FAIL;

		vtmp = { -9, 5, 0.1f };
		pGameObject = CCommonItemWatch::Create(vtmp);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Item", pGameObject)))
			return E_FAIL;

		vtmp = { -8, 5, 0.1f };
		pGameObject = CCommonItemBone::Create(vtmp);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Item", pGameObject)))
			return E_FAIL;


		vtmp = { -7, 5, 0.1f };
		pGameObject = CCommonItemDeepseaCoral::Create(vtmp);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Item", pGameObject)))
			return E_FAIL;

		vtmp = { -6, 5, 0.1f };
		pGameObject = CCommonItemFragment::Create(vtmp);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Item", pGameObject)))
			return E_FAIL;

		vtmp = { -5, 5, 0.1f };
		pGameObject = CCommonItemThurible::Create(vtmp);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Item", pGameObject)))
			return E_FAIL;

		vtmp = { -4, 5, 0.1f };
		pGameObject = CCommonItemRope::Create(vtmp);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Item", pGameObject)))
			return E_FAIL;

		vtmp = { -3, 5, 0.1f };
		pGameObject = CCommonItemUmb::Create(vtmp);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Item", pGameObject)))
			return E_FAIL;

	}

	
	// 맵 
	pGameObject = CTerrian::Create(L"BackGround_GLB_File");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"BackGround_GLB_File", pGameObject)))
		return E_FAIL;


	pGameObject = CTerrian::Create(L"GLB_Terrian1", L"Terrian1_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"GLB_Terrian1", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrian::Create(L"GLB_Terrian2", L"Terrian2_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"GLB_Terrian2", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrian::Create(L"GLB_Terrian3", L"Terrian3_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"GLB_Terrian3", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrian::Create(L"GLB_Terrian4", L"Terrian4_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"GLB_Terrian4", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrian::Create(L"GLB_Terrian5", L"Terrian5_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"GLB_Terrian5", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrian::Create(L"GLB_Terrian6", L"Terrian6_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"GLB_Terrian6", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrian::Create(L"GLB_Terrian7", L"Terrian6_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"GLB_Terrian7", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrian::Create(L"GLB_Terrian8", L"Terrian6_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"GLB_Terrian8", pGameObject)))
		return E_FAIL;

	// 보스
	pGameObject = CJohn::Create(15.f, 10.f, 0.f);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"John", pGameObject)))
		return E_FAIL;

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

HRESULT CDive::Ready_UI_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	// DaggerBox
	pGameObject = CDaggerBoxUI::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveDaveDaggerBoxUI", pGameObject)))
		return E_FAIL;


	pGameObject = CDaggerThumbnailUI::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveDaveDaggerThumbnailUI", pGameObject)))
		return E_FAIL;

	pGameObject = CDaggerBoxMouseLKeyUI::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveDaveDaggerBoxMouseLKeyUI", pGameObject)))
		return E_FAIL;

	// ItemBox
	pGameObject = CItemBoxUI::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveDaveItemBoxUI_1", pGameObject)))
		return E_FAIL;
	static_cast<CDiveDave*>(m_pDive)->Add_Observer(static_cast<IObserver*>(pGameObject)); // 플레이어 관찰

	pGameObject = CItemBoxUI::Create(true);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveDaveItemBoxUI_2", pGameObject)))
		return E_FAIL;
	static_cast<CDiveDave*>(m_pDive)->Add_Observer(static_cast<IObserver*>(pGameObject)); // 플레이어 관찰

	pGameObject = CChangeTab::Create(385.f, -330.f, 0.f);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveDaveChangeTab1", pGameObject)))
		return E_FAIL;

	pGameObject = CRKeyUI::Create(405.f, -330.f, 0.f);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RKeyUI", pGameObject)))
		return E_FAIL;
	static_cast<CDiveDave*>(m_pDive)->Add_Observer(static_cast<IObserver*>(pGameObject)); // 플레이어 관찰

	pGameObject = CCKeyUI::Create(350.f, -255.f, 0.f);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"CKeyUI", pGameObject)))
		return E_FAIL;

	// WPBox
	pGameObject = CWPBoxUI::Create(false);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"WPBoxUI1", pGameObject)))
		return E_FAIL;
	static_cast<CDiveDave*>(m_pDive)->Add_Observer(static_cast<IObserver*>(pGameObject)); // 플레이어 관찰

	pGameObject = CWPBoxUI::Create(true);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"WPBoxUI2", pGameObject)))
		return E_FAIL;
	static_cast<CDiveDave*>(m_pDive)->Add_Observer(static_cast<IObserver*>(pGameObject)); // 플레이어 관찰


	pGameObject = CChangeTab::Create(530.f, -328.f, 0.f);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveDaveChangeTab2", pGameObject)))
		return E_FAIL;

	pGameObject = CTabKeyUI::Create(550, -328.f, 0.f);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"TabKeyUI", pGameObject)))
		return E_FAIL;
	static_cast<CDiveDave*>(m_pDive)->Add_Observer(static_cast<IObserver*>(pGameObject)); // 플레이어 관찰

	// O2 UI
	pGameObject = CO2UI::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"O2UI", pGameObject)))
		return E_FAIL;

	pGameObject = CO2StrokeUI::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"O2StrokeUI", pGameObject)))
		return E_FAIL;
	static_cast<CDiveDave*>(m_pDive)->Add_Observer(static_cast<IObserver*>(pGameObject)); // 플레이어 관찰

	CO2TxT* pO2TxT = CO2TxT::Create(0.f, 0.f);
	if (nullptr == pO2TxT)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"O2Text", pO2TxT)))
		return E_FAIL;
	pO2TxT->Set_Opt(DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
	static_cast<CDiveDave*>(m_pDive)->Add_Observer(static_cast<IObserver*>(pO2TxT)); // 플레이어 관찰

	CO2Text* pO2Text = CO2Text::Create(0.f, 0.f);
	if (nullptr == pO2Text)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"CurO2Text", pO2Text)))
		return E_FAIL;
	pO2Text->Set_Opt(DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
	static_cast<CDiveDave*>(m_pDive)->Add_Observer(static_cast<IObserver*>(pO2Text)); // 플레이어 관찰


	CDepthText* pDepthText = CDepthText::Create(0.f, 0.f);
	if (nullptr == pDepthText)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"CurDepthText", pDepthText)))
		return E_FAIL;
	pO2Text->Set_Opt(DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
	static_cast<CDiveDave*>(m_pDive)->Add_Observer(static_cast<IObserver*>(pDepthText)); // 플레이어 관찰

	pGameObject = CWeightIcon::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"WeightIcon", pGameObject)))
		return E_FAIL;

	CWeightText* pWeightText = CWeightText::Create(0.f, 0.f);
	if (nullptr == pWeightText)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"WeightText", pWeightText)))
		return E_FAIL;
	pO2Text->Set_Opt(DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
	static_cast<CDiveDave*>(m_pDive)->Add_Observer(static_cast<IObserver*>(pWeightText)); // 플레이어 관찰

	pGameObject = COverloadedIcon::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"OverloadedIconUI", pGameObject)))
		return E_FAIL;

	// GaugeBar UI
	pGameObject = CGaugeBarUI::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"GaugeBarUI", pGameObject)))
		return E_FAIL;
	static_cast<CDiveDave*>(m_pDive)->Add_Observer(static_cast<IObserver*>(pGameObject)); // 플레이어 관찰




	// [LSY] 아이템 설명 유아이
	auto pDescUI = CDiveItemDescUI::Create(0.f, -150.f);
	pLayer->Add_GameObject(L"DiveItemDescUI", pDescUI);

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	m_pDive = nullptr;

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
	// [LSY] 씬이 종료될때 다이브 정보 기록
	// TODO: Set_Depth 할때 현재 depth가 아니라 다이브중 했던 최고 잠수 기록 으로 해야 맞을거같다.
	if (const auto& pDave = m_mapLayer[L"0_GameLogic_Layer"]->Get_GameObjectFirst<CDiveDave>(L"DiveDave"))
	{
		CGameMemMgr::GetInstance()->Get_DiveInfos().back().Set_Depth(pDave->Get_BestDepth());
		CGameMemMgr::GetInstance()->Get_DiveInfos().back().DiveEnd();
	}
	

	CScene::Free();
	CParticleMgr::GetInstance()->Clear_Particle();
	CColliderMgr::GetInstance()->Clear_ColliderGroup();
	CCameraMgr::GetInstance()->DestroyInstance();
}
