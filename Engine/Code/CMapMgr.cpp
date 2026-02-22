#include "CMapMgr.h"
#include "CAssetMgr.h"
#include "CAssetJson.h"
#include <fstream>

IMPLEMENT_SINGLETON(CMapMgr)

CMapMgr::CMapMgr() : m_Scene(nullptr)
{
}

CMapMgr::~CMapMgr()
{
}

void CMapMgr::Load() {
	using json = nlohmann::json;
	json scene;

	scene = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetJson>(L"Json_Map")->Get_Json();
	


	for (auto& i : scene["Scene"]) {
		std::string layerName = i["LayerName"];
		CLayer* m_Layer = m_Scene->Get_Layer(StringToWString(layerName));
		if (m_Layer != nullptr) {
			for (auto& j : i["Layer"]) {
				std::string GameObjectLayerName = j["GameObjectLayerName"].get<std::string>();

				if ((&(*m_Layer->Get_GameObjects(StringToWString(GameObjectLayerName)))) != nullptr) {
					list<CGameObject*> CGameObjectLayer = *m_Layer->Get_GameObjects(StringToWString(GameObjectLayerName));
					if ((GameObjectLayerName) != "DiveDave") {
						auto it = CGameObjectLayer.begin();
						for (auto& k : j["GameObjectLayer"]) {
							int ObjCnt = k["ObjCnt"];

							for (auto& elem : k["Transform"]) {
								std::string type = elem[0];

								if (type == "pos")
								{

									float x = elem[1];
									float y = elem[2];
									float z = elem[3];
									dynamic_cast<CTransform*>((*it)->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(x, y, z);

								}
								else if (type == "rot")
								{
									float rx = elem[1];
									float ry = elem[2];
									float rz = elem[3];
									_vec3 rotation{ rx, ry, rz };
									dynamic_cast<CTransform*>((*it)->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Rotation(&rotation);
								}
								else if (type == "scale")
								{
									float sx = elem[1];
									float sy = elem[2];
									float sz = elem[3];
									_vec3 scale{ sx, sy, sz };
									dynamic_cast<CTransform*>((*it)->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Scale(&scale);
								}
							}
							++it;
						}
					}
		
				}
			


			}
		}
	
	}



	

}

void CMapMgr::Save()
{
	using json = nlohmann::json;

	json Scene;
	Scene["Scene"] = json::array();

	for (auto i : *m_Scene->Get_Layer()) {
		json Layer;

		Layer["Layer"] = json::array();
		for (auto j : *i.second->Get_GameObjects()) {
			json GameObjectLayer;

			GameObjectLayer["GameObjectLayer"] = json::array();
			int Cnt = 0;
			for (auto o : j.second) {
				_vec3 pos, rot, scale;
				dynamic_cast<CTransform*>(o->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &pos);
				dynamic_cast<CTransform*>(o->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Rotation(&rot);
				dynamic_cast<CTransform*>(o->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Scale(&scale);
				json jObj;

				jObj["ObjCnt"] = Cnt++;
				jObj["Transform"] = { { "pos",pos.x,pos.y,pos.z},
										{ "rot",rot.x,rot.y,rot.z},
										{ "scale",scale.x,scale.y,scale.z} };

				GameObjectLayer["GameObjectLayer"].push_back(jObj);
			}
			GameObjectLayer["GameObjectLayerName"] = j.first.data();
			Layer["Layer"].push_back(GameObjectLayer);
		}

		Layer["LayerName"] = i.first.data();
		Scene["Scene"].push_back(Layer);
	}

	CAssetMgr::GetInstance()->Get_AssetFirst<CAssetJson>(L"Json_Map")->Save(Scene);

}

void CMapMgr::Show_GUI()
{
	
	if (m_Scene != nullptr) {
		// Obj
		Object_Show();

		// Save Load
		SaveLoad_Show();
	}
}

void CMapMgr::Object_Show()
{



	ImGui::Begin("Object");

		for (auto i : *(m_Scene->Get_Layer())) {
		
			for (auto j : *i.second->Get_GameObjects()) {
				int Cnt = 0;
				for (auto k : j.second) {
					string pStr;
					pStr = wstringToString(j.first);
					string buffer;
					buffer = to_string(Cnt);

					pStr = pStr + buffer;
					ImGui::Selectable(pStr.c_str());
					Cnt++;
				}
			}
		}

	ImGui::End();
}

void CMapMgr::SaveLoad_Show()
{

	ImGui::Begin("SaveLoad");




	if (ImGui::Button("SAVE")) {
		Save();
	}
	
	if (ImGui::Button("LOAD")) {
		Load();
	}
	ImGui::End();

}

void CMapMgr::Update_Map(const _float& fTimeDelta)
{

}

void CMapMgr::LateUpdate_Map(const _float& fTimeDelta)
{
}

void CMapMgr::Render_Map()
{
	Show_GUI();
}

void CMapMgr::Free()
{
}

wstring  CMapMgr::StringToWString(const std::string& str)
{
	if (str.empty()) return L"";

	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);

	wstr.pop_back(); 
	return wstr;
}

string  CMapMgr::wstringToString(const std::wstring& wstr) {
	int size = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string str(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size, NULL, NULL);
	return str;
}