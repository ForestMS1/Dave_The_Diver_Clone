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
		 auto key = wstring(svTex);

		 // Lv.1
		 g_mapIDiverInfo[key][1].iStep = 1;
		 g_mapIDiverInfo[key][1].iCost = 0; 
		 g_mapIDiverInfo[key][1].iAmount = 90;
		 g_mapIDiverInfo[key][1].sTitle = L"산소통";
		 g_mapIDiverInfo[key][1].sLevel = L"Lv.1";
		 g_mapIDiverInfo[key][1].sUnitName = L"공기량";
		 g_mapIDiverInfo[key][1].sUnit = L"90 bar";
		 g_mapIDiverInfo[key][1].sUpgradeDesc = L"물속에서 호흡할 공기를 담은 공기통.\r\n업그레이드 하면 용량이 늘어난다.";
		 g_mapIDiverInfo[key][1].sUpgradeSuccessDesc = L"더 오래 잠수할 수 있습니다.";
		 // Lv.2
		 g_mapIDiverInfo[key][2].iStep = 2;
		 g_mapIDiverInfo[key][2].iCost = 65;
		 g_mapIDiverInfo[key][2].iAmount = 115;
		 g_mapIDiverInfo[key][2].sTitle = L"산소통";
		 g_mapIDiverInfo[key][2].sLevel = L"Lv.2";
		 g_mapIDiverInfo[key][2].sUnitName = L"공기량";
		 g_mapIDiverInfo[key][2].sUnit = L"115 bar";
		 g_mapIDiverInfo[key][2].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 g_mapIDiverInfo[key][2].sUpgradeSuccessDesc = L"공기량이 조금 더 늘어났습니다. \r\n탐사 시간이 길어집니다.";

		 // Lv.3
		 g_mapIDiverInfo[key][3].iStep = 3;
		 g_mapIDiverInfo[key][3].iCost = 225;
		 g_mapIDiverInfo[key][3].iAmount = 145;
		 g_mapIDiverInfo[key][3].sTitle = L"산소통";
		 g_mapIDiverInfo[key][3].sLevel = L"Lv.3";
		 g_mapIDiverInfo[key][3].sUnitName = L"공기량";
		 g_mapIDiverInfo[key][3].sUnit = L"145 bar";
		 g_mapIDiverInfo[key][3].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 g_mapIDiverInfo[key][3].sUpgradeSuccessDesc = L"공기가 넉넉해졌습니다. \r\n이제 \r\n대시를 조금 더 자주 써도 되겠군요.";

		 // Lv.4
		 g_mapIDiverInfo[key][4].iStep = 4;
		 g_mapIDiverInfo[key][4].iCost = 400;
		 g_mapIDiverInfo[key][4].iAmount = 180;
		 g_mapIDiverInfo[key][4].sTitle = L"산소통";
		 g_mapIDiverInfo[key][4].sLevel = L"Lv.4";
		 g_mapIDiverInfo[key][4].sUnitName = L"공기량";
		 g_mapIDiverInfo[key][4].sUnit = L"180 bar";
		 g_mapIDiverInfo[key][4].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 g_mapIDiverInfo[key][4].sUpgradeSuccessDesc = L"상당히 여유로운 용량입니다.\r\n 심해 초입 탐사도 끄떡없습니다.";

		 // Lv.5
		 g_mapIDiverInfo[key][5].iStep = 5;
		 g_mapIDiverInfo[key][5].iCost = 750;
		 g_mapIDiverInfo[key][5].iAmount = 220;
		 g_mapIDiverInfo[key][5].sTitle = L"산소통";
		 g_mapIDiverInfo[key][5].sLevel = L"Lv.5";
		 g_mapIDiverInfo[key][5].sUnitName = L"공기량";
		 g_mapIDiverInfo[key][5].sUnit = L"220 bar";
		 g_mapIDiverInfo[key][5].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 g_mapIDiverInfo[key][5].sUpgradeSuccessDesc = L"이제 산소 걱정을 덜고 \r\n채집에 더 집중할 수 있습니다.";

		 // Lv.6
		 g_mapIDiverInfo[key][6].iStep = 6;
		 g_mapIDiverInfo[key][6].iCost = 1500;
		 g_mapIDiverInfo[key][6].iAmount = 265;
		 g_mapIDiverInfo[key][6].sTitle = L"산소통";
		 g_mapIDiverInfo[key][6].sLevel = L"Lv.6";
		 g_mapIDiverInfo[key][6].sUnitName = L"공기량";
		 g_mapIDiverInfo[key][6].sUnit = L"265 bar";
		 g_mapIDiverInfo[key][6].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 g_mapIDiverInfo[key][6].sUpgradeSuccessDesc = L"전문 다이버급 산소통입니다. \r\n더 깊은 곳의 비밀을 파헤치세요.";

		 // Lv.7
		 g_mapIDiverInfo[key][7].iStep = 7;
		 g_mapIDiverInfo[key][7].iCost = 3000;
		 g_mapIDiverInfo[key][7].iAmount = 315;
		 g_mapIDiverInfo[key][7].sTitle = L"산소통";
		 g_mapIDiverInfo[key][7].sLevel = L"Lv.7";
		 g_mapIDiverInfo[key][7].sUnitName = L"공기량";
		 g_mapIDiverInfo[key][7].sUnit = L"315 bar";
		 g_mapIDiverInfo[key][7].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 g_mapIDiverInfo[key][7].sUpgradeSuccessDesc = L"용량이 어마어마하군요. \r\n블루홀의 깊은 곳도 \r\n이제 두렵지 않습니다.";

		 // Lv.8
		 g_mapIDiverInfo[key][8].iStep = 8;
		 g_mapIDiverInfo[key][8].iCost = 6000;
		 g_mapIDiverInfo[key][8].iAmount = 370;
		 g_mapIDiverInfo[key][8].sTitle = L"산소통";
		 g_mapIDiverInfo[key][8].sLevel = L"Lv.8";
		 g_mapIDiverInfo[key][8].sUnitName = L"공기량";
		 g_mapIDiverInfo[key][8].sUnit = L"370 bar";
		 g_mapIDiverInfo[key][8].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 g_mapIDiverInfo[key][8].sUpgradeSuccessDesc = L"최첨단 고압 공기통입니다. \r\n극한의 환경에서도 \r\n든든한 파트너입니다.";

		 // Lv.9
		 g_mapIDiverInfo[key][9].iStep = 9;
		 g_mapIDiverInfo[key][9].iCost = 11300;
		 g_mapIDiverInfo[key][9].iAmount = 425;
		 g_mapIDiverInfo[key][9].sTitle = L"산소통";
		 g_mapIDiverInfo[key][9].sLevel = L"Lv.9";
		 g_mapIDiverInfo[key][9].sUnitName = L"공기량";
		 g_mapIDiverInfo[key][9].sUnit = L"425 bar";
		 g_mapIDiverInfo[key][9].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 g_mapIDiverInfo[key][9].sUpgradeSuccessDesc = L"이 정도 용량이면 \r\n바다에서 살 수도 있겠는데요? \r\n엄청난 효율입니다.";

		 // Lv.10 (MAX)
		 g_mapIDiverInfo[key][10].iStep = 10;
		 g_mapIDiverInfo[key][10].iCost = 17000;
		 g_mapIDiverInfo[key][10].iAmount = 480;
		 g_mapIDiverInfo[key][10].sTitle = L"산소통";
		 g_mapIDiverInfo[key][10].sLevel = L"MAX";
		 g_mapIDiverInfo[key][10].sUnitName = L"공기량";
		 g_mapIDiverInfo[key][10].sUnit = L"480 bar";
		 g_mapIDiverInfo[key][10].sUpgradeDesc = L"이미 최대 레벨입니다. \r\n더 이상의 강화는 불필요합니다.";
		 g_mapIDiverInfo[key][10].sUpgradeSuccessDesc = L"전설적인 산소통입니다! \r\n이제 당신을 막을 수 있는 \r\n수심은 없습니다.";
		

	}

	 void InitIDiverClothesInfo(std::wstring_view svTex)
	{
		 std::wstring key = wstring(svTex);

		 // Lv.1 (기본 수심 40m)
		 g_mapIDiverInfo[key][1].iStep = 1;
		 g_mapIDiverInfo[key][1].iCost = 0;
		 g_mapIDiverInfo[key][1].iAmount = 40;
		 g_mapIDiverInfo[key][1].sTitle = L"잠수복";
		 g_mapIDiverInfo[key][1].sLevel = L"Lv.1";
		 g_mapIDiverInfo[key][1].sUnitName = L"한계수심";
		 g_mapIDiverInfo[key][1].sUnit = L"40 m";
		 g_mapIDiverInfo[key][1].sUpgradeDesc = L"더 깊은 바다로 들어갈 수 있는 잠수복.\r\n업그레이드 하면 더 깊이 잠수할 수 있다.";
		 g_mapIDiverInfo[key][1].sUpgradeSuccessDesc = L"더 깊게 잠수할 수 있습니다.";

		 // Lv.2 (튜토리얼 보상 개념)
		 g_mapIDiverInfo[key][2].iStep = 2;
		 g_mapIDiverInfo[key][2].iCost = 60;
		 g_mapIDiverInfo[key][2].iAmount = 80;
		 g_mapIDiverInfo[key][2].sTitle = L"잠수복";
		 g_mapIDiverInfo[key][2].sLevel = L"Lv.2";
		 g_mapIDiverInfo[key][2].sUnitName = L"한계수심";
		 g_mapIDiverInfo[key][2].sUnit = L"80 m";
		 g_mapIDiverInfo[key][2].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 g_mapIDiverInfo[key][2].sUpgradeSuccessDesc = L"심해 입구까지 \r\n들어갈 수 있습니다.";

		 // Lv.3
		 g_mapIDiverInfo[key][3].iStep = 3;
		 g_mapIDiverInfo[key][3].iCost = 225;
		 g_mapIDiverInfo[key][3].iAmount = 150;
		 g_mapIDiverInfo[key][3].sTitle = L"잠수복";
		 g_mapIDiverInfo[key][3].sLevel = L"Lv.3";
		 g_mapIDiverInfo[key][3].sUnitName = L"한계수심";
		 g_mapIDiverInfo[key][3].sUnit = L"150 m";
		 g_mapIDiverInfo[key][3].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 g_mapIDiverInfo[key][3].sUpgradeSuccessDesc = L"본격적인 \r\n심해 탐사가 가능해졌습니다.";

		 // Lv.4
		 g_mapIDiverInfo[key][4].iStep = 4;
		 g_mapIDiverInfo[key][4].iCost = 1000;
		 g_mapIDiverInfo[key][4].iAmount = 230;
		 g_mapIDiverInfo[key][4].sTitle = L"잠수복";
		 g_mapIDiverInfo[key][4].sLevel = L"Lv.4";
		 g_mapIDiverInfo[key][4].sUnitName = L"한계수심";
		 g_mapIDiverInfo[key][4].sUnit = L"230 m";
		 g_mapIDiverInfo[key][4].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 g_mapIDiverInfo[key][4].sUpgradeSuccessDesc = L"심해의 더 깊은 곳까지 \r\n탐사할 수 있습니다.";

		 // Lv.5
		 g_mapIDiverInfo[key][5].iStep = 5;
		 g_mapIDiverInfo[key][5].iCost = 2000;
		 g_mapIDiverInfo[key][5].iAmount = 375;
		 g_mapIDiverInfo[key][5].sTitle = L"잠수복";
		 g_mapIDiverInfo[key][5].sLevel = L"Lv.5";
		 g_mapIDiverInfo[key][5].sUnitName = L"한계수심";
		 g_mapIDiverInfo[key][5].sUnit = L"375 m";
		 g_mapIDiverInfo[key][5].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 g_mapIDiverInfo[key][5].sUpgradeSuccessDesc = L"심해 생태계의 끝자락을 \r\n탐사할 수 있습니다.";

		 // Lv.6
		 g_mapIDiverInfo[key][6].iStep = 6;
		 g_mapIDiverInfo[key][6].iCost = 3500;
		 g_mapIDiverInfo[key][6].iAmount = 540;
		 g_mapIDiverInfo[key][6].sTitle = L"잠수복";
		 g_mapIDiverInfo[key][6].sLevel = L"Lv.6";
		 g_mapIDiverInfo[key][6].sUnitName = L"한계수심";
		 g_mapIDiverInfo[key][6].sUnit = L"540 m";
		 g_mapIDiverInfo[key][6].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 g_mapIDiverInfo[key][6].sUpgradeSuccessDesc = L"잠수함 수준의 수심까지 \r\n내려갈 수 있습니다.";

		 // Lv.7 (내열 잠수복 1단계)
		 g_mapIDiverInfo[key][7].iStep = 7;
		 g_mapIDiverInfo[key][7].iCost = 4800;
		 g_mapIDiverInfo[key][7].iAmount = 560;
		 g_mapIDiverInfo[key][7].sTitle = L"내열 잠수복";
		 g_mapIDiverInfo[key][7].sLevel = L"내열 Lv.1";
		 g_mapIDiverInfo[key][7].sUnitName = L"한계수심";
		 g_mapIDiverInfo[key][7].sUnit = L"560 m";
		 g_mapIDiverInfo[key][7].sUpgradeDesc = L"차가운 바다에서도 \r\n버틸 수 있는 내열 기능이 \r\n추가되었습니다.";
		 g_mapIDiverInfo[key][7].sUpgradeSuccessDesc = L"빙하 지역 탐사가 \r\n가능해졌습니다.";

		 // Lv.8 (MAX - 내열 잠수복 2단계)
		 g_mapIDiverInfo[key][8].iStep = 8;
		 g_mapIDiverInfo[key][8].iCost = 6600;
		 g_mapIDiverInfo[key][8].iAmount = 800;
		 g_mapIDiverInfo[key][8].sTitle = L"내열 잠수복";
		 g_mapIDiverInfo[key][8].sLevel = L"MAX";
		 g_mapIDiverInfo[key][8].sUnitName = L"한계수심";
		 g_mapIDiverInfo[key][8].sUnit = L"800 m";
		 g_mapIDiverInfo[key][8].sUpgradeDesc = L"모든 수역을 탐사할 수 있는 \r\n최강의 잠수복입니다.";
		 g_mapIDiverInfo[key][8].sUpgradeSuccessDesc = L"이제 심해와 빙하, \r\n모든 곳이 당신의 구역입니다.";


		//g_mapIDiverInfo[wstring(svTex)][1].iStep = 1;
		//g_mapIDiverInfo[wstring(svTex)][1].iCost = 10;
		//g_mapIDiverInfo[wstring(svTex)][1].iAmount = 15;
		//g_mapIDiverInfo[wstring(svTex)][1].sTitle = L"잠수복";
		//g_mapIDiverInfo[wstring(svTex)][1].sLevel = L"Lv1";
		//g_mapIDiverInfo[wstring(svTex)][1].sUnitName = L"한계수심";
		//g_mapIDiverInfo[wstring(svTex)][1].sUnit = L"15 m";
		//g_mapIDiverInfo[wstring(svTex)][1].sUpgradeDesc = L"더 깊은 바다로 들어갈 수 있는 잠수복.\n업그레이드 하면 더 깊이 잠수할 수 있다.";
		//g_mapIDiverInfo[wstring(svTex)][1].sUpgradeSuccessDesc = L"더 깊게 잠수할 수 있습니다.";

		//g_mapIDiverInfo[wstring(svTex)][2].iStep = 2;
		//g_mapIDiverInfo[wstring(svTex)][2].iCost = 20;
		//g_mapIDiverInfo[wstring(svTex)][2].iAmount = 25;
		//g_mapIDiverInfo[wstring(svTex)][2].sTitle = L"잠수복";
		//g_mapIDiverInfo[wstring(svTex)][2].sLevel = L"Lv2";
		//g_mapIDiverInfo[wstring(svTex)][2].sUnitName = L"한계수심";
		//g_mapIDiverInfo[wstring(svTex)][2].sUnit = L"25 m";
		//g_mapIDiverInfo[wstring(svTex)][2].sUpgradeDesc = L"sUpgradeDesc";
		//g_mapIDiverInfo[wstring(svTex)][2].sUpgradeSuccessDesc = L"sUpgradeSuccessDesc";
	}

	 void InitIDiverCageInfo(std::wstring_view svTex)
	{
		 std::wstring key = wstring(svTex);

		 // Lv.1
		 g_mapIDiverInfo[key][1].iStep = 1;
		 g_mapIDiverInfo[key][1].iCost = 0;
		 g_mapIDiverInfo[key][1].iAmount = 9; // 실제 게임 시작 무게
		 g_mapIDiverInfo[key][1].sTitle = L"적재함";
		 g_mapIDiverInfo[key][1].sLevel = L"Lv.1";
		 g_mapIDiverInfo[key][1].sUnitName = L"허용무게";
		 g_mapIDiverInfo[key][1].sUnit = L"9 kg";
		 g_mapIDiverInfo[key][1].sUpgradeDesc = L"물 속에서 획득한 것을 담는 박스.\r\n업그레이드 하면 \r\n한번에 담는 용량이 증가한다.";
		 g_mapIDiverInfo[key][1].sUpgradeSuccessDesc = L"더 많이 담을 수 있습니다.";

		 // Lv.2
		 g_mapIDiverInfo[key][2].iStep = 2;
		 g_mapIDiverInfo[key][2].iCost = 55;
		 g_mapIDiverInfo[key][2].iAmount = 13;
		 g_mapIDiverInfo[key][2].sTitle = L"적재함";
		 g_mapIDiverInfo[key][2].sLevel = L"Lv.2";
		 g_mapIDiverInfo[key][2].sUnitName = L"허용무게";
		 g_mapIDiverInfo[key][2].sUnit = L"13 kg";
		 g_mapIDiverInfo[key][2].sUpgradeSuccessDesc = L"작은 물고기 몇 마리를 \r\n더 담을 여유가 생겼습니다.";

		 // Lv.3
		 g_mapIDiverInfo[key][3].iStep = 3;
		 g_mapIDiverInfo[key][3].iCost = 110;
		 g_mapIDiverInfo[key][3].iAmount = 19;
		 g_mapIDiverInfo[key][3].sTitle = L"적재함";
		 g_mapIDiverInfo[key][3].sLevel = L"Lv.3";
		 g_mapIDiverInfo[key][3].sUnitName = L"허용무게";
		 g_mapIDiverInfo[key][3].sUnit = L"19 kg";
		 g_mapIDiverInfo[key][3].sUpgradeSuccessDesc = L"적재함이 조금 더 묵직해졌습니다.\r\n 채집이 즐거워지겠네요!";

		 // Lv.4
		 g_mapIDiverInfo[key][4].iStep = 4;
		 g_mapIDiverInfo[key][4].iCost = 325;
		 g_mapIDiverInfo[key][4].iAmount = 28;
		 g_mapIDiverInfo[key][4].sTitle = L"적재함";
		 g_mapIDiverInfo[key][4].sLevel = L"Lv.4";
		 g_mapIDiverInfo[key][4].sUnitName = L"허용무게";
		 g_mapIDiverInfo[key][4].sUnit = L"28 kg";
		 g_mapIDiverInfo[key][4].sUpgradeSuccessDesc = L"이제 웬만한 물고기는 \r\n가득 채워도 \r\n속도가 느려지지 않습니다.";

		 // Lv.5
		 g_mapIDiverInfo[key][5].iStep = 5;
		 g_mapIDiverInfo[key][5].iCost = 675;
		 g_mapIDiverInfo[key][5].iAmount = 42;
		 g_mapIDiverInfo[key][5].sTitle = L"적재함";
		 g_mapIDiverInfo[key][5].sLevel = L"Lv.5";
		 g_mapIDiverInfo[key][5].sUnitName = L"허용무게";
		 g_mapIDiverInfo[key][5].sUnit = L"42 kg";
		 g_mapIDiverInfo[key][5].sUpgradeSuccessDesc = L"상당히 넓은 공간입니다. \r\n대형 어종 탐사도 문제없겠군요!";

		 // Lv.6
		 g_mapIDiverInfo[key][6].iStep = 6;
		 g_mapIDiverInfo[key][6].iCost = 1375;
		 g_mapIDiverInfo[key][6].iAmount = 63;
		 g_mapIDiverInfo[key][6].sTitle = L"적재함";
		 g_mapIDiverInfo[key][6].sLevel = L"Lv.6";
		 g_mapIDiverInfo[key][6].sUnitName = L"허용무게";
		 g_mapIDiverInfo[key][6].sUnit = L"63 kg";
		 g_mapIDiverInfo[key][6].sUpgradeSuccessDesc = L"전문 채집가용 적재함입니다. \r\n반초가 아주 좋아하겠는걸요?";

		 // Lv.7
		 g_mapIDiverInfo[key][7].iStep = 7;
		 g_mapIDiverInfo[key][7].iCost = 2750;
		 g_mapIDiverInfo[key][7].iAmount = 94;
		 g_mapIDiverInfo[key][7].sTitle = L"적재함";
		 g_mapIDiverInfo[key][7].sLevel = L"Lv.7";
		 g_mapIDiverInfo[key][7].sUnitName = L"허용무게";
		 g_mapIDiverInfo[key][7].sUnit = L"94 kg";
		 g_mapIDiverInfo[key][7].sUpgradeSuccessDesc = L"어마어마한 양을 담을 수 있습니다. \r\n바다를 통째로 가져올 셈인가요?";

		 // Lv.8
		 g_mapIDiverInfo[key][8].iStep = 8;
		 g_mapIDiverInfo[key][8].iCost = 5500;
		 g_mapIDiverInfo[key][8].iAmount = 125;
		 g_mapIDiverInfo[key][8].sTitle = L"적재함";
		 g_mapIDiverInfo[key][8].sLevel = L"Lv.8";
		 g_mapIDiverInfo[key][8].sUnitName = L"허용무게";
		 g_mapIDiverInfo[key][8].sUnit = L"125 kg";
		 g_mapIDiverInfo[key][8].sUpgradeSuccessDesc = L"적재함의 끝판왕급 성능입니다. \r\n인양 드론과 함께 쓰면 무적입니다.";

		 // Lv.9
		 g_mapIDiverInfo[key][9].iStep = 9;
		 g_mapIDiverInfo[key][9].iCost = 9950;
		 g_mapIDiverInfo[key][9].iAmount = 155;
		 g_mapIDiverInfo[key][9].sTitle = L"적재함";
		 g_mapIDiverInfo[key][9].sLevel = L"Lv.9";
		 g_mapIDiverInfo[key][9].sUnitName = L"허용무게";
		 g_mapIDiverInfo[key][9].sUnit = L"155 kg";
		 g_mapIDiverInfo[key][9].sUpgradeSuccessDesc = L"공간이 남아돌 정도입니다. \r\n모든 보물을 싹쓸이해 보세요!";

		 // Lv.10 (MAX)
		 g_mapIDiverInfo[key][10].iStep = 10;
		 g_mapIDiverInfo[key][10].iCost = 15500;
		 g_mapIDiverInfo[key][10].iAmount = 185;
		 g_mapIDiverInfo[key][10].sTitle = L"적재함";
		 g_mapIDiverInfo[key][10].sLevel = L"MAX";
		 g_mapIDiverInfo[key][10].sUnitName = L"허용무게";
		 g_mapIDiverInfo[key][10].sUnit = L"185 kg";
		 g_mapIDiverInfo[key][10].sUpgradeSuccessDesc = L"적재함이 한계에 도달했습니다. \r\n이제 당신은 바다의 운송 트럭입니다!";

		 // 공통 Desc 처리 (필요시)
		 for (int i = 2; i <= 10; ++i) {
			 g_mapIDiverInfo[key][i].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 }

		//g_mapIDiverInfo[wstring(svTex)][1].iStep = 1;
		//g_mapIDiverInfo[wstring(svTex)][1].iCost = 10;
		//g_mapIDiverInfo[wstring(svTex)][1].iAmount = 15;
		//g_mapIDiverInfo[wstring(svTex)][1].sTitle = L"적재함";
		//g_mapIDiverInfo[wstring(svTex)][1].sLevel = L"Lv1";
		//g_mapIDiverInfo[wstring(svTex)][1].sUnitName = L"허용무게";
		//g_mapIDiverInfo[wstring(svTex)][1].sUnit = L"15 kg";
		//g_mapIDiverInfo[wstring(svTex)][1].sUpgradeDesc = L"물 속에서 획득한 것을 담는 박스.\n업그레이드 하면\n한번에 담는 용량이 증가한다.";
		//g_mapIDiverInfo[wstring(svTex)][1].sUpgradeSuccessDesc = L"더 많이 담을수 있습니다.";

		//g_mapIDiverInfo[wstring(svTex)][2].iStep = 2;
		//g_mapIDiverInfo[wstring(svTex)][2].iCost = 20;
		//g_mapIDiverInfo[wstring(svTex)][2].iAmount = 25;
		//g_mapIDiverInfo[wstring(svTex)][2].sTitle = L"적재함";
		//g_mapIDiverInfo[wstring(svTex)][2].sLevel = L"Lv2";
		//g_mapIDiverInfo[wstring(svTex)][2].sUnitName = L"허용무게";
		//g_mapIDiverInfo[wstring(svTex)][2].sUnit = L"25 kg";
		//g_mapIDiverInfo[wstring(svTex)][2].sUpgradeDesc = L"sUpgradeDesc";
		//g_mapIDiverInfo[wstring(svTex)][2].sUpgradeSuccessDesc = L"sUpgradeSuccessDesc";
	}

	 void InitIDiverJaksalInfo(std::wstring_view svTex)
	{
		 std::wstring key = wstring(svTex);

		 // Lv.1 (낡은 작살)
		 g_mapIDiverInfo[key][1].iStep = 1;
		 g_mapIDiverInfo[key][1].iCost = 0;
		 g_mapIDiverInfo[key][1].iAmount = 15;
		 g_mapIDiverInfo[key][1].sTitle = L"작살총";
		 g_mapIDiverInfo[key][1].sLevel = L"낡은 작살";
		 g_mapIDiverInfo[key][1].sUnitName = L"데미지";
		 g_mapIDiverInfo[key][1].sUnit = L"15";
		 g_mapIDiverInfo[key][1].sUpgradeDesc = L"물고기를 포획할 수 있는 작살.\r\n업그레이드 하면 \r\n물고기에게 입히는 피해가 증가한다.";
		 g_mapIDiverInfo[key][1].sUpgradeSuccessDesc = L"작살이 조금 더 날카로워졌습니다.";

		 // Lv.2
		 g_mapIDiverInfo[key][2].iStep = 2;
		 g_mapIDiverInfo[key][2].iCost = 50;
		 g_mapIDiverInfo[key][2].iAmount = 19;
		 g_mapIDiverInfo[key][2].sTitle = L"작살총";
		 g_mapIDiverInfo[key][2].sLevel = L"기본 작살";
		 g_mapIDiverInfo[key][2].sUnitName = L"데미지";
		 g_mapIDiverInfo[key][2].sUnit = L"19";
		 g_mapIDiverInfo[key][2].sUpgradeSuccessDesc = L"이제 작은 물고기 정도는 \r\n한 번에 꿰뚫을 수 있습니다.";

		 // Lv.3
		 g_mapIDiverInfo[key][3].iStep = 3;
		 g_mapIDiverInfo[key][3].iCost = 150;
		 g_mapIDiverInfo[key][3].iAmount = 24;
		 g_mapIDiverInfo[key][3].sTitle = L"작살총";
		 g_mapIDiverInfo[key][3].sLevel = L"강철 작살";
		 g_mapIDiverInfo[key][3].sUnitName = L"데미지";
		 g_mapIDiverInfo[key][3].sUnit = L"24";
		 g_mapIDiverInfo[key][3].sUpgradeSuccessDesc = L"강철 촉을 달아 관통력이\r\n 비약적으로 상승했습니다.";

		 // Lv.4
		 g_mapIDiverInfo[key][4].iStep = 4;
		 g_mapIDiverInfo[key][4].iCost = 500;
		 g_mapIDiverInfo[key][4].iAmount = 30;
		 g_mapIDiverInfo[key][4].sTitle = L"작살총";
		 g_mapIDiverInfo[key][4].sLevel = L"날카로운 작살";
		 g_mapIDiverInfo[key][4].sUnitName = L"데미지";
		 g_mapIDiverInfo[key][4].sUnit = L"30";
		 g_mapIDiverInfo[key][4].sUpgradeSuccessDesc = L"작살 끝이 아주 예리합니다.\r\n 상어와 싸울 때 유용하겠네요.";

		 // Lv.5
		 g_mapIDiverInfo[key][5].iStep = 5;
		 g_mapIDiverInfo[key][5].iCost = 1100;
		 g_mapIDiverInfo[key][5].iAmount = 36;
		 g_mapIDiverInfo[key][5].sTitle = L"작살총";
		 g_mapIDiverInfo[key][5].sLevel = L"강화 작살";
		 g_mapIDiverInfo[key][5].sUnitName = L"데미지";
		 g_mapIDiverInfo[key][5].sUnit = L"36";
		 g_mapIDiverInfo[key][5].sUpgradeSuccessDesc = L"강화 처리가 되어 \r\n쉽게 무뎌지지 않는 \r\n강력한 작살입니다.";

		 // Lv.6
		 g_mapIDiverInfo[key][6].iStep = 6;
		 g_mapIDiverInfo[key][6].iCost = 2500;
		 g_mapIDiverInfo[key][6].iAmount = 43;
		 g_mapIDiverInfo[key][6].sTitle = L"작살총";
		 g_mapIDiverInfo[key][6].sLevel = L"초합금 작살";
		 g_mapIDiverInfo[key][6].sUnitName = L"데미지";
		 g_mapIDiverInfo[key][6].sUnit = L"43";
		 g_mapIDiverInfo[key][6].sUpgradeSuccessDesc = L"첨단 소재를 사용해 \r\n무게는 줄이고 파괴력은 높였습니다.";

		 // Lv.7
		 g_mapIDiverInfo[key][7].iStep = 7;
		 g_mapIDiverInfo[key][7].iCost = 5200;
		 g_mapIDiverInfo[key][7].iAmount = 51;
		 g_mapIDiverInfo[key][7].sTitle = L"작살총";
		 g_mapIDiverInfo[key][7].sLevel = L"대형 작살";
		 g_mapIDiverInfo[key][7].sUnitName = L"데미지";
		 g_mapIDiverInfo[key][7].sUnit = L"51";
		 g_mapIDiverInfo[key][7].sUpgradeSuccessDesc = L"웬만한 바다 생물은 \r\n이 작살 하나로 모두 제압이 가능합니다.";

		 // Lv.8 (MAX)
		 g_mapIDiverInfo[key][8].iStep = 8;
		 g_mapIDiverInfo[key][8].iCost = 11000;
		 g_mapIDiverInfo[key][8].iAmount = 60;
		 g_mapIDiverInfo[key][8].sTitle = L"작살총";
		 g_mapIDiverInfo[key][8].sLevel = L"최종병기작살";
		 g_mapIDiverInfo[key][8].sUnitName = L"데미지";
		 g_mapIDiverInfo[key][8].sUnit = L"60";
		 g_mapIDiverInfo[key][8].sUpgradeSuccessDesc = L"블루홀의 최상위 포식자조차 \r\n두려워할 최강의 작살입니다!";

		 // 공통 Desc 처리
		 for (int i = 2; i <= 8; ++i) {
			 g_mapIDiverInfo[key][i].sUpgradeDesc = g_mapIDiverInfo[key][1].sUpgradeDesc;
		 }


		//g_mapIDiverInfo[wstring(svTex)][1].iStep = 1;
		//g_mapIDiverInfo[wstring(svTex)][1].iCost = 10;
		//g_mapIDiverInfo[wstring(svTex)][1].iAmount = 15;
		//g_mapIDiverInfo[wstring(svTex)][1].sTitle = L"작살총";
		//g_mapIDiverInfo[wstring(svTex)][1].sLevel = L"낡은작살";
		//g_mapIDiverInfo[wstring(svTex)][1].sUnitName = L"데미지";
		//g_mapIDiverInfo[wstring(svTex)][1].sUnit = L"15";
		//g_mapIDiverInfo[wstring(svTex)][1].sUpgradeDesc = L"물고기를 포획할 수 있는 작살.\n업그레이드 하면\n물고기에게 입히는 피해가 증가 한다.";
		//g_mapIDiverInfo[wstring(svTex)][1].sUpgradeSuccessDesc = L"더 강하게 공격할수 있습니다.";

		//g_mapIDiverInfo[wstring(svTex)][2].iStep = 2;
		//g_mapIDiverInfo[wstring(svTex)][2].iCost = 20;
		//g_mapIDiverInfo[wstring(svTex)][2].iAmount = 25;
		//g_mapIDiverInfo[wstring(svTex)][2].sTitle = L"작살총";
		//g_mapIDiverInfo[wstring(svTex)][2].sLevel = L"강철작살";
		//g_mapIDiverInfo[wstring(svTex)][2].sUnitName = L"데미지";
		//g_mapIDiverInfo[wstring(svTex)][2].sUnit = L"25";
		//g_mapIDiverInfo[wstring(svTex)][2].sUpgradeDesc = L"sUpgradeDesc";
		//g_mapIDiverInfo[wstring(svTex)][2].sUpgradeSuccessDesc = L"sUpgradeSuccessDesc";
	}
}