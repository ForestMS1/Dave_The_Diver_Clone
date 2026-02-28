#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include <queue>
BEGIN(Engine)


// 게임진행에 필요한 플래그라던지 전역적으로 접근가능한 데이터의 관리를 매니저
class ENGINE_DLL CGameMemMgr : public CBase
{
	DECLARE_SINGLETON(CGameMemMgr)

private:
	explicit CGameMemMgr();
	virtual ~CGameMemMgr();
public:
	typedef struct tagFish {
		wstring name;
		int quantity;
		int quality;
		int level;
		int cost;
	}FISH;
public:
	HRESULT Ready();
	void addFish(wstring name, int quantity, int cost, int quality);
	void addMenu(wstring name, int quantity);
	void reduceMenu(wstring name);
	void addCookingMenu(wstring name);
	void deleteCookingMenu();
	void ClearCookingMenu();
	vector<FISH*>& getFishes() { return fishes; }
	vector<FISH*>& getMenu() { return SelectedMenu; }
	queue<FISH*>& getCookingMenu() { return CookingMenu; }

	void levelUp(wstring name);
private:
	virtual void		Free();
	vector<FISH*> fishes;
	vector<FISH*> SelectedMenu;
	queue<FISH*> CookingMenu;


	// 돈
public:
	_uint Get_Money() const { return m_iMoney; }
	void Set_Money(_uint iMoney) { m_iMoney = iMoney; }

	bool Get_ShipNight() const { return m_bShipNight; }
	void Set_ShipNight(bool b) { m_bShipNight = b; }

private:
	_uint m_iMoney;

	bool m_bShipNight;

	// IDiverCurrent
public:
	unordered_map<std::wstring, _uint>* Get_IDiverCurrentLevel() { return &m_mapIDiverCurrentLevel; };
private:
	unordered_map<std::wstring, _uint> m_mapIDiverCurrentLevel;
};

END

