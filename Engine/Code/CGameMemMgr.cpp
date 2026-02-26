#include "CGameMemMgr.h"
IMPLEMENT_SINGLETON(CGameMemMgr)


CGameMemMgr::CGameMemMgr()
	: m_iMoney(0)
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
}
HRESULT CGameMemMgr::Ready()
{
	addFish(L"블루종", 4, 11, 15);
	addFish(L"노랑탕", 5, 13, 18);
	addFish(L"코반아지", 6, 18, 30);
	addFish(L"노랑백", 7, 12, 20);
	addFish(L"흰동가리", 8, 15, 23);
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
	delete CookingMenu.front();
	CookingMenu.pop();
}
