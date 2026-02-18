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
#include "CShipDiverBox.h"
#include "CShipPhoneIcon.h"
#include "CShipPhone.h"
#include "CShipPhoneApp.h"
#include "CTestAmericanLobster.h"
#include "CTestDancing.h"

CShip::CShip()
	: CScene()
{
}
CShip::~CShip()
{
}

HRESULT CShip::Ready_Scene()
{
	CColliderMgr::GetInstance()->Set_Render(true);

	if (FAILED(Ready_GameLogic_Layer(L"0_GameLogic_Layer")))
		return E_FAIL;

	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	//// 임시 카메라
	_vec3	vEye{ 0.f, 0.f, -10.f };
	_vec3	vAt{ 0.f, 0.f, 0.f };
	_vec3	vUp{ 0.f, 1.f, 0.f };
	_matrix	matView, matProj;

	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
	
	//// TestCam1
	//CCamera* pCamera = CFreeCam::Create(&vEye, &vAt, &vUp, D3DXToRadian(60.f), (_float)WINCX / WINCY, 1.f, 1000.f);
	//if (nullptr == pCamera)
	//	return E_FAIL;
	//CCameraMgr::GetInstance()->Set_Camera(L"TestCam1", pCamera);

	return S_OK;
}

HRESULT CShip::Ready_GameLogic_Layer(wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CTestAmericanLobster* pTest = CTestAmericanLobster::Create();
	if (nullptr == pTest)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipTestLobster", pTest)))
		return E_FAIL;

	CTestDancing* pDancing = CTestDancing::Create();
	if (nullptr == pDancing)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipestDancing", pDancing)))
		return E_FAIL;

	CShipDave* pShipDave = CShipDave::Create();
	if (nullptr == pShipDave)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipDave", pShipDave)))
		return E_FAIL;




	CShipBoat* pShipBoat = CShipBoat::Create();
	if (nullptr == pShipBoat)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipBoat", pShipBoat)))
		return E_FAIL;

	//pGameObject = CShipDiverBox::Create();
	//if (nullptr == pGameObject)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"ShipDiverBox", pGameObject)))
	//	return E_FAIL;

	CShipPhoneIcon* pShipPhoneIcon = CShipPhoneIcon::Create();
	if (nullptr == pShipPhoneIcon)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"ShipPhoneIcon", pShipPhoneIcon)))
		return E_FAIL;

	


	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

_int CShip::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	ImGui::Begin("Curr Scene: CShip");
	if (ImGui::Button("Go Dive Scene"))
	{
		CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_SHIP, CTransition::SCENE_DIVE));
	}
	if (ImGui::Button("Go Sushi Scene"))
	{
		CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_SHIP, CTransition::SCENE_SUSHI));
	}
	if (ImGui::Button("Go Logo Scene"))
	{
		CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_SHIP, CTransition::SCENE_LOGO));
	}
	ImGui::End();
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
