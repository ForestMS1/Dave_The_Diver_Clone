#include "CMapMgr.h"
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

#include <io.h>

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
	vec_Map.reserve(15 * 15);

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
	Show_GUI();
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

void CMapEditor::Show_GUI() {

	if (ROOM_CUSTOM != e_MapEditorLevel) {
		{
			ImGui::SetNextWindowPos(ImVec2(WINCX / 5 * 5.7, WINCY / 5 * 0.5), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);

			ImGui::Begin("ROOM_INFO");

			switch (e_MapEditorLevel) {
			case ROOM_PICKING:

				break;
			case ROOM_CHOICE:
			case ROOM_CREATE: {
				const ImVec2 btnSize = ImVec2(350, 30);

				ImGui::BeginChild("RoomList", ImVec2(0, 350), true);


				for (auto& i : vec_Map) {
					
					_vec2 Num = dynamic_cast<CMapEditorTerrain*>(i.front())->Get_RoomNum();

					char label[32];
					sprintf_s(label, "Room %d,%d", (_int)Num.x, (_int)Num.y);

					ImGui::PushID(Num.y * 100 + Num.x);

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
						m_pPickMiniMap = arrMiniMap[(_int)Num.x][(_int)Num.y];
						e_MapEditorLevel = ROOM_INTO_DELETE;
					}

					ImGui::PopID();

				}


				ImGui::EndChild();

			}


				break;

			case ROOM_INTO_DELETE: {
				_vec2 Num = dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Get_RoomNum();

				ImGui::Text("Room [%d][%d]", int(Num.y), int(Num.x));
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
					e_MapEditorLevel = ROOM_CHOICE;
				}

				ImGui::PopStyleVar();

			}
			break;
			case ROOM_CHOICE: {
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
			case ROOM_CREATE: {
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

					e_MapEditorLevel = ROOM_CHOICE;
					// 클릭되어 있을때 텍스쳐가 2일떄
					if (dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Get_TypeNum() == 2) {
						dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Set_TypeNum(1);
						e_MapEditorLevel = ROOM_CHOICE;

					}
				}

				ImGui::PopStyleVar();


			}
							break;
			case ROOM_INTO_DELETE: {
				float fullW = ImGui::GetContentRegionAvail().x;
				float h = 55.0f;


				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 12));

				ImGui::Spacing();

				if (ImGui::Button("Into the Room", ImVec2(fullW, h)))
				{
					_vec2 Room = dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Get_RoomNum();
					
					for (auto& i : vec_Map) {
						_vec2 RealRoomNum = { Room.y,Room.x };
						if (dynamic_cast<CMapEditorTerrain*>(i.front())->Get_RoomNum() == RealRoomNum) {
							m_pPickRoom = &i;
						}
					}
					
					for (auto& Tile : *m_pPickRoom) {
							dynamic_cast<CMapEditorTerrain*>(Tile)->Set_bRender(true);
					}


					e_MapEditorLevel = ROOM_CUSTOM;
					// 카메라 코드
					MoveCamera({ Room.y * 13 + 7.5f ,14,Room.x * 13 + 7 }, { Room.y * 13 + 7.5f  ,0,Room.x * 13 + 7 }, { 0.f,0.f,1.f });

				}

				ImGui::Spacing();

				if (ImGui::Button("Delete Room", ImVec2(fullW, h)))
				{
					// 맵 지우는 코드
				}

				ImGui::Spacing();

				if (ImGui::Button("Back", ImVec2(fullW, h)))
				{
					e_MapEditorLevel = ROOM_CHOICE;
				}


				ImGui::PopStyleVar();
				break;
			}

			}

			ImGui::End();
		}

	}

	// Custom 모드 일떄
	if (ROOM_CUSTOM == e_MapEditorLevel) {
		{

			ImGui::SetNextWindowPos(ImVec2(WINCX / 5 * 0.1, WINCY / 5 * 0.5), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);

			ImGui::Begin("ROOM_INFO");

			const ImVec2 btnSize = ImVec2(350, 30);

			ImGui::BeginChild("Room", ImVec2(0, 350), true);
			ImGui::Text("Room Object");

			ImGui::Spacing();

			if (ImGui::Button("Back", ImVec2(350, 30)))
			{
				e_MapEditorLevel = ROOM_CHOICE;
				// 카메라
				MoveCamera({ 100,250,100 }, { 100,0,100 }, { 0,0,1 });
				for (auto& Tile : *m_pPickRoom) {
					dynamic_cast<CMapEditorTerrain*>(Tile)->Set_bRender(false);
				}
			}


			ImGui::EndChild();

			ImGui::End();
		}

		{

			ImGui::SetNextWindowPos(ImVec2(WINCX / 5 * 5.3, WINCY / 5 * 0.5), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(520, 950), ImGuiCond_Always);
			ImGui::Begin("RESOURCE");
			float fullWidth = ImGui::GetContentRegionAvail().x;
			ImGui::SetNextItemWidth(fullWidth);
	

			const char* items[] = { "OBJECT", "TILE" };
	
			ImGui::Combo("##2", &nCurrentItem, items, IM_ARRAYSIZE(items));
		

			switch (nCurrentItem) {
			case 0:
				// Object
				if (nPrevItem != nCurrentItem) {
					NameRefresh(L"");
					nPrevItem = nCurrentItem;
				}
				break;

			case 1:
				// Texture
				if (nPrevItem != nCurrentItem) {
					NameRefresh(L"../Bin/Resource/Texture/Floor");
					nPrevItem = nCurrentItem;
				}
			
				break;
			}
		
			for (auto i : Name) {
				if (ImGui::Button(i.c_str())) {

				}
			
			}
		
			ImGui::End();
		}

	
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
			e_MapEditorLevel = ROOM_CREATE;
	
		}

		// 방이 만들어 졌을떄 (0일때)
		if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB) && dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Get_TypeNum() == 0) {
			e_MapEditorLevel = ROOM_INTO_DELETE;

		}
	}

}

void CMapEditor::MoveCamera(const _vec3& pEye,const _vec3& pAt, const _vec3& pUp) {
	dynamic_cast<CDynamicCamera*>
		(CManagement::GetInstance()->
			Get_Scene()->
			Get_Layer(L"Environment_Layer")->
			Get_GameObjectFirst(L"DynamicCamera"))->Set_Eye(pEye);

	dynamic_cast<CDynamicCamera*>
		(CManagement::GetInstance()->
			Get_Scene()->
			Get_Layer(L"Environment_Layer")->
			Get_GameObjectFirst(L"DynamicCamera"))->Set_At(pAt);


	dynamic_cast<CDynamicCamera*>
		(CManagement::GetInstance()->
			Get_Scene()->
			Get_Layer(L"Environment_Layer")->
			Get_GameObjectFirst(L"DynamicCamera"))->Set_Up(pUp);
}

HRESULT CMapEditor::CreateRoom() {
	_vec2 RoomNum =	dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Get_RoomNum();

	CGameObject* pGameObject = nullptr;
	vec_Map.emplace_back();
	vec_Map[iRoom_Cnt].reserve(13 * 13);

	for (int i = 0; i < 13; ++i) {
		for (int j = 0; j < 13; ++j) {
			pGameObject = CMapEditorTerrain::Create();
			vec_Map[iRoom_Cnt].emplace_back(pGameObject);
			if (nullptr == pGameObject)
				return E_FAIL;

			wstring L = L"MapTerrain";

			wstring roomCnt = to_wstring((iRoom_Cnt));
			wstring O = L"_";
			wstring a = to_wstring(int(RoomNum.y));
			wstring b = to_wstring(int(RoomNum.x));


			if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic_Layer")->Add_GameObject(L +roomCnt + O + a + b, pGameObject)))
				return E_FAIL;

			static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(int(RoomNum.y) * 13 + j, 0, int(RoomNum.x) * 13+i);
			dynamic_cast<CMapEditorTerrain*>(pGameObject)->Set_RoomNum({ RoomNum.y, RoomNum.x});
		}
	}
	

	iRoom_Cnt++;
	
	
	dynamic_cast<CMiniMapTerrain*>(m_pPickMiniMap)->Set_TypeNum(0);
	e_MapEditorLevel = ROOM_CHOICE;

	return S_OK;

}

void CMapEditor::NameRefresh(std::wstring_view path) {

	{

		Name.clear();
		Name.reserve(100);


		int len = WideCharToMultiByte(
			CP_UTF8, 0,
			path.data(), (int)path.size(),
			nullptr, 0,
			nullptr, nullptr);

		std::string pathName(len, 0);

		WideCharToMultiByte(
			CP_UTF8, 0,
			path.data(), (int)path.size(),
			pathName.data(), len,
			nullptr, nullptr);


		_finddata_t fd;
		string plus = "/*.*";
		string slash = "/";
		string name = pathName + plus;
		pathName += slash;
		long long handle = _findfirst(name.data(), &fd);

		int iResult = 0;
	;
		char* szCurPath = pathName.data();
		
		char szFullPath[128] = {};

	

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_None);
		ImGui::SetNextWindowSize(ImVec2(200, 700), ImGuiCond_None);

		while (iResult != -1)
		{
			if (fd.name[0] == 'F')
			{
				strcpy_s(szFullPath, szCurPath);
				strcat_s(szFullPath, fd.name);
				Name.emplace_back(szFullPath);

			}
			if (fd.name[0] == 'A') {
				strcpy_s(szFullPath, szCurPath);
				strcat_s(szFullPath, fd.name);
				string str1(szFullPath);
				char a = str1[str1.size() - 5];
				if (a == '0') {
					Name.emplace_back(szFullPath);
				}


			}
			iResult = _findnext(handle, &fd);
		}


	}
}