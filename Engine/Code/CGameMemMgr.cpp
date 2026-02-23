#include "CGameMemMgr.h"
IMPLEMENT_SINGLETON(CGameMemMgr)


CGameMemMgr::CGameMemMgr()
{
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
}
HRESULT CGameMemMgr::Ready()
{
	addFish(L"블루종", 4);
	addFish(L"노랑탕", 5);
	addFish(L"코반아지", 6);
	addFish(L"노랑백", 7);
	addFish(L"흰동가리", 8);
    return S_OK;
}

void CGameMemMgr::addFish(wstring name, int quantity)
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
		newfish->name = name;
		newfish->quantity = quantity;
		newfish->level = 1;
		newfish->cost = 11;
		fishes.push_back(newfish);
	}
}
