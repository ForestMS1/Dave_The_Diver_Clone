#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include <queue>
BEGIN(Engine)


// 게임진행에 필요한 플래그라던지 전역적으로 접근가능한 데이터의 관리를 매니저
class ENGINE_DLL CGameMemMgr : public CBase
{
	DECLARE_SINGLETON(CGameMemMgr)

public:
	class CDaveInfo
	{
	public:
		enum DAVE_JACKSALCHONG
		{
			JKCHONG_DEFAULT,
			JKCHONG_END
		};

		enum DAVE_JACKSALCHOCK
		{
			JKCHOCK_DEFAULT,
			JKCHOCK_END
		};

		enum DAVE_GUN
		{
			GUN_DEFAULT,
			GUN_TRIPLE_ACCEL,
			GUN_PENTA_ACCEL,
			GUN_END
		};

	public:
		_uint Get_GonggiVolume() const { return m_iGonggiVolume; }
		_uint Get_JamsuDepth()const { return m_iJamsuDepth; }
		_uint Get_JeokjaeWeight()const { return m_iJeokjaeWeight; }
		_uint Get_JaksalDamage()const { return m_iJaksalDamage; }

		void Set_GonggiVolume(_uint i) { m_iGonggiVolume = i; }
		void Set_JamsuDepth(_uint i) { m_iJamsuDepth = i; }
		void Set_JeokjaeWeight(_uint i) { m_iJeokjaeWeight = i; }
		void Set_JaksalDamage(_uint i) { m_iJaksalDamage = i; }

	private:
		_uint m_iGonggiVolume = 90;
		_uint m_iJamsuDepth = 40;
		_uint m_iJeokjaeWeight = 9;
		_uint m_iJaksalDamage = 15;

	};

	// 이번 다이브 시간 다이브 타임 잡은 물고기 등등
	class CDiveInfo
	{
	public:
		typedef struct tagDiveFish
		{
			std::wstring sFishName;
			std::wstring sThumbNailAssetName;
			std::wstring sSushiThumbNailAssetName;
			_uint iRank;
			_uint iStar;
			_uint iMeatCnt;
			float fWeight;
			float fLength;
			_uint iSushiMoney;
			_uint iSushiLv;
			bool bFish;
		} DIVE_FISH;

		typedef struct tagDiveItem
		{
			std::wstring sItemName;
			std::wstring sThumbNailAssetName;
		} DIVE_ITEM;

	public:
		void DiveStart() { m_DiveStart = clock(); }
		void DiveEnd() { m_DiveEnd = clock(); m_bDiveEnd = true; }

		float CalcDiveTime()
		{
			return (float)(m_DiveEnd - m_DiveStart) / CLOCKS_PER_SEC;
		}

		std::wstring CalcDiveTimeStr()
		{
			auto total_seconds = (int)CalcDiveTime();
			int hours = total_seconds / 3600;
			int minutes = (total_seconds / 60) % 60;
			int seconds = total_seconds % 60;

			// 3. 00:00:00 포맷으로 출력
			std::wstringstream wss;
			wss << std::setw(2) << std::setfill(L'0') << hours << L":"
				<< std::setw(2) << std::setfill(L'0') << minutes << L":"
				<< std::setw(2) << std::setfill(L'0') << seconds;
			return wss.str();
		}

		void Add_FishFront(DIVE_FISH& fish) { m_vecCaughtFishes.push_front(fish); ++m_iCaughtFish; }
		std::list<DIVE_FISH>& Get_Fishes() { return m_vecCaughtFishes; }

		std::list< DIVE_ITEM>& Get_Itemes() { return m_vecCaughtItems; }
		void Add_ItemBack(DIVE_ITEM& item) { m_vecCaughtItems.push_back(item); ++m_iObtained; }

		void Set_Depth(_float f) { m_fDepth = f; }
		_float Get_Depth() const { return m_fDepth; }

		_uint Get_CaughtFish() const { return m_iCaughtFish; }
		_uint Get_Obtained() const { return m_iObtained; }

		bool Get_DiveEnd() const { return m_bDiveEnd; }

	private:
		std::list<DIVE_FISH> m_vecCaughtFishes;
		std::list<DIVE_ITEM> m_vecCaughtItems;

		clock_t m_DiveStart;
		clock_t m_DiveEnd;

		_uint m_iCaughtFish = 0;
		_float m_fDepth = 0.f;
		_uint m_iObtained = 0;

		bool m_bDiveEnd = false;
	};

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
	void Set_Money2(_uint iMoney) { m_iMoney = iMoney; }

	bool Get_ShipNight() const { return m_bShipNight; }
	void Set_ShipNight(bool b) { m_bShipNight = b; }

	void Set_Money(_uint iMoney) { m_iMoney += iMoney; }
private:
	_uint m_iMoney;

	bool m_bShipNight;

	// IDiverCurrent
public:
	unordered_map<std::wstring, _uint>* Get_IDiverCurrentLevel() { return &m_mapIDiverCurrentLevel; }
	void Set_IDiverCurrentLevel(std::wstring& s, _uint i) { m_mapIDiverCurrentLevel[s] = i; };
	void Set_IDiverSansoLevelUp() { m_mapIDiverCurrentLevel[L"Tex_Ship_IDiver_Item_Sanso"]+=1; }
private:
	unordered_map<std::wstring, _uint> m_mapIDiverCurrentLevel;

public:
	CDaveInfo& Get_DaveInfo() { return m_DaveInfo; }
	std::vector<CDiveInfo>& Get_DiveInfos() { return m_vecDiveInfo; }

private:
	std::vector<CDiveInfo> m_vecDiveInfo;
	CDaveInfo m_DaveInfo;
};

END

