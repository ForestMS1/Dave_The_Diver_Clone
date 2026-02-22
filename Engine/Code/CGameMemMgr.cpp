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

void CGameMemMgr::levelUp(string name)
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
}
HRESULT CGameMemMgr::Ready()
{
	addFish("블루", 4);
    return S_OK;
}

void CGameMemMgr::addFish(string name, int quantity)
{
	bool found = false;
	//물고기가 이미 등록 되있으면
	for (auto& fish : fishes) {
		if (fish->name == name) {
			found = true;
			fish->quantity += quantity;
		}
	}
	// 새로운 물고기면
	if (found == false) {
		FISH* newfish = new FISH;
		newfish->name == name;
		newfish->quantity = quantity;
		newfish->level = 1;
		newfish->cost = 11;
		fishes.push_back(newfish);
	}
}
