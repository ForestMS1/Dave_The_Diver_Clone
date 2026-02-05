#include "pch.h"
#include "CMapEditor.h"

#include "CProtoMgr.h"
#include "CDynamicCamera.h"
 
#include "CLightMgr.h"
#include "CMapEditorTerrain.h"
#include "CMiniMapTerrain.h"
#include "CManagement.h"


CMapEditor::CMapEditor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CMapEditor::~CMapEditor()
{
}

HRESULT CMapEditor::Ready_Scene()
{
	for(int i =0; i<15; ++i){
		vecMiniMap[i].resize(15);
	}

	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
		return E_FAIL;

	if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
		return E_FAIL;

	if (FAILED(Ready_UI_Layer(L"UI_Layer")))
		return E_FAIL;


	return S_OK;
}

_int CMapEditor::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	return iExit;
}

void CMapEditor::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CMapEditor::Render_Scene()
{
	// debug ¿ë Ãâ·Â
	Show_Position();
}

HRESULT CMapEditor::Ready_Environment_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	_vec3	vEye{ 100.f, 250.f, 100.f };
	_vec3	vAt{ 100.f, 50.f, 100.f };
	_vec3	vUp{ 0.f, 0.f, 1.f };

	// DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &vEye, &vAt, &vUp);

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"DynamicCamera", pGameObject)))
		return E_FAIL;

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

HRESULT CMapEditor::Ready_GameLogic_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;


	// MapEditorTerrain

	pGameObject = CMapEditorTerrain::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"MapEditorTerrain", pGameObject)))
		return E_FAIL;




	for (int i = 0; i < 15; ++i) {
		for (int j = 0; j < 15; ++j) {
	
			// MiniMapTerrain
			pGameObject = CMiniMapTerrain::Create(m_pGraphicDev);

			if (nullptr == pGameObject)
				return E_FAIL;

			if (FAILED(pLayer->Add_GameObject(L"MiniMapTerrain", pGameObject)))
				return E_FAIL;

			static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(i * 13,50,j*13);

		}
	}


	

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

HRESULT CMapEditor::Ready_UI_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;



	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

HRESULT CMapEditor::Ready_Light()
{
	D3DLIGHT9	tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tLightInfo.Direction = { 1.f, -1.f, 1.f };

	if (FAILED(CLightMgr::GetInstance()->Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
		return E_FAIL;	
	



	//tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	//
	//tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//
	//tLightInfo.Direction = { 1.f, -1.f, 1.f };
	//
	//if (FAILED(CLightMgr::GetInstance()->Ready_Light(m_pGraphicDev, &tLightInfo, 1)))
	//	return E_FAIL;

	return S_OK;
}



CMapEditor* CMapEditor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMapEditor* pMapEditor = new CMapEditor(pGraphicDev);

	if (FAILED(pMapEditor->Ready_Scene()))
	{
		Safe_Release(pMapEditor);
		MSG_BOX("CMapEditor Create Failed");
		return nullptr;
	}

	return pMapEditor;
}

void CMapEditor::Free()
{
	CScene::Free();
}

void CMapEditor::Show_Position() {
	_vec3 Position{};
	dynamic_cast<Engine::CTransform*>(CManagement::GetInstance()->Get_FirstObjectComponent(ID_DYNAMIC, L"GameLogic_Layer", L"MapEditorTerrain", L"Com_Transform"))->Get_Info(INFO_POS,&Position);

	//ImGui::Begin("Operation");
	//if (ImGui::RadioButton("Translate", m_CurrentGizmoOperation == ImGuizmo::TRANSLATE))
	//	m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
	//ImGui::SameLine();
	//if (ImGui::RadioButton("Rotate", m_CurrentGizmoOperation == ImGuizmo::ROTATE))
	//	m_CurrentGizmoOperation = ImGuizmo::ROTATE;
	//ImGui::SameLine();
	//if (ImGui::RadioButton("Scale", m_CurrentGizmoOperation == ImGuizmo::SCALE))
	//	m_CurrentGizmoOperation = ImGuizmo::SCALE;
	//ImGui::End();

	ImGuizmo::ROTATE;
	ImGui::SliderFloat3("Position", Position,100.f,100.f);
}