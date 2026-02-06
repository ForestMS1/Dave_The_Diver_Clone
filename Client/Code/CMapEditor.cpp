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



	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });


	for (_uint i = 0; i < 15; ++i) {
		for (_uint j = 0; j < 15; ++j) {
	
			// MiniMapTerrain
			pGameObject = CMiniMapTerrain::Create();

			if (nullptr == pGameObject)
				return E_FAIL;



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

		switch (e_MapEditorLevel) {
		case ROOM_PICKING:

			break;
		case LEVEL1:
		case LEVEL2: {
			const ImVec2 btnSize = ImVec2(350, 30);

			ImGui::BeginChild("RoomList", ImVec2(0, 350), true);

			for (_uint i = 0; i < 15; ++i)
			{
				for (_uint j = 0; j < 15; ++j)
				{
					if (arrRoom[i][j] == nullptr)
						continue;

		
					char label[32];
					sprintf_s(label, "Room %d,%d", j, i);

					// 더블클릭 안정적으로 받기 위해 InvisibleButton + Text 조합
					ImGui::PushID(i * 100 + j);

					ImGui::InvisibleButton("RoomBtn", btnSize);

					// 버튼처럼 보이게 그리기(선택사항)
					ImVec2 pMin = ImGui::GetItemRectMin();
					ImVec2 pMax = ImGui::GetItemRectMax();

					ImDrawList* draw = ImGui::GetWindowDrawList();
					ImU32 col = ImGui::IsItemHovered()
						? IM_COL32(120, 160, 220, 255)
						: IM_COL32(70, 70, 70, 255);

					draw->AddRectFilled(pMin, pMax, col, 6.0f);
					draw->AddRect(pMin, pMax, IM_COL32(30, 30, 30, 255), 6.0f);

				
					ImVec2 textPos = ImVec2(pMin.x + 10.0f, pMin.y + 7.0f);
					draw->AddText(textPos, IM_COL32(255, 255, 255, 255), label);


					if (ImGui::IsItemHovered() &&
						ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
					
					}

					ImGui::PopID();

				
				
				}
			}

			ImGui::EndChild();
			
		}
		

			break;

		case LEVEL3: {
			_vec2 Num = dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Get_RoomNum();

			ImGui::Text("Room [%d][%d]", Num.y, Num.x);
		}
			
			break;

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


			if (ImGui::Button("Back", ImVec2(fullW, h)))
			{
	
				e_MapEditorLevel = LEVEL1;
				// 클릭되어 있을때 텍스쳐가 2일떄
				if (dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Get_TypeNum() == 2) {
					dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Set_TypeNum(1);
					e_MapEditorLevel = LEVEL1;

				}
			}

			ImGui::PopStyleVar();

			
		}  
				   break;
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
	
		// 클릭이 될떄 (기본 상태일때)
		if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB) && dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Get_TypeNum() ==1) {
			dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Set_TypeNum(2);
			e_MapEditorLevel = LEVEL2;
	
		}

		// 방이 만들어 졌을떄 (0일때)
		if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB) && dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Get_TypeNum() == 0) {
		
			e_MapEditorLevel = LEVEL2;

		}

	
	}

}

HRESULT CMapEditor::CreateRoom() {
	_vec2 RoomNum =	dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Get_RoomNum();

	CGameObject* pGameObject = nullptr;


	arrRoom[int(RoomNum.y)][int(RoomNum.x)] = pGameObject = CMapEditorTerrain::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	wstring L = L"MapTerrain";

	wstring a = to_wstring(int(RoomNum.y));
	wstring b = to_wstring(int(RoomNum.x));


	if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic_Layer")->Add_GameObject(L+a+b, pGameObject)))
		return E_FAIL;

	static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(int(RoomNum.y)*13, 0, int(RoomNum.x)*13);


	dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Set_TypeNum(0);
	e_MapEditorLevel = LEVEL1;

	return S_OK;

}