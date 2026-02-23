#pragma once

#include "Engine_Define.h"

namespace IDiver
{
	typedef struct tagIDiverInfo
	{
		_uint iStep;
		_uint iCost;
		_uint iAmount;
		std::wstring sTitle;
		std::wstring sLevel;
		std::wstring sUnit;
		std::wstring sUnitName;
		std::wstring sUpgradeDesc;
		std::wstring sUpgradeSuccessDesc;
		
	} IDIVER_INFO;


	extern unordered_map<std::wstring, unordered_map<_uint, IDIVER_INFO>> g_mapIDiverInfo;

	void InitIDiverSansoInfo(std::wstring_view svTex);
	void InitIDiverClothesInfo(std::wstring_view svTex);
	void InitIDiverCageInfo(std::wstring_view svTex);
	void InitIDiverJaksalInfo(std::wstring_view svTex);
	void InitIDiverInfo();

	IDIVER_INFO& Get_Info(std::wstring_view svTex, _uint iLevel);
}