#include "pch.h"
#include "CMapEditor.h"

#include "CProtoMgr.h"
#include "CDynamicCamera.h"
 
#include "CLightMgr.h"
#include "CMapEditorTerrain.h"
#include "CMiniMapTerrain.h"
#include "CManagement.h"
#include "CGraphicDev.h"
#include "CDInputMgr.h"


CMapEditor::CMapEditor()
	: CScene()
{
}

CMapEditor::~CMapEditor()
{
}

HRESULT CMapEditor::Ready_Scene()
{
	for (int i = 0; i < 15; ++i)
	{
		for (int j = 0; j < 15; ++j) {
			arrMiniMap[i][j] = nullptr;
		}
	}
	for (int i = 0; i < 15; ++i)
	{
		for (int j = 0; j < 15; ++j) {
			arrRoom[i][j] = nullptr;
		}
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
	// debug 용 출력
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
	pGameObject  = CDynamicCamera::Create( &vEye, &vAt, &vUp);

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





	for (_uint i = 0; i < 15; ++i) {
		for (_uint j = 0; j < 15; ++j) {
	
			// MiniMapTerrain
			pGameObject = CMiniMapTerrain::Create();

			if (nullptr == pGameObject)
				return E_FAIL;

			ostringstream oss;

			wstring L = L"MinMapTerrain";
			_int Cnt = (i * 15 + j);
			wstring a = to_wstring(Cnt);



			if (FAILED(pLayer->Add_GameObject(L + a, pGameObject)))
				return E_FAIL;

			static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(i * 13,50,j*13);
			arrMiniMap[i][j] = pGameObject;

			dynamic_cast<CMiniMapTerrain*>(arrMiniMap[i][j])->Set_RoomNum({ (float)j,(float)i });
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

	if (FAILED(CLightMgr::GetInstance()->Ready_Light(&tLightInfo, 0)))
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



CMapEditor* CMapEditor::Create()
{
	CMapEditor* pMapEditor = new CMapEditor();

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
	{
		ImGui::SetNextWindowPos(ImVec2(WINCX / 5 * 5.7, WINCY / 5 * 0.5), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);

		ImGui::Begin("ROOM_INFO");

		for (_uint i = 0; i < 15; ++i) {
			for (_uint j = 0; j < 15; ++j) {
				if (arrRoom[i][j] != nullptr) {
					ImGui::Text("Room [%d][%d]",j,i);
			
				}
				

			}
		}


	
		ImGui::End();
	}

	{
		ImGui::SetNextWindowPos(ImVec2(WINCX / 5 * 5.7, WINCY / 5 * 3.5), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_Always);

		ImGui::Begin("Function");
		switch (e_MapEditorLevel) {
		case ROOM_PICKING:
		{
			PickMiniMap();

			float fullW = ImGui::GetContentRegionAvail().x;
			float h = 55.0f;


			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 12));

		
			ImGui::Spacing();

			if (ImGui::Button("Back", ImVec2(fullW, h)))
			{
				e_MapEditorLevel = LEVEL1;
			}

			ImGui::PopStyleVar();

		}
			break;
		case LEVEL1: {
			float fullW = ImGui::GetContentRegionAvail().x;
			float h = 55.0f;


			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 12));

			if (ImGui::Button("Room Choice", ImVec2(fullW, h)))
			{
				e_MapEditorLevel = ROOM_PICKING;
			}
			ImGui::PopStyleVar();

		}
				   break;
		case LEVEL2: {
			float fullW = ImGui::GetContentRegionAvail().x;
			float h = 55.0f;


			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 12));

			if (ImGui::Button("Room Create", ImVec2(fullW, h)))
			{
				CreateRoom();
				
			}

			ImGui::Spacing();

			if (ImGui::Button("InTo the Room", ImVec2(fullW, h)))
			{
				e_MapEditorLevel = LEVEL3;
			}

			ImGui::Spacing();

			if (ImGui::Button("Back", ImVec2(fullW, h)))
			{
				e_MapEditorLevel = LEVEL1;
			}

			ImGui::PopStyleVar();
			break;
		}


		case LEVEL3: {
			float fullW = ImGui::GetContentRegionAvail().x;
			float h = 55.0f;


			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 12));

			ImGui::Spacing();

			if (ImGui::Button("Back", ImVec2(fullW, h)))
			{
				e_MapEditorLevel = LEVEL2;
			}

			ImGui::PopStyleVar();
			break;
		}

		}

		ImGui::End();
	}
	

}

void CMapEditor::PickMiniMap() {
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3		vMousePos;

	_D3DVIEWPORT9 ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	pGraphicDev->GetViewport(&ViewPort);

	// 뷰포트 -> 투영
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영 -> 뷰 스페이스
	_matrix matProj;
	pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix matView;
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	
	_vec3 vRayPos{ 0.f, 0.f, 0.f };
	_vec3 vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	float dis_pos = vRayPos.y - 50;

	float dis = dis_pos / vRayDir.y;
	
	float dirx = (dis) * vRayDir.x;
	float diry = (dis) * vRayDir.y;
	float dirz = (dis) * vRayDir.z;

	_vec3 Pos = { vRayPos.x - dirx , vRayPos.y - diry ,vRayPos.z - dirz };

	ImGui::Begin("INFO", nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Ray Direction Debug");
	ImGui::Separator();

	ImGui::Text("Pos.x : %.3f", Pos.x);
	ImGui::Text("Pos.y : %.3f", Pos.y);
	ImGui::Text("Pos.z : %.3f", Pos.z);

	ImGui::End();
	if (Pos.x/13 <= 15 && Pos.z/13 <= 15 && Pos.x >=0 && Pos.z >=0) {
		m_pPickMiniMap = arrMiniMap[(int)Pos.x/ 13][(int)Pos.z/ 13];
	
		// 클릭되어 있을때
		if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB) && dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Get_TypeNum() != 0) {
			dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Set_TypeNum(2);
			e_MapEditorLevel = LEVEL2;
	
		}
		
	}


	
}

void CMapEditor::CreateRoom() {
	_vec2 RoomNum =	dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Get_RoomNum();

	CGameObject* pGameObject = nullptr;


	// MapEditorTerrain

	arrRoom[int(RoomNum.y)][int(RoomNum.x)] =  CMapEditorTerrain::Create();
	dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Set_TypeNum(0);
	e_MapEditorLevel = LEVEL1;

}