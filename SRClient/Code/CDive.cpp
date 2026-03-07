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

#include "CDInputMgr.h"

#include "CHoldFishUI.h"

#include "CCoral.h"
#include "CWPAmmoCntText.h"
#include "CAmmoPack.h"
#include "CSoundMgr.h"

#include "CDiveGetWeaponUI.h"
#include "CGetItemUI.h"
#include "CSpaceKeyUI.h"
#include "CFishTankCollider.h"
#include "CDaveGoldBall.h"

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
	_vec3	vEye{ -30.f, -20.f, -10.f };
	_vec3	vAt{ -30.f, -20.f, 0.f };
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

	_matrix CameraProj = CCameraMgr::GetInstance()->Get_Camera(L"ChaseToPlayerCam")->Get_ProjMatrix();
	m_pFrustumCollider = CBoundingFrustum::Create(&CameraProj);
	m_pFrustumCollider->Set_OriginalColor(D3DXCOLOR{});
	CParticleMgr::GetInstance()->Set_ParicleOn(true);
	CParticleMgr::GetInstance()->spwan_Particle(PARTICLE_SEABUBBLE, {0.f,0.f,1.f}, 40);


	return S_OK;
}

_int CDive::Update_Scene(const _float& fTimeDelta)
{

	//CColliderMgr::GetInstance()->Set_Render(false);
	DeepDark();
	if (CDInputMgr::GetInstance()->Key_Down(DIK_H))
	{
		if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
		{
			if (auto pUI = pLayer->Get_GameObjectFirst(L"HoldFishUI"))
			{
				pUI->Set_DeadCascade();
			}
			else
			{
				auto pHoldFishUI = CHoldFishUI::Create(0.f, 0.f);
				pLayer->Add_GameObject(L"HoldFishUI", pHoldFishUI);
			}
		}
	}

	if (CDInputMgr::GetInstance()->Key_Down(DIK_B))
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

	//::GetInstance()->Set_Render(false);
	Frustum();
	
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	
	


	
	CParticleMgr::GetInstance()->Update_Particle(fTimeDelta);

	
	return iExit;
}

void CDive::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);

	
	Place_Fish();
}

void CDive::Render_Scene()
{
#ifdef _DEBUG
	_vec2	vPos{ 0.f, 0.f };
	CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Default");
	pDefFont->Render_Font(L"Here is CDive", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));




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
	ImGui::Begin("Curr Scene: CDive");
	if (ImGui::Button("Go Ship Scene"))
	{
		CMapMgr::GetInstance()->SetScene(nullptr);
		//CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_DIVE, CTransition::SCENE_SHIP));
		CTransition::FadedTransition(CTransition::SCENE_DIVE, CTransition::SCENE_SHIP);
	}
	ImGui::End();

	ImGui::Begin("PARTICLE_TEST");
	if (ImGui::Button("PARTICLE_BLOOMBUBBLE"))
	{
		CGameObject* pDiveDave = m_mapLayer[L"0_GameLogic_Layer"]->Get_GameObjectFirst(L"DiveDave");
		CTransform* pDaveTransform = static_cast<CTransform*>(pDiveDave->Get_Component(ID_DYNAMIC, L"Com_Transform"));


		CParticleMgr::GetInstance()->spwan_Particle(PARTICLE_BLOOMBUBBLE, pDaveTransform->m_vInfo[INFO_POS], 10);
	}
	ImGui::End();


	if (ImGui::Button("Spawn Dave Pos Fish"))
	{
		if (auto pUI = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst<CDiveDave>(L"DiveDave"))
		{
			_vec3 vPos;
			pUI->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

			if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
			{
				if (auto pHq = pLayer->Get_GameObjectFirst<CFishHQ>(L"FishHQ"))
				{
					Fish::AddLayer_Fish<Fish::CGreenHumpheadParrotfish>(L"GreenHumpheadParrotfish", pLayer, vPos.x, vPos.y, 0.3f, pHq);
				}
			}

			//pUI->Set_DeadCascade();
		}
	}
	if (ImGui::Button("CDiveGetWeaponUI"))
	{
		//CDiveGetWeaponUI
		if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
		{
			auto pGetWeaponUI = CDiveGetWeaponUI::Create(500.f, -150.f);
			pGetWeaponUI->Set_ImgAssetName(L"Tex_UI_Gun_Penta_Accel");
			pGetWeaponUI->Set_Title(L"펜타 악셀");
			pGetWeaponUI->Set_Desc(L"펜타 악세루다.");
			pGetWeaponUI->Ready_AfterCreate();
			pLayer->Add_GameObject(L"GetWeaponUI", pGetWeaponUI);
		}
	}
	if (ImGui::Button("CDiveItemDescUI"))
	{
		if (auto pUI = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer")->Get_GameObjectFirst<CDiveItemDescUI>(L"DiveItemDescUI"))
		{
			pUI->Set_Title(L"Tiasdf");
			pUI->Set_Desc(L"DDDEEESSCC");
			pUI->Set_Render(!pUI->Get_Render());
			//pUI->Set_DeadCascade();
		}
		else
		{
		}
	}
	if (ImGui::Button("GetItemUI"))
	{
		if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
		{
			auto pGetItemUI = CGetItemUI::Create(-500.f, 250.f);
			pGetItemUI->Set_ImgAssetName(L"Tex_FishThumb_JohnHead");
			pGetItemUI->Ready_AfterCreate();
			pLayer->Add_GameObject(L"GetItemUI", pGetItemUI);
		}
	}

	if (ImGui::Button("HoldUI"))
	{
		if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
		{
			if (auto pUI = pLayer->Get_GameObjectFirst(L"HoldFishUI"))
			{
				pUI->Set_DeadCascade();
			}
			else
			{
				auto pHoldFishUI = CHoldFishUI::Create(0.f, 0.f);
				pLayer->Add_GameObject(L"HoldFishUI", pHoldFishUI);


			}
		}
	}

	if (ImGui::Button("Fish_Object"))
	{
		if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
		{
			if (auto pColl1 = pLayer->Get_GameObjectFirst(L"FishTankCollider_1"))
			{
				_vec3 vPos;
				pColl1->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);
				//Fish::AddLayer_BlueTang(pLayer, vPos.x, vPos.y, 0.3f, pColl1);
				
			}
			if (auto pColl2 = pLayer->Get_GameObjectFirst(L"FishTankCollider_2"))
			{

			}
			if (auto pColl3 = pLayer->Get_GameObjectFirst(L"FishTankCollider_3"))
			{

			}
			if (auto pColl4 = pLayer->Get_GameObjectFirst(L"FishTankCollider_4"))
			{

			}
			if (auto pColl5 = pLayer->Get_GameObjectFirst(L"FishTankCollider_5"))
			{

			}
			if (auto pColl6 = pLayer->Get_GameObjectFirst(L"FishTankCollider_6"))
			{

			}
			if (auto pColl7 = pLayer->Get_GameObjectFirst(L"FishTankCollider_7"))
			{

			}
			if (auto pColl8 = pLayer->Get_GameObjectFirst(L"FishTankCollider_8"))
			{

			}
			if (auto pColl9 = pLayer->Get_GameObjectFirst(L"FishTankCollider_9"))
			{

			}
			
		}
	}
#endif
	CCameraMgr::GetInstance()->Render_Camera();
	CMapMgr::GetInstance()->Render_Map();


}

void CDive::Frustum() {
	CGameObject* pDiveDave = m_mapLayer[L"0_GameLogic_Layer"]->Get_GameObjectFirst(L"DiveDave");
	CTransform* pDaveTransform = static_cast<CTransform*>(pDiveDave->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_TestCamera", m_pFrustumCollider);

	if(CDInputMgr::GetInstance()->Key_Down(DIK_1)) {
		if (!CColliderMgr::GetInstance()->Get_Render()) {
			CColliderMgr::GetInstance()->Set_Render(true);
			CCameraMgr::GetInstance()->Change_CurCamera(L"FreeCam");


		}
		else {
			CColliderMgr::GetInstance()->Set_Render(false);
			CCameraMgr::GetInstance()->Change_CurCamera(L"ChaseToPlayerCam");

		}
	}


	if (CColliderMgr::GetInstance()->Get_Render()) {
		_matrix CameraView = CCameraMgr::GetInstance()->Get_Camera(L"ChaseToPlayerCam")->Get_ViewMatrix();
		D3DXMatrixInverse(&CameraView, nullptr, &CameraView);
		_matrix MoveMatrix;
		D3DXMatrixIdentity(&MoveMatrix);
		memcpy(&MoveMatrix.m[3][0], &pDaveTransform->Get_World()->m[3][0], sizeof(_vec3));
		MoveMatrix.m[3][2] = CameraView.m[3][2];
		m_pFrustumCollider->Transform(&MoveMatrix);

	}
	else {
		CCameraMgr::GetInstance()->Get_Camera(L"ChaseToPlayerCam")->Update_MatView();
		_matrix CameraView = CCameraMgr::GetInstance()->Get_Camera(L"ChaseToPlayerCam")->Get_ViewMatrix();
		D3DXMatrixInverse(&CameraView, nullptr, &CameraView);

		m_pFrustumCollider->Transform(&CameraView);
	}


	
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
	for (int i = 1; i <= 9; ++i) {
		CFishTankCollider* pFishTankCollider = CFishTankCollider::Create(1.f, 1.f);
		pLayer->Add_GameObject(L"FishTankCollider_" + to_wstring(i), pFishTankCollider);
	}

	


	//CFishTankCollider* pFishTankCollider = CFishTankCollider::Create(L"AABB_FishTank", L"Coll_FishTankFishes");
	//pLayer->Add_GameObject(L"FishTankCollider", pFishTankCollider);

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

	pGameObject = CDaveGoldBall::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DaveGoldBall", pGameObject)))
		return E_FAIL;
	pGameObject->Set_Parent(pDiveDave);

	pGameObject = CDiveDaveGun::Create(CGameMemMgr::CDaveInfo::GUN_DEFAULT);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveDaveGun", pGameObject)))
		return E_FAIL;
	pGameObject->Set_Parent(pDiveDave);



	//-----------------------------------------------------ItemBox---------------------------------------------------------------------
	// ItemBox 생성
	// [KDS] : 박스의 종류와 위치를 인자로 받습니다. DROPITEM의 종류도 넣어 주세요.
	// Add_GameObject 태그를 다르게 해줘야 트랜스폼이 각자마다 적용됩니다.
	pGameObject = CDiveItemBox::Create(ITEMBOXTEX::CHEST_A, 3, 3, 0,DROPITEM::O2CAPSULE);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveItemBox_O2_1", pGameObject)))
		return E_FAIL;

	pGameObject = CDiveItemBox::Create(ITEMBOXTEX::CHEST_A, 3, 3, 0, DROPITEM::O2CAPSULE);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveItemBox_O2_2", pGameObject)))
		return E_FAIL;

	pGameObject = CDiveItemBox::Create(ITEMBOXTEX::CHEST_BOX, 0, 3, 0, DROPITEM::O2CAPSULE);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveItemBox_O2_3", pGameObject)))
		return E_FAIL;

	pGameObject = CDiveItemBox::Create(ITEMBOXTEX::CHEST_BOX, -3, 3,0, DROPITEM::O2CAPSULE);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveItemBox_O2_4", pGameObject)))
		return E_FAIL;
	
	pGameObject = CDiveItemBox::Create(ITEMBOXTEX::CHEST_WEAPON, -6, 3, 0.f, DROPITEM::PENTAAXEL);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"DiveItemBox_PENTAAXEL", pGameObject)))
		return E_FAIL;
	//-----------------------------------------------------ItemBox---------------------------------------------------------------------


	_vec3 vtmp{ -10, 3, 0.f };
	pGameObject = CCommonItemWood::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_Wood", pGameObject)))
		return E_FAIL;
	vtmp = { -10, 5, 0.f };
	pGameObject = CCommonItemWoodPlate::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_WoodPlate", pGameObject)))
		return E_FAIL;
	vtmp = { -9, 5, 0.f };
	pGameObject = CCommonItemWatch::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_Watch", pGameObject)))
		return E_FAIL;
	vtmp = { -8, 5, 0.f };
	pGameObject = CCommonItemBone::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_Bone", pGameObject)))
		return E_FAIL;
	vtmp = { -7, 5, 0.f };
	pGameObject = CCommonItemDeepseaCoral::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_DeepseaCoral", pGameObject)))
		return E_FAIL;
	vtmp = { -6, 5, 0.f };
	pGameObject = CCommonItemFragment::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_Fragment", pGameObject)))
		return E_FAIL;
	vtmp = { -5, 5, 0.f };
	pGameObject = CCommonItemThurible::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_Thurible", pGameObject)))
		return E_FAIL;
	vtmp = { -4, 5, 0.f };
	pGameObject = CCommonItemRope::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_Rope", pGameObject)))
		return E_FAIL;
	vtmp = { -3, 5, 0.f };
	pGameObject = CCommonItemUmb::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_Umb", pGameObject)))
		return E_FAIL;
	vtmp = { 0.f, 0.f, 0.f };
	pGameObject = CAmmoPack::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_AmmoPack0", pGameObject)))
		return E_FAIL;
	pGameObject = CAmmoPack::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_AmmoPack1", pGameObject)))
		return E_FAIL;
	pGameObject = CAmmoPack::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_AmmoPack2", pGameObject)))
		return E_FAIL;
	pGameObject = CAmmoPack::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_AmmoPack3", pGameObject)))
		return E_FAIL;
	pGameObject = CAmmoPack::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_AmmoPack4", pGameObject)))
		return E_FAIL;
	pGameObject = CAmmoPack::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_AmmoPack5", pGameObject)))
		return E_FAIL;
	pGameObject = CAmmoPack::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_AmmoPack6", pGameObject)))
		return E_FAIL;
	pGameObject = CAmmoPack::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_AmmoPack7", pGameObject)))
		return E_FAIL;

	pGameObject = CCommonItemWood::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_Wood_0", pGameObject)))
		return E_FAIL;

	pGameObject = CCommonItemWoodPlate::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_WoodPlate_0", pGameObject)))
		return E_FAIL;

	pGameObject = CCommonItemWatch::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_Watch_0", pGameObject)))
		return E_FAIL;

	pGameObject = CCommonItemBone::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_Bone_0", pGameObject)))
		return E_FAIL;

	pGameObject = CCommonItemDeepseaCoral::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_DeepseaCoral_0", pGameObject)))
		return E_FAIL;

	pGameObject = CCommonItemFragment::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_Fragment_0", pGameObject)))
		return E_FAIL;

	pGameObject = CCommonItemThurible::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_Thurible_0", pGameObject)))
		return E_FAIL;

	pGameObject = CCommonItemRope::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_Rope_0", pGameObject)))
		return E_FAIL;
	vtmp = { -3, 5, 0.f };
	pGameObject = CCommonItemUmb::Create(vtmp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Item_Umb_0", pGameObject)))
		return E_FAIL;
	// 맵 
	pGameObject = CTerrian::Create(L"BackGround_GLB_File");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"BackGround_GLB_File", pGameObject)))
		return E_FAIL;
	dynamic_cast<CTerrian*>(pGameObject)->Set_Frustom(true);
	dynamic_cast<CTerrian*>(pGameObject)->Set_BackGround(true);

	pGameObject = CTerrian::Create(L"GLB_Terrian1", L"Terrian1_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"0GLB_Terrian1", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrian::Create(L"GLB_Terrian2", L"Terrian2_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"0GLB_Terrian2", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrian::Create(L"GLB_Terrian3", L"Terrian3_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"0GLB_Terrian3", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrian::Create(L"GLB_Terrian4", L"Terrian4_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"0GLB_Terrian4", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrian::Create(L"GLB_Terrian5", L"Terrian5_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"0GLB_Terrian5", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrian::Create(L"GLB_Terrian6", L"Terrian6_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"0GLB_Terrian6", pGameObject)))
		return E_FAIL;

	pGameObject = CTerrian::Create(L"GLB_Terrian7", L"Terrian7_Collision");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"0GLB_Terrian7", pGameObject)))
		return E_FAIL;

	//pGameObject = CTerrian::Create(L"GLB_Terrian8", L"Terrian8_Collision");
	//if (nullptr == pGameObject)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"0GLB_Terrian8", pGameObject)))
	//	return E_FAIL;

	//// 보스
	//pGameObject = CJohn::Create(15.f, 10.f, 0.f);
	//if (nullptr == pGameObject)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"John", pGameObject)))
	//	return E_FAIL;

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
	pGameObject = CWPBoxUI::Create(true);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"WPBoxUI1", pGameObject)))
		return E_FAIL;
	static_cast<CDiveDave*>(m_pDive)->Add_Observer(static_cast<IObserver*>(pGameObject)); // 플레이어 관찰

	pGameObject = CWPBoxUI::Create(false);
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

	CWPAmmoCntText * pAmmoCnt = CWPAmmoCntText::Create(0.f, 0.f);
	if (nullptr == pAmmoCnt)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"AmmoCntTextUI", pAmmoCnt)))
		return E_FAIL;
	pAmmoCnt->Set_Opt(DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
	static_cast<CDiveDave*>(m_pDive)->Add_Observer(static_cast<IObserver*>(pAmmoCnt)); // 플레이어 관찰

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
	//pDepthText->Set_Opt();
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

	pGameObject = CSpaceKeyUI::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"SpaceKeyUI", pGameObject)))
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
	Safe_Release(m_pFrustumCollider);
	CSoundMgr::GetInstance()->StopAll();
}

void CDive::DeepDark() {


	_vec3 floor;
	
	CGameObject* pDiveDave = Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"DiveDave");
	CTransform* pDaveTransform = static_cast<CTransform*>(pDiveDave->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	pDaveTransform->Get_Info(INFO_POS, &floor);

	
	float _dark = CGameMemMgr::GetInstance()->Get_Dark();

	if (floor.y <= -80.f && _dark > 0.5f) {
		_dark -= 0.01f;
		CGameMemMgr::GetInstance()->Set_Dark(_dark);
	}
	else if (floor.y > -80.f && _dark <= 0.99f) {
		_dark += 0.01f;
		CGameMemMgr::GetInstance()->Set_Dark(_dark);
	}
	else if (floor.y > -130.f && _dark <= 0.5f) {
		_dark += 0.01f;
		CGameMemMgr::GetInstance()->Set_Dark(_dark);
	}
	else if (floor.y <= -130.f && _dark >= 0.01f) {
		_dark -= 0.01f;
		CGameMemMgr::GetInstance()->Set_Dark(_dark);

	}
}


void CDive::Place_Fish() {
	if (!m_bFishCreate) {
		if (auto pLayer = m_mapLayer[L"2_Fish_Layer"])
		{
			if (auto pColl = pLayer->Get_GameObjectFirst(L"FishTankCollider_1"))
			{



				_vec3 vPos;
				pColl->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

				Fish::AddLayer_Fish<Fish::CBlueTang>(L"BlueTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CBlueTang>(L"BlueTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);
				Fish::AddLayer_Fish<Fish::CBlueTang>(L"BlueTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);
				Fish::AddLayer_Fish<Fish::CBlueTang>(L"BlueTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);
				Fish::AddLayer_Fish<Fish::CBlueTang>(L"BlueTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);
				Fish::AddLayer_Fish<Fish::CBlueTang>(L"BlueTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);
				Fish::AddLayer_Fish<Fish::CBlueTang>(L"BlueTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);
				Fish::AddLayer_Fish<Fish::CBlueTang>(L"BlueTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);
				Fish::AddLayer_Fish<Fish::CBlueTang>(L"BlueTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				//---------------------------------------------------------------------------------------------

				Fish::AddLayer_Fish<Fish::CBlueTang>(L"BlueTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CClownfish>(L"Clownfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				Fish::AddLayer_Fish<Fish::CGreenHumpheadParrotfish>(L"GreenHumpheadParrotfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				Fish::AddLayer_Fish<Fish::CHumboldtSquid>(L"HumboldtSquid", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CJuvenileCircularBatFish>(L"JuvenileCircularBatFish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CRedLionfish>(L"RedLionfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CSmallspottedDart>(L"SmallspottedDart", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CTitanTriggerfish>(L"TitanTriggerfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CYellowbackFusilier>(L"YellowbackFusilier", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CYellowTang>(L"YellowTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);



				
				//---------------------------------------------------------------------------------------------
			}
			if (auto pColl = pLayer->Get_GameObjectFirst(L"FishTankCollider_2"))
			{
				_vec3 vPos;
				pColl->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

				Fish::AddLayer_Fish<Fish::CBlueTang>(L"BlueTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CClownfish>(L"Clownfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				Fish::AddLayer_Fish<Fish::CGreenHumpheadParrotfish>(L"GreenHumpheadParrotfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				Fish::AddLayer_Fish<Fish::CHumboldtSquid>(L"HumboldtSquid", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CJuvenileCircularBatFish>(L"JuvenileCircularBatFish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CRedLionfish>(L"RedLionfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CSmallspottedDart>(L"SmallspottedDart", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CTitanTriggerfish>(L"TitanTriggerfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CYellowbackFusilier>(L"YellowbackFusilier", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CYellowTang>(L"YellowTang", pLayer, vPos.x, vPos.y, 0.3f, pColl); Fish::AddLayer_Fish<Fish::CBlueTang>(L"BlueTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CClownfish>(L"Clownfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				Fish::AddLayer_Fish<Fish::CGreenHumpheadParrotfish>(L"GreenHumpheadParrotfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				Fish::AddLayer_Fish<Fish::CHumboldtSquid>(L"HumboldtSquid", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CJuvenileCircularBatFish>(L"JuvenileCircularBatFish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CRedLionfish>(L"RedLionfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CSmallspottedDart>(L"SmallspottedDart", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CTitanTriggerfish>(L"TitanTriggerfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CYellowbackFusilier>(L"YellowbackFusilier", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CYellowTang>(L"YellowTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);
			
			}
			if (auto pColl = pLayer->Get_GameObjectFirst(L"FishTankCollider_3"))
			{
				_vec3 vPos;
				pColl->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

				Fish::AddLayer_Fish<Fish::CBlueTang>(L"BlueTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CClownfish>(L"Clownfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				Fish::AddLayer_Fish<Fish::CGreenHumpheadParrotfish>(L"GreenHumpheadParrotfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				Fish::AddLayer_Fish<Fish::CHumboldtSquid>(L"HumboldtSquid", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CJuvenileCircularBatFish>(L"JuvenileCircularBatFish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CRedLionfish>(L"RedLionfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CSmallspottedDart>(L"SmallspottedDart", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CTitanTriggerfish>(L"TitanTriggerfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CYellowbackFusilier>(L"YellowbackFusilier", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CYellowTang>(L"YellowTang", pLayer, vPos.x, vPos.y, 0.3f, pColl);
			}
			if (auto pColl = pLayer->Get_GameObjectFirst(L"FishTankCollider_4"))
			{
				_vec3 vPos;
				pColl->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

				Fish::AddLayer_Fish<Fish::CGiantTrevally>(L"GiantTrevally", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CGiantTrevally>(L"GiantTrevally", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CGiantTrevally>(L"GiantTrevally", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CGreatSpiderCrab>(L"GreatSpiderCrab", pLayer, vPos.x, vPos.y, 0.3f, pColl);
				Fish::AddLayer_Fish<Fish::CGreatSpiderCrab>(L"GreatSpiderCrab", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				Fish::AddLayer_Fish<Fish::CGreyTriggerfish>(L"GreyTriggerfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CGreyTriggerfish>(L"GreyTriggerfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				Fish::AddLayer_Fish<Fish::CHumboldtSquid>(L"HumboldtSquid", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CHumboldtSquid>(L"HumboldtSquid", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				Fish::AddLayer_Fish<Fish::CSmallspottedDart>(L"SmallspottedDart", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				Fish::AddLayer_Fish<Fish::CSmallspottedDart>(L"SmallspottedDart", pLayer, vPos.x, vPos.y, 0.3f, pColl);

			
			}
			if (auto pColl = pLayer->Get_GameObjectFirst(L"FishTankCollider_5"))
			{
				_vec3 vPos;
				pColl->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);


				Fish::AddLayer_Fish<Fish::CGreyTriggerfish>(L"GreyTriggerfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CGreyTriggerfish>(L"GreyTriggerfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				Fish::AddLayer_Fish<Fish::CDevilScorpionFish>(L"DevilScorpionFish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CGiantTrevally>(L"GiantTrevally", pLayer, vPos.x, vPos.y, 0.3f, pColl);



				Fish::AddLayer_Fish<Fish::CDevilScorpionFish>(L"DevilScorpionFish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CGiantTrevally>(L"GiantTrevally", pLayer, vPos.x, vPos.y, 0.3f, pColl);

			}
			if (auto pColl = pLayer->Get_GameObjectFirst(L"FishTankCollider_6"))
			{
				_vec3 vPos;
				pColl->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);


				Fish::AddLayer_Fish<Fish::CGreyTriggerfish>(L"GreyTriggerfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CHumboldtSquid>(L"HumboldtSquid", pLayer, vPos.x, vPos.y, 0.3f, pColl);

			}
			if (auto pColl = pLayer->Get_GameObjectFirst(L"FishTankCollider_7"))
			{
				_vec3 vPos;
				pColl->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);


				Fish::AddLayer_Fish<Fish::CGreyTriggerfish>(L"GreyTriggerfish", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CHumboldtSquid>(L"HumboldtSquid", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CAsianSheepshead>(L"Asiansheepshead", pLayer, vPos.x, vPos.y, 0.3f, pColl);
			}
			if (auto pColl = pLayer->Get_GameObjectFirst(L"FishTankCollider_8"))
			{
				_vec3 vPos;
				pColl->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);


				Fish::AddLayer_Fish<Fish::CGiantTrevally>(L"GiantTrevally", pLayer, vPos.x, vPos.y, 0.3f, pColl);
				Fish::AddLayer_Fish<Fish::CGiantTrevally>(L"GiantTrevally", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CGiantTrevally>(L"GiantTrevally", pLayer, vPos.x, vPos.y, 0.3f, pColl);
		
			}
			if (auto pColl = pLayer->Get_GameObjectFirst(L"FishTankCollider_9"))
			{
				_vec3 vPos;
				pColl->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

				Fish::AddLayer_Fish<Fish::CHumboldtSquid>(L"HumboldtSquid", pLayer, vPos.x, vPos.y, 0.3f, pColl);
				Fish::AddLayer_Fish<Fish::CHumboldtSquid>(L"HumboldtSquid", pLayer, vPos.x, vPos.y, 0.3f, pColl);

				Fish::AddLayer_Fish<Fish::CAsianSheepshead>(L"Asiansheepshead", pLayer, vPos.x, vPos.y, 0.3f, pColl);


				Fish::AddLayer_Fish<Fish::CAsianSheepshead>(L"Asiansheepshead", pLayer, vPos.x, vPos.y, 0.3f, pColl);
		
			}

		}

		m_bFishCreate = true;
	}
	
}
