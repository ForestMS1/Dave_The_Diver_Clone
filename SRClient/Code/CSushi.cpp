#include "CSushi.h"
#include "CAssetMgr.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CTransition.h"
#include "CGraphicDev.h"
#include "CBackground.h"
#include "CSakura.h"
#include "CSushiCamera.h"
#include "CRoof.h"
#include "CFence.h"
#include "CWood.h"
#include "CHood.h"
#include "CTable.h"
#include "CMenu.h"
#include "CFishTank.h"
#include "CPatternFrame.h"
#include "CSign.h"
#include "CSushiLight.h"
#include "CChair.h"
#include "COpen.h"
#include "CKitchen.h"
#include "CSpeaker.h"
#include "CBancho.h"
#include "CSushiDave.h"
#include "CCustomer1.h"
#include "CColliderMgr.h"
#include "CMenuFrame.h"
#include "CDInputMgr.h"

CGameObject* g_pObject = nullptr;

CSushi::CSushi()
	: CScene()
{
}
CSushi::~CSushi()
{
}

HRESULT CSushi::Ready_Scene()
{

	if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
		return E_FAIL;

	if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
		return E_FAIL;

	if (FAILED(Ready_UI_Layer(L"UI_Layer")))
		return E_FAIL;

	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	//pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);      
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);       
	//pGraphicDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);   

	return S_OK;

}

_int CSushi::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	ImGui::Begin("Curr Scene: CSushi");
	if (ImGui::Button("Go Ship Scene"))
	{
		CManagement::GetInstance()->Set_Scene(CTransition::Create(CTransition::SCENE_SUSHI, CTransition::SCENE_SHIP));
	}
	ImGui::End();
	Key_Input();
	return iExit;
}

void CSushi::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CSushi::Render_Scene()
{

	_vec2	vPos{ 0.f, 0.f };
	CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Default");
	pDefFont->Render_Font(L"Here is CSushi", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


	//if (g_pObject != nullptr)
	//{

	//	CTransform* pTransform = static_cast<CTransform*>(g_pObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	//	if (pTransform != nullptr)
	//	{
	//		ImGui::Begin("Transform Inspector");

	//		// Position �Է�
	//		ImGui::InputFloat3("Position", (float*)&pTransform->m_vInfo[INFO_POS]);

	//		// Rotation �Է�
	//		ImGui::InputFloat3("Rotation", (float*)&pTransform->m_vAngle);

	//		// Scale �Է�
	//		ImGui::InputFloat3("Scale", (float*)&pTransform->m_vScale);

	//		ImGui::End();
	//	}

	//	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	//	ImGuiIO& io = ImGui::GetIO();
	//	ImGuizmo::SetDrawlist();
	//	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	//	//ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
	//	_matrix matView;
	//	_matrix matProj;
	//	_matrix* matWorld;
	//	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//	pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	//	matWorld = static_cast<CTransform*>(g_pObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_World();
	//	if (matWorld == nullptr) {
	//		MSG_BOX("���� ��� ����");
	//	}
	//	float* view = (float*)&matView;
	//	float* proj = (float*)&matProj;
	//	//* model = (float*)matWorld;

	//	ImGuizmo::Manipulate(
	//		view,
	//		proj,
	//		m_CurrentGizmoOperation,
	//		ImGuizmo::WORLD,
	//		(float*)matWorld
	//	);

	//	if (ImGuizmo::IsUsing())
	//	{
	//		float vPos[3], vRot[3], vScale[3];

	//		ImGuizmo::DecomposeMatrixToComponents((float*)matWorld, vPos, vRot, vScale);


	//		CTransform* pTransform = static_cast<CTransform*>(g_pObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	//		pTransform->m_vInfo[INFO_POS] = { vPos[0], vPos[1], vPos[2] };
	//		pTransform->m_vAngle = { vRot[0], vRot[1], vRot[2] };
	//		pTransform->m_vScale = { vScale[0], vScale[1], vScale[2] };
	//	}
	//}
	////ImGui::End();


	//ImGui::Begin("Scene Hierarchy");
	//for (auto& LayerIter : m_mapLayer)
	//{
	//	for (auto& ObjListIter : *LayerIter.second->Get_GameObjects())
	//	{
	//		for (auto& Obj : ObjListIter.second)
	//		{
	//			if (ImGui::Selectable(to_string((_int)Obj).c_str(), g_pObject == Obj))
	//			{
	//				g_pObject = Obj;
	//			}
	//		}
	//	}
	//}
	//ImGui::End();

}

CSushi* CSushi::Create()
{
	CSushi* pSushi = new CSushi;

	if (FAILED(pSushi->Ready_Scene()))
	{
		MSG_BOX("CSushi Create Failed");
		Safe_Release(pSushi);
		return nullptr;
	}
	return pSushi;
}

HRESULT CSushi::Ready_Environment_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;
	
	pGameObject = CBackground::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Background", pGameObject)))
		return E_FAIL;

	pGameObject = CSakura::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Sakura", pGameObject)))
		return E_FAIL;


	pGameObject = CRoof::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Roof", pGameObject)))
		return E_FAIL;

	pGameObject = CFence::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Fence", pGameObject)))
		return E_FAIL;


	pGameObject = CHood::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Hood", pGameObject)))
		return E_FAIL;

	pGameObject = CTable::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Table", pGameObject)))
		return E_FAIL;


	pGameObject = CWood::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Wood", pGameObject)))
		return E_FAIL;

	pGameObject = CMenu::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Menu", pGameObject)))
		return E_FAIL;

	pGameObject = CFishTank::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"FishTank", pGameObject)))
		return E_FAIL;

	pGameObject = CPatternFrame::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Frame", pGameObject)))
		return E_FAIL;
	
	pGameObject = CSign::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Sign", pGameObject)))
		return E_FAIL;

	pGameObject = COpen::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Open", pGameObject)))
		return E_FAIL;
	pGameObject = CKitchen::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Kitchen", pGameObject)))
		return E_FAIL;

	pGameObject = CBancho::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Bancho", pGameObject)))
		return E_FAIL;
	for (int i = 0; i < 4; i++) {
		pGameObject = CSushiLight::Create();
		CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		pTransform->m_vScale = { 0.4f,1.3f,1.f };
		pTransform->m_vInfo[INFO_POS] = {-4.8f + (3.f*i),3.f,-3.f};
		if (nullptr == pGameObject)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(L"Light", pGameObject)))
			return E_FAIL;
	}
	
	for (int i = 0; i < 6; i++) {
		pGameObject = CChair::Create();
		CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		pTransform->m_vScale = { 0.3f,0.3f,1.f };
		pTransform->m_vInfo[INFO_POS] = { -3.4f + (1.5f * i),-2.4f,-3.f };
		if (nullptr == pGameObject)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(L"Chair", pGameObject)))
			return E_FAIL;
	}


	for (int i = 0; i < 2; i++) {
		pGameObject = CSpeaker::Create();
		CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		pTransform->m_vScale = { 0.4f,0.5f,1.f };
		pTransform->m_vInfo[INFO_POS] = { -2.5f + (6.7f * i),0.8f,-3.f };
		if (nullptr == pGameObject)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(L"Speaker", pGameObject)))
			return E_FAIL;
	}
	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;

}

HRESULT CSushi::Ready_GameLogic_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;


	pGameObject = CSushiDave::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Dave", pGameObject)))
		return E_FAIL;

	for (int i = 0; i < 5; i++) {
		pGameObject = CCustomer1::Create();
		if (nullptr == pGameObject)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(L"Customer", pGameObject)))
			return E_FAIL;
	}
	


	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });
	return S_OK;
}

HRESULT CSushi::Ready_UI_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	_vec3 vEye{ 0.f, 0.f, -10.f };   
	_vec3 vAt{ 0.f, 0.f, 0.f };      
	_vec3 vUp{ 0.f, 1.f, 0.f };      

	pGameObject = CSushiCamera::Create(&vEye, &vAt, &vUp);

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Camera", pGameObject)))
		return E_FAIL;

	pGameObject = CMenuFrame::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"MenuFrame", pGameObject)))
		return E_FAIL;


	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });
	return S_OK;
}

void CSushi::Key_Input()
{
	if (CDInputMgr::GetInstance()->Key_Up(DIKEYBOARD_I))
	{
		list<CGameObject*>* button = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"MenuFrame");
		list<CGameObject*>::iterator iter = button->begin();
		for (iter; iter != button->end(); iter++) {
			static_cast<CMenuFrame*>(*iter)->Show();
		}
	}
	if (CDInputMgr::GetInstance()->Key_Up(DIKEYBOARD_O))
	{
		list<CGameObject*>* button = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"MenuFrame");
		list<CGameObject*>::iterator iter = button->begin();
		for (iter; iter != button->end(); iter++) {
			static_cast<CMenuFrame*>(*iter)->Hide();
		}
	}
}

void CSushi::Free()
{
	CScene::Free();
	
}
