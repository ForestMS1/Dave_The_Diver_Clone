#include "pch.h" 
#include "IDiverInfo.h" 

namespace IDiver
{
    unordered_map<std::wstring, unordered_map<_uint, IDIVER_INFO>> g_mapIDiverInfo;

	 void InitIDiverInfo()
	{
		InitIDiverSansoInfo(L"Tex_Ship_IDiver_Item_Sanso");
		InitIDiverClothesInfo(L"Tex_Ship_IDiver_Item_Clothes");
		InitIDiverCageInfo(L"Tex_Ship_IDiver_Item_Cage");
		InitIDiverJaksalInfo(L"Tex_Ship_IDiver_Item_Jaksal");
	}

	 IDIVER_INFO& Get_Info(std::wstring_view svTex, _uint iLevel)
	 {
		 return g_mapIDiverInfo[wstring(svTex)][iLevel];
	 }

	 void InitIDiverSansoInfo(std::wstring_view svTex)
	{
		g_mapIDiverInfo[wstring(svTex)][1].iStep = 1;
		g_mapIDiverInfo[wstring(svTex)][1].iCost = 10;
		g_mapIDiverInfo[wstring(svTex)][1].iAmount = 15;
		g_mapIDiverInfo[wstring(svTex)][1].sTitle = L"산소통";
		g_mapIDiverInfo[wstring(svTex)][1].sLevel = L"Lv1";
		g_mapIDiverInfo[wstring(svTex)][1].sUnitName = L"공기량";
		g_mapIDiverInfo[wstring(svTex)][1].sUnit = L"15 bar";
		g_mapIDiverInfo[wstring(svTex)][1].sUpgradeDesc = L"sUpgradeDesc";
		g_mapIDiverInfo[wstring(svTex)][1].sUpgradeSuccessDesc = L"sUpgradeSuccessDesc";

		g_mapIDiverInfo[wstring(svTex)][2].iStep = 2;
		g_mapIDiverInfo[wstring(svTex)][2].iCost = 20;
		g_mapIDiverInfo[wstring(svTex)][2].iAmount = 25;
		g_mapIDiverInfo[wstring(svTex)][2].sTitle = L"산소통";
		g_mapIDiverInfo[wstring(svTex)][2].sLevel = L"Lv2";
		g_mapIDiverInfo[wstring(svTex)][2].sUnitName = L"공기량";
		g_mapIDiverInfo[wstring(svTex)][2].sUnit = L"25 bar";
		g_mapIDiverInfo[wstring(svTex)][2].sUpgradeDesc = L"sUpgradeDesc";
		g_mapIDiverInfo[wstring(svTex)][2].sUpgradeSuccessDesc = L"sUpgradeSuccessDesc";

		g_mapIDiverInfo[wstring(svTex)][3].iStep = 2;
		g_mapIDiverInfo[wstring(svTex)][3].iCost = 30;
		g_mapIDiverInfo[wstring(svTex)][3].iAmount = 35;
		g_mapIDiverInfo[wstring(svTex)][3].sTitle = L"산소통";
		g_mapIDiverInfo[wstring(svTex)][3].sLevel = L"Lv3";
		g_mapIDiverInfo[wstring(svTex)][3].sUnitName = L"공기량";
		g_mapIDiverInfo[wstring(svTex)][3].sUnit = L"35 bar";
		g_mapIDiverInfo[wstring(svTex)][3].sUpgradeDesc = L"sUpgradeDesc";
		g_mapIDiverInfo[wstring(svTex)][3].sUpgradeSuccessDesc = L"sUpgradeSuccessDesc";
	}

	 void InitIDiverClothesInfo(std::wstring_view svTex)
	{
		g_mapIDiverInfo[wstring(svTex)][1].iStep = 1;
		g_mapIDiverInfo[wstring(svTex)][1].iCost = 10;
		g_mapIDiverInfo[wstring(svTex)][1].iAmount = 15;
		g_mapIDiverInfo[wstring(svTex)][1].sTitle = L"잠수복";
		g_mapIDiverInfo[wstring(svTex)][1].sLevel = L"Lv1";
		g_mapIDiverInfo[wstring(svTex)][1].sUnitName = L"한계수심";
		g_mapIDiverInfo[wstring(svTex)][1].sUnit = L"15 m";
		g_mapIDiverInfo[wstring(svTex)][1].sUpgradeDesc = L"sUpgradeDesc";
		g_mapIDiverInfo[wstring(svTex)][1].sUpgradeSuccessDesc = L"sUpgradeSuccessDesc";

		g_mapIDiverInfo[wstring(svTex)][2].iStep = 2;
		g_mapIDiverInfo[wstring(svTex)][2].iCost = 20;
		g_mapIDiverInfo[wstring(svTex)][2].iAmount = 25;
		g_mapIDiverInfo[wstring(svTex)][2].sTitle = L"잠수복";
		g_mapIDiverInfo[wstring(svTex)][2].sLevel = L"Lv2";
		g_mapIDiverInfo[wstring(svTex)][2].sUnitName = L"한계수심";
		g_mapIDiverInfo[wstring(svTex)][2].sUnit = L"25 m";
		g_mapIDiverInfo[wstring(svTex)][2].sUpgradeDesc = L"sUpgradeDesc";
		g_mapIDiverInfo[wstring(svTex)][2].sUpgradeSuccessDesc = L"sUpgradeSuccessDesc";
	}

	 void InitIDiverCageInfo(std::wstring_view svTex)
	{
		g_mapIDiverInfo[wstring(svTex)][1].iStep = 1;
		g_mapIDiverInfo[wstring(svTex)][1].iCost = 10;
		g_mapIDiverInfo[wstring(svTex)][1].iAmount = 15;
		g_mapIDiverInfo[wstring(svTex)][1].sTitle = L"적재함";
		g_mapIDiverInfo[wstring(svTex)][1].sLevel = L"Lv1";
		g_mapIDiverInfo[wstring(svTex)][1].sUnitName = L"허용무게";
		g_mapIDiverInfo[wstring(svTex)][1].sUnit = L"15 kg";
		g_mapIDiverInfo[wstring(svTex)][1].sUpgradeDesc = L"sUpgradeDesc";
		g_mapIDiverInfo[wstring(svTex)][1].sUpgradeSuccessDesc = L"sUpgradeSuccessDesc";

		g_mapIDiverInfo[wstring(svTex)][2].iStep = 2;
		g_mapIDiverInfo[wstring(svTex)][2].iCost = 20;
		g_mapIDiverInfo[wstring(svTex)][2].iAmount = 25;
		g_mapIDiverInfo[wstring(svTex)][2].sTitle = L"적재함";
		g_mapIDiverInfo[wstring(svTex)][2].sLevel = L"Lv2";
		g_mapIDiverInfo[wstring(svTex)][2].sUnitName = L"허용무게";
		g_mapIDiverInfo[wstring(svTex)][2].sUnit = L"25 kg";
		g_mapIDiverInfo[wstring(svTex)][2].sUpgradeDesc = L"sUpgradeDesc";
		g_mapIDiverInfo[wstring(svTex)][2].sUpgradeSuccessDesc = L"sUpgradeSuccessDesc";
	}

	 void InitIDiverJaksalInfo(std::wstring_view svTex)
	{
		g_mapIDiverInfo[wstring(svTex)][1].iStep = 1;
		g_mapIDiverInfo[wstring(svTex)][1].iCost = 10;
		g_mapIDiverInfo[wstring(svTex)][1].iAmount = 15;
		g_mapIDiverInfo[wstring(svTex)][1].sTitle = L"작살총";
		g_mapIDiverInfo[wstring(svTex)][1].sLevel = L"낡은작살";
		g_mapIDiverInfo[wstring(svTex)][1].sUnitName = L"데미지";
		g_mapIDiverInfo[wstring(svTex)][1].sUnit = L"15";
		g_mapIDiverInfo[wstring(svTex)][1].sUpgradeDesc = L"sUpgradeDesc";
		g_mapIDiverInfo[wstring(svTex)][1].sUpgradeSuccessDesc = L"sUpgradeSuccessDesc";

		g_mapIDiverInfo[wstring(svTex)][2].iStep = 2;
		g_mapIDiverInfo[wstring(svTex)][2].iCost = 20;
		g_mapIDiverInfo[wstring(svTex)][2].iAmount = 25;
		g_mapIDiverInfo[wstring(svTex)][2].sTitle = L"작살총";
		g_mapIDiverInfo[wstring(svTex)][2].sLevel = L"강철작살";
		g_mapIDiverInfo[wstring(svTex)][2].sUnitName = L"데미지";
		g_mapIDiverInfo[wstring(svTex)][2].sUnit = L"25";
		g_mapIDiverInfo[wstring(svTex)][2].sUpgradeDesc = L"sUpgradeDesc";
		g_mapIDiverInfo[wstring(svTex)][2].sUpgradeSuccessDesc = L"sUpgradeSuccessDesc";
	}
}