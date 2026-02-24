#include "CShip.h"
#include "CAssetMgr.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CTransition.h"
#include "CShipDave.h"
#include "CFreeCam.h"
#include "CCameraMgr.h"
#include "CColliderMgr.h"
#include "CGraphicDev.h"
#include "CShipBoat.h"
#include "CShipDiverBoxInventory.h"
#include "CShipPhoneIcon.h"
#include "CShipPhone.h"
#include "CShipPhoneApp.h"
#include "CTestAmericanLobster.h"
#include "CTestDancing.h"
#include "CShipDiverBox.h"
#include "CShipPhoneIDiverBG.h"
#include "CShipTransformCam.h"
#include "CShipPhoneIDiverItem.h"
#include "CShipPhoneIDiverUpgrade.h"
#include "CShipBG.h"
#include "CShipFakeBG.h"
#include "CShipUIDiveBtn.h"
#include "CTriggerAABB.h"
#include "CShipUIGoBtn.h"
#include "CLog.h"
#include "CShipUISpaceKey.h"
#include "CDInputMgr.h"
#include "CShipUIMoney.h"
#include "CShipDiverBoxInventoryEdge.h"

#include "IDiverInfo.h"

#include "FishInclude.h"


CShip::CShip()
	: CScene()
{
}
CShip::~CShip()
{
}

HRESULT CShip::Ready_Scene()
{
	CColliderMgr::GetInstance()->Set_Render(false);

	if (FAILED(Ready_GameLogic_Layer(L"0_GameLogic_Layer")))
		return E_FAIL;

	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	_matrix	matView, matProj;


	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	
	IDiver::InitIDiverInfo();

	return S_OK;
}

HRESULT CShip::Ready_GameLogic_Layer(wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	//{
	//	Fish::CBlueTang* pFish = Fish::CBlueTang::Create(2.f, 2.f);
	//	pLayer->Add_GameObject(L"Fishes", pFish);
	//}

	//{
	//	Fish::CYellowTang* pFish = Fish::CYellowTang::Create(-2.f, -2.f);
	//	pLayer->Add_GameObject(L"Fishes", pFish);
	//}

	//{
	//	Fish::CSmallspottedDart* pFish = Fish::CSmallspottedDart::Create(0.f, 0.f);
	//	pLayer->Add_GameObject(L"Fishes", pFish);
	//}

	//{
	//	Fish::CClownfish* pFish = Fish::CClownfish::Create(2.f, -2.f);
	//	pLayer->Add_GameObject(L"Fishes", pFish);
	//}

	//{
	//	Fish::CYellowbackFusilier* pFish = Fish::CYellowbackFusilier::Create(-2.f, 2.f);
	//	pLayer->Add_GameObject(L"Fishes", pFish);
	//}

	//{
	//	CFishBlueTang* pBlueTang = CFishBlueTang::Create(-1.f, -1.f);
	//	pLayer->Add_GameObject(L"BlueTang", pBlueTang);
	//}

	//{
	//	CFishBlueTang* pBlueTang = CFishBlueTang::Create(-1.f, 0.f);
	//	pLayer->Add_GameObject(L"BlueTang", pBlueTang);
	//}

	//CTestAmericanLobster* pTest = CTestAmericanLobster::Create();
	//if (nullptr == pTest)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"ShipTestLobster", pTest)))
	//	return E_FAIL;

	//CTestDancing* pDancing = CTestDancing::Create();
	//if (nullptr == pDancing)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"ShipestDancing", pDancing)))
	//	return E_FAIL;

	CShipTransformCam* pCam = CShipTransformCam::Create();
	if (nullptr == pCam)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipCam", pCam)))
		return E_FAIL;


	CShipDave* pShipDave = CShipDave::Create();
	if (nullptr == pShipDave)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipDave", pShipDave)))
		return E_FAIL;






	//CShipBoat* pShipBoat = CShipBoat::Create();
	//if (nullptr == pShipBoat)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"ShipBoat", pShipBoat)))
	//	return E_FAIL;

	//CShipDiverBox* pShipDiverBox = CShipDiverBox::Create();
	//if (nullptr == pShipDiverBox)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"ShipDiverBox", pShipDiverBox)))
	//	return E_FAIL;


	CShipPhoneIcon* pShipPhoneIcon = CShipPhoneIcon::Create();
	if (nullptr == pShipPhoneIcon)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipPhoneIcon", pShipPhoneIcon)))
		return E_FAIL;

	//CShipPhoneIDiverItem
	//CShipPhoneIDiverItem* pShipItem = CShipPhoneIDiverItem::Create();
	//if (nullptr == pShipItem)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"ShipPhoneIDIverItem", pShipItem)))
	//	return E_FAIL;

	// CShipPhoneIDiverUpgrade

	//CShipPhoneIDiverUpgrade* pShipIDiverUpgrade = CShipPhoneIDiverUpgrade::Create(0.f, 0.f);
	//if (nullptr == pShipIDiverUpgrade)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"ShipPhoneUpgrade", pShipIDiverUpgrade)))
	//	return E_FAIL;

	//CShipBG
	CShipBG* pShipBG = CShipBG::Create(0.f, 0.f);
	if (nullptr == pShipBG)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipBG", pShipBG)))
		return E_FAIL;

	CShipFakeBG* pShipFakeBG = CShipFakeBG::Create(0.f, 0.f);
	if (nullptr == pShipFakeBG)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipFakeBG", pShipFakeBG)))
		return E_FAIL;

	//CShipUIMoney
	CShipUIMoney* pShipUIMoney = CShipUIMoney::Create(-7.5f, 4.5f);
	if (nullptr == pShipUIMoney)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipUIMoney", pShipUIMoney)))
		return E_FAIL;

	//CShipUIDiveBtn
	CShipUIDiveBtn* pShipDiveBtn = CShipUIDiveBtn::Create(-2.f, -2.f);
	if (nullptr == pShipDiveBtn)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipDiveBtn", pShipDiveBtn)))
		return E_FAIL;


	//CShipUIGoBtn
	CShipUIGoBtn* pShipGoBoatBtn = CShipUIGoBtn::Create(-6.f, -1.3f);
	if (nullptr == pShipGoBoatBtn)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipGoBtn", pShipGoBoatBtn)))
		return E_FAIL;


	// CShipUISpaceKey
	CShipUISpaceKey* pShipSpace = CShipUISpaceKey::Create(-3.5f, -2.5f);
	if (nullptr == pShipSpace)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipDiverBoxSpace", pShipSpace)))
		return E_FAIL;

	CTriggerAABB* pTriggerDive = CTriggerAABB::Create(L"Coll_DiveTrigger", L"Coll_ShipDave");
	if (nullptr == pTriggerDive)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipDiveTrigger", pTriggerDive)))
		return E_FAIL;
	{
		pTriggerDive->Get_Transform()->Set_Pos(-1.5f, -2.5f, 0.f);
		pTriggerDive->Set_OnTriggerEnter([](CCollider* pCollider) {
			if (pCollider->Get_Tag() == L"AABB_Dave")
			{
				if (auto pBtn = CManagement::GetInstance()
					->Get_Scene()
					->Get_Layer(L"0_GameLogic_Layer")
					->Get_GameObjectFirst<CShipUIDiveBtn>(L"ShipDiveBtn"))
				{
					pBtn->AddRender(true);
				}
			}
			});
		pTriggerDive->Set_OnTriggerExit([](CCollider* pCollider) {
			if (pCollider->Get_Tag() == L"AABB_Dave")
			{
				if (auto pBtn = CManagement::GetInstance()
					->Get_Scene()
					->Get_Layer(L"0_GameLogic_Layer")
					->Get_GameObjectFirst<CShipUIDiveBtn>(L"ShipDiveBtn"))
				{
					pBtn->AddRender(false);
				}
			}
			});
	}

	CTriggerAABB* pTriggerDiverBox = CTriggerAABB::Create(L"Coll_DiverBoxTrigger", L"Coll_ShipDave");
	if (nullptr == pTriggerDiverBox)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipDiverBoxTrigger", pTriggerDiverBox)))
		return E_FAIL;
	{
		pTriggerDiverBox->Get_Transform()->Set_Pos(-3.5f, -2.5f, 0.f);
		_vec3 vScale = { 0.1, 1.f, 1.f };
		pTriggerDiverBox->Get_Transform()->Set_Scale(&vScale);
		pTriggerDiverBox->Set_OnTriggerEnter([](CCollider* pCollider) {
			if (pCollider->Get_Tag() == L"AABB_Dave")
			{
				if (auto pBtn = CManagement::GetInstance()
					->Get_Scene()
					->Get_Layer(L"0_GameLogic_Layer")
					->Get_GameObjectFirst<CShipUISpaceKey>(L"ShipDiverBoxSpace"))
				{
					pBtn->AddRender(true);
				}
			}
			});
		pTriggerDiverBox->Set_OnTriggerStay([](CCollider* pCollider) {
			if (pCollider->Get_Tag() == L"AABB_Dave")
			{
				if (CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
				{
					auto pExists = CManagement::GetInstance()
						->Get_Scene()
						->Get_Layer(L"0_GameLogic_Layer")
						->Get_GameObjectFirst(L"ShipDiverBoxInventory");
					if (!pExists)
					{
						CManagement::GetInstance()
							->Get_Scene()
							->Get_Layer(L"0_GameLogic_Layer")
							->Add_GameObject(L"ShipDiverBoxInventory", CShipDiverBoxInventory::Create());
					}
				}
			}
			});
		pTriggerDiverBox->Set_OnTriggerExit([](CCollider* pCollider) {
			if (pCollider->Get_Tag() == L"AABB_Dave")
			{
				if (auto pBtn = CManagement::GetInstance()
					->Get_Scene()
					->Get_Layer(L"0_GameLogic_Layer")
					->Get_GameObjectFirst<CShipUISpaceKey>(L"ShipDiverBoxSpace"))
				{
					pBtn->AddRender(false);
				}
			}
			});
	}

	CTriggerAABB* pTriggerGoBoat = CTriggerAABB::Create(L"Coll_GoBoatTrigger", L"Coll_ShipDave");
	if (nullptr == pTriggerGoBoat)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipGoBoatTrigger", pTriggerGoBoat)))
		return E_FAIL;
	{
		pTriggerGoBoat->Get_Transform()->Set_Pos(-6.5f, -2.5f, 0.f);
		_vec3 vScale = { 0.3, 1.f, 1.f };
		pTriggerGoBoat->Get_Transform()->Set_Scale(&vScale);
		pTriggerGoBoat->Set_OnTriggerEnter([](CCollider* pCollider) {
			if (pCollider->Get_Tag() == L"AABB_Dave")
			{
				if (auto pBtn = CManagement::GetInstance()
					->Get_Scene()
					->Get_Layer(L"0_GameLogic_Layer")
					->Get_GameObjectFirst<CShipUIGoBtn>(L"ShipGoBtn"))
				{
					pBtn->SetActive(true);
				}
			}
			});
		pTriggerGoBoat->Set_OnTriggerExit([](CCollider* pCollider) {
			if (pCollider->Get_Tag() == L"AABB_Dave")
			{
				if (auto pBtn = CManagement::GetInstance()
					->Get_Scene()
					->Get_Layer(L"0_GameLogic_Layer")
					->Get_GameObjectFirst<CShipUIGoBtn>(L"ShipGoBtn"))
				{
					pBtn->SetActive(false);
				}
			}
			});
	}




	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

_int CShip::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	ImGui::Begin("Curr Scene: CShip");
	if (ImGui::Button("Go Dive Scene"))
	{
		//CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_SHIP, CTransition::SCENE_DIVE));
		CTransition::FadedTransition(CTransition::SCENE_SHIP, CTransition::SCENE_DIVE);
	}
	if (ImGui::Button("Go Sushi Scene"))
	{
		//CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_SHIP, CTransition::SCENE_SUSHI));
		CTransition::FadedTransition(CTransition::SCENE_SHIP, CTransition::SCENE_SUSHI);
	}
	if (ImGui::Button("Go Logo Scene"))
	{
		CTransition::FadedTransition(CTransition::SCENE_SHIP, CTransition::SCENE_LOGO);
		//CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_SHIP, CTransition::SCENE_LOGO));
	}
	ImGui::End();

	if (ImGui::Button("Collider Render"))
	{
		CColliderMgr::GetInstance()->Set_Render(!CColliderMgr::GetInstance()->Get_Render());
	}
	
	return iExit;
}

void CShip::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CShip::Render_Scene()
{
	_vec2	vPos{ 0.f, 0.f };
	CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Default");
	pDefFont->Render_Font(L"Here is CShip", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

CShip* CShip::Create()
{
	CShip* pShip = new CShip;

	if (FAILED(pShip->Ready_Scene()))
	{
		MSG_BOX("CShip Create Failed");
		Safe_Release(pShip);
		return nullptr;
	}
	return pShip;
}

void CShip::Free()
{
	CScene::Free();
	CColliderMgr::GetInstance()->Clear_ColliderGroup();
}
