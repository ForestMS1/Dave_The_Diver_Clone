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
#include "CLight.h"
#include "CChair.h"
#include "COpen.h"
#include "CKitchen.h"
#include "CSpeaker.h"

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
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);       // 깊이 테스트 켜기
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);        // 깊이 버퍼 쓰기 허용
	//pGraphicDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);   // 가까운 픽셀 우선

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


	if (g_pObject != nullptr)
	{

		CTransform* pTransform = static_cast<CTransform*>(g_pObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		if (pTransform != nullptr)
		{
			ImGui::Begin("Transform Inspector");

			// Position 입력
			ImGui::InputFloat3("Position", (float*)&pTransform->m_vInfo[INFO_POS]);

			// Rotation 입력
			ImGui::InputFloat3("Rotation", (float*)&pTransform->m_vAngle);

			// Scale 입력
			ImGui::InputFloat3("Scale", (float*)&pTransform->m_vScale);

			ImGui::End();
		}

		LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		//ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
		_matrix matView;
		_matrix matProj;
		_matrix* matWorld;
		pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

		matWorld = static_cast<CTransform*>(g_pObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_World();
		if (matWorld == nullptr) {
			MSG_BOX("월드 행렬 없음");
		}
		float* view = (float*)&matView;
		float* proj = (float*)&matProj;
		//* model = (float*)matWorld;

		ImGuizmo::Manipulate(
			view,
			proj,
			m_CurrentGizmoOperation,
			ImGuizmo::WORLD,
			(float*)matWorld
		);

		if (ImGuizmo::IsUsing())
		{
			float vPos[3], vRot[3], vScale[3];

			ImGuizmo::DecomposeMatrixToComponents((float*)matWorld, vPos, vRot, vScale);


			CTransform* pTransform = static_cast<CTransform*>(g_pObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
			pTransform->m_vInfo[INFO_POS] = { vPos[0], vPos[1], vPos[2] };
			pTransform->m_vAngle = { vRot[0], vRot[1], vRot[2] };
			pTransform->m_vScale = { vScale[0], vScale[1], vScale[2] };
		}
	}
	//ImGui::End();


	ImGui::Begin("Scene Hierarchy");
	for (auto& LayerIter : m_mapLayer)
	{
		for (auto& ObjListIter : *LayerIter.second->Get_GameObjects())
		{
			for (auto& Obj : ObjListIter.second)
			{
				if (ImGui::Selectable(to_string((_int)Obj).c_str(), g_pObject == Obj))
				{
					g_pObject = Obj;
				}
			}
		}
	}
	ImGui::End();

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
	//배경
	pGameObject = CBackground::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Background", pGameObject)))
		return E_FAIL;

	//벗꽃
	pGameObject = CSakura::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Sakura", pGameObject)))
		return E_FAIL;

	//지붕
	pGameObject = CRoof::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Roof", pGameObject)))
		return E_FAIL;

	//울타리
	pGameObject = CFence::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Fence", pGameObject)))
		return E_FAIL;

	//후드
	pGameObject = CHood::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Hood", pGameObject)))
		return E_FAIL;
	//테이블
	pGameObject = CTable::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Table", pGameObject)))
		return E_FAIL;

	//나무 기둥
	pGameObject = CWood::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Wood", pGameObject)))
		return E_FAIL;
	//메뉴	
	pGameObject = CMenu::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Menu", pGameObject)))
		return E_FAIL;
	//피시 탱크	
	pGameObject = CFishTank::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"FishTank", pGameObject)))
		return E_FAIL;
	//패턴 프레임
	pGameObject = CPatternFrame::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Frame", pGameObject)))
		return E_FAIL;
	//사인 펫말
	pGameObject = CSign::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Sign", pGameObject)))
		return E_FAIL;
	//오픈 사인
	pGameObject = COpen::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Open", pGameObject)))
		return E_FAIL;
	//주방	
	pGameObject = CKitchen::Create();
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Kitchen", pGameObject)))
		return E_FAIL;
	//불빛

	for (int i = 0; i < 4; i++) {
		pGameObject = CLight::Create();
		CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		pTransform->m_vScale = { 0.4f,1.3f,1.f };
		pTransform->m_vInfo[INFO_POS] = {-4.8f + (3.f*i),3.f,-3.f};
		if (nullptr == pGameObject)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(L"Light", pGameObject)))
			return E_FAIL;
	}
	//의자
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

	//스피커
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
	return S_OK;
}

HRESULT CSushi::Ready_UI_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	_vec3 vEye{ 0.f, 0.f, -10.f };   // 카메라 위치
	_vec3 vAt{ 0.f, 0.f, 0.f };      // 바라보는 대상
	_vec3 vUp{ 0.f, 1.f, 0.f };      // 위 방향

	pGameObject = CSushiCamera::Create(&vEye, &vAt, &vUp);

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Camera", pGameObject)))
		return E_FAIL;

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });
	return S_OK;
}

void CSushi::Free()
{
	CScene::Free();
}
