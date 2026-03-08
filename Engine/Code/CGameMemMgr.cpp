#include "CGameMemMgr.h"
IMPLEMENT_SINGLETON(CGameMemMgr)


std::wstring CGameMemMgr::Get_ItemTexName(DAVE_ITEM eItem)
{
	switch (eItem)
	{
	case	IT_WOOD: return L"Tex_UIITem_Wood";
	case	IT_BONE: return L"Tex_UIITem_Bone";
	case	IT_FRAGMENT: return L"Tex_UIITem_Fragment";
	case	IT_JADETHURIBLE: return L"Tex_UIITem_JadeThurible";
	case	IT_ROPE: return L"Tex_UIITem_Rope";
	case	IT_RUBYRING: return L"Tex_UIITem_RubyRing";
	case	IT_UMBELLULA: return L"Tex_UIITem_Umbellula";
	case	IT_WOODPLATE: return L"Tex_UIITem_Woodplate";
	case	IT_WATCH: return L"Tex_UIITem_Watch";
	}

	return L"";
}

std::wstring CGameMemMgr::Get_ItemTitle(DAVE_ITEM eItem)
{
	switch (eItem)
	{
	case	IT_WOOD: return L"나무";
	case	IT_BONE: return L"뼈";
	case	IT_FRAGMENT: return L"부품";
	case	IT_JADETHURIBLE: return L"옥향로";
	case	IT_ROPE: return L"로프";
	case	IT_RUBYRING: return L"루비반지";
	case	IT_UMBELLULA: return L"움벨룰라";
	case	IT_WOODPLATE: return L"나무판자";
	case	IT_WATCH: return L"고급시계";
	}

	return L"404";
}

std::wstring CGameMemMgr::Get_ItemDesc(DAVE_ITEM eItem)
{
	switch (eItem)
	{
	case	IT_WOOD: return L"나무 다";
	case	IT_BONE: return L"뼈 다";
	case	IT_FRAGMENT: return L"부품 이다";
	case	IT_JADETHURIBLE: return L"옥향로 다";
	case	IT_ROPE: return L"로프 다";
	case	IT_RUBYRING: return L"루비반지 다";
	case	IT_UMBELLULA: return L"움벨룰라 다";
	case	IT_WOODPLATE: return L"나무판자 다";
	case	IT_WATCH: return L"고급시계 다";
	}

	return L"404";
}

CGameMemMgr::CGameMemMgr()
	: m_iMoney(1500)
	, m_bShipNight(false)
{
	m_mapIDiverCurrentLevel[L"Tex_Ship_IDiver_Item_Sanso"] = 1;
	m_mapIDiverCurrentLevel[L"Tex_Ship_IDiver_Item_Clothes"] = 1;
	m_mapIDiverCurrentLevel[L"Tex_Ship_IDiver_Item_Cage"] = 1;
	m_mapIDiverCurrentLevel[L"Tex_Ship_IDiver_Item_Jaksal"] = 1;
}

CGameMemMgr::~CGameMemMgr()
{
    Free();

}

void CGameMemMgr::levelUp(wstring name)
{
	//물고기 레벨업
	for (auto& fish : fishes) {
		if (fish->name == name) {
			fish->level++;
			fish->cost += 5;
			fish->quantity -= 3;
		}
	}
}

void CGameMemMgr::Free()
{
	for_each(fishes.begin(), fishes.end(), Safe_Delete<FISH*>);
	fishes.clear();
	for_each(SelectedMenu.begin(), SelectedMenu.end(), Safe_Delete<FISH*>);
	SelectedMenu.clear();
	CGameMemMgr::GetInstance()->ClearCookingMenu();

}
HRESULT CGameMemMgr::Ready()
{
	addFish(L"블루종", 4, 11, 15);
	addFish(L"노랑탕", 5, 13, 18);
	addFish(L"코반아지", 6, 18, 30);
	addFish(L"노랑백", 7, 12, 20);
	addFish(L"흰동가리", 8, 15, 23);
	addFish(L"반쵸", 1, 10, 0);
	stockPicIndex = 0;
	stockCreated = 0;
    return S_OK;
}

void CGameMemMgr::addFish(wstring name, int quantity, int cost, int quality)
{
	bool found = false;
	//물고기가 이미 등록 되있으면
	for (auto& fish : fishes) {
		if (fish->name == name) {
			found = true;
			fish->quantity += quantity;
			fish->quality = quality;
			fish->cost = cost;
		}
	}
	// 새로운 물고기면
	if (found == false) {
		FISH* newfish = new FISH;
		newfish->name = name;
		newfish->quantity = quantity;
		newfish->quality = quality;
		newfish->cost = cost;
		newfish->level = 1;
		fishes.push_back(newfish);
	}
}

void CGameMemMgr::addMenu(wstring name, int quantity)
{
	FISH* newfish = new FISH;
	newfish->name = name;
	newfish->quantity = quantity;
	SelectedMenu.push_back(newfish);
}

void CGameMemMgr::reduceMenu(wstring name)
{
	vector<FISH*>::iterator iter = SelectedMenu.begin();
	for (auto& menu : SelectedMenu) {
		if (menu->name == name) {
			menu->quantity -= 1;
		}
	}
	for (iter; iter != SelectedMenu.end();) {
		if ((*iter)->quantity <= 0) {
			Safe_Delete(*iter);
			iter = SelectedMenu.erase(iter);
		}
		else {
			iter++;
		}
	}
}

void CGameMemMgr::addCookingMenu(wstring name)
{
	FISH* newfish = new FISH;
	newfish->name = name;
	CookingMenu.push(newfish);
}

void CGameMemMgr::deleteCookingMenu()
{
	if (!CookingMenu.empty()) {
		Safe_Delete(CookingMenu.front());
		CookingMenu.pop();
	}
}

void CGameMemMgr::ClearCookingMenu()
{
	while (!CookingMenu.empty()) {

		Safe_Delete(CookingMenu.front());
		CookingMenu.pop(); 
	}
}

void CGameMemMgr::AddStockMarketIndex()
{
	if (Get_DiveInfos().size() == 0 +1) {
		if (stockPicIndex < 4) {
			stockPicIndex++;
		}
	}
	else if (Get_DiveInfos().size() ==  1+1) {
		if (CGameMemMgr::GetInstance()->GetStockCreated() == 2) {
			if (stockPicIndex < 9) {
				stockPicIndex++;
			}
		}
		else if (CGameMemMgr::GetInstance()->GetStockCreated() == 3) {
			if (stockPicIndex < 12) {
				stockPicIndex++;
			}
		}
		

	}
	else if(Get_DiveInfos().size() == 2+1){
		if (stockPicIndex < 13) {
			stockPicIndex++;
		}
	}
	
}





std::wstring CGameMemMgr::CDaveInfo::Get_DaveGunTexName(DAVE_GUN eItem)
{
	switch (eItem)
	{
	case GUN_DEFAULT: return L"";
	case GUN_PENTA_ACCEL: return L"Tex_UI_Gun_Penta_Accel";
	case GUN_TRIPLE_ACCEL: return L"Tex_UI_Gun_Triple_Accel";
	}
	return L"";
}

std::wstring CGameMemMgr::CDaveInfo::Get_DaveGunTitle(DAVE_GUN eItem)
{
	switch (eItem)
	{
	case GUN_DEFAULT: return L"";
	case GUN_PENTA_ACCEL: return L"펜타 악셀";
	case GUN_TRIPLE_ACCEL: return L"트리플 악셀";
	}
	return L"";
}

std::wstring CGameMemMgr::CDaveInfo::Get_DaveGunDesc(DAVE_GUN eItem)
{
	switch (eItem)
	{
	case GUN_DEFAULT: return L"";
	case GUN_PENTA_ACCEL: return L"펜타 악셀 이다";
	case GUN_TRIPLE_ACCEL: return L"트리플 악셀 이다";
	}
	return L"";
}

void CGameMemMgr::CDaveInfo::InventoryInitialize()
{
	// 0 ~ 31: 잡템칸
	// 32: 작살
	// 33: 총
	// 34: 근접무기
	// 35: 부적1
	// 36: 부적2
	for (int i = 0; i < 37; ++i)
	{
		CGameMemMgr::DIVERBOX_ITEM item1{};
		m_mapInventory.insert({ i, item1 });
	}

	{
		m_mapInventory[32].sItemName = L"작살";
		m_mapInventory[32].sItemDesc = L"물고기를 잡기 위해 작대기\n끝에 뾰족한 쇠를 박아 만드는 도구";
		m_mapInventory[32].sAssetName = L"Tex_Ship_InventoryUpperItem_Jaksal";
		m_mapInventory[32].iCnt = 0;
	}

	{
		m_mapInventory[33].sItemName = L"총";
		m_mapInventory[33].sItemDesc = L"인류를 먹이사슬의 \n최정점에 자리하게 해준 도구이자 \n인류 과학기술 진보의 역사 \n그 자체인 도구이다";
		m_mapInventory[33].sAssetName = L"Tex_Ship_InventoryUpperItem_Gun";
		//m_mapInventory[33].sAssetName = L"Tex_UI_Gun_Triple_Accel";
		m_mapInventory[33].iCnt = 0;
	}

	{
		m_mapInventory[34].sItemName = L"칼";
		m_mapInventory[34].sItemDesc = L"물건을 베거나 썰거나 깎는 데 쓰는 도구.";
		m_mapInventory[34].sAssetName = L"Tex_Ship_InventoryUpperItem_Knief";
		m_mapInventory[34].iCnt = 0;
	}

	{
		m_mapInventory[35].sItemName = L"쥬신";
		m_mapInventory[35].sItemDesc = L"지니고 있으면\n취업 확률을 높여준다.";
		m_mapInventory[35].sAssetName = L"Tex_Ship_InventoryUpperItem_Jusin";
		m_mapInventory[35].iCnt = 0;
	}

	{
		m_mapInventory[36].sItemName = L"쥬신";
		m_mapInventory[36].sItemDesc = L"지니고 있으면\n취업 확률을 높여준다.";
		m_mapInventory[36].sAssetName = L"Tex_Ship_InventoryUpperItem_Jusin";
		m_mapInventory[36].iCnt = 0;
	}
}
