#include "pch.h"
#include "CTransition.h"
#include "CAssetDefaultFont.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CManagement.h"
#include "CLogo.h"
#include "CProtoMgr.h"
#include "CShip.h"
#include "CSushi.h"
#include "CDive.h"
#include "CMapMgr.h"
#include "CAssetJson.h"
#include "CAssetGlb.h"
#include "CGlbTex.h"
#include "CAttackReadyArmTex.h"
#include "CAssetTexture.h"
#include "CTargetArrowTex.h"
#include "CAssetCubeTexture.h"
#include "CLineBuffer.h"
#include "CTransitionImg.h"
#include "CTransitionBG.h"
#include "CTransitionFade.h"
#include "CTransitionTxt.h"
#include "CTransitionFace.h"

CTransition::CTransition(SCENE_ID eSrcScene, SCENE_ID eDstScene)
	: m_eSrcScene(eSrcScene)
	, m_eDstScene(eDstScene)
	, m_bFinish(false)
	, m_Crt({})
	, m_hThread(nullptr)
	, m_reserveTransfer({})
{
}

CTransition::~CTransition()
{
}

pair<std::wstring, map<const std::wstring, CLayer*>> CTransition::Before_SceneChange()
{
	return m_reserveTransfer;
}

void CTransition::After_SceneChange(const pair<std::wstring, map<const std::wstring, CLayer*>>& transfer)
{
	m_reserveTransfer = transfer;
}

	

HRESULT CTransition::Transition_INIT_TO_LOGO()
{
	
	CAssetMgr::GetInstance()->AddAsset(L"Json_Map", CAssetJson::Create(L"../Bin/Data/Map1.json"));
	CAssetMgr::GetInstance()->LoadAsset(L"Json_Map");


	//CAssetMgr::GetInstance()->AddAsset(L"GLB_File", CAssetGlb::Create(L"../Bin/Resource/PolybrushMesh-3786884.glb"));
	CAssetMgr::GetInstance()->AddAsset(L"GLB_File", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"GLB_File");

	CAssetMgr::GetInstance()->AddAsset(L"BackGround_GLB_File", CAssetGlb::Create(L"../Bin/Resource/Glb/BackGround.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"BackGround_GLB_File");

	

	CProtoMgr::GetInstance()->Ready_Prototype(L"GLB_File",CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_File")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_File")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_File")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_File")->Get_Index()));

	CProtoMgr::GetInstance()->Ready_Prototype(L"BackGround_GLB_File", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"BackGround_GLB_File")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"BackGround_GLB_File")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"BackGround_GLB_File")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"BackGround_GLB_File")->Get_Index()));


	CAssetMgr::GetInstance()->AddAsset(L"BackGround", CAssetTexture::Create(L"../Bin/Resource/Texture/Map/BackGround1.png"));

	CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BackGroundSea", CTexture::Create(L"BackGround"));
	


	// Write By SY
	{
		Common_Logo_Env_Load();
	}
	m_sComment = L"Transition_INIT_TO_LOGO COMPLETE";
#ifdef _DEBUG
	//Sleep(500);
#endif // DEBUG
	m_bFinish = true;

	return S_OK;
}

HRESULT CTransition::Transition_LOGO_TO_SHIP()
{

	if (FAILED(Common_SHIP_Load()))
	{
		return E_FAIL;
	}


	m_sComment = L"Transition_LOGO_TO_SHIP COMPLETE";
//#ifdef _DEBUG
//	Sleep(500);
//#endif // DEBUG
	m_bFinish = true;

	return S_OK;
}

HRESULT CTransition::Transition_SHIP_TO_LOGO()
{
	m_sComment = L"Transition_SHIP_TO_LOGO COMPLETE";
//#ifdef _DEBUG
//	Sleep(500);
//#endif // DEBUG
	m_bFinish = true;

	return S_OK;
}

HRESULT CTransition::Transition_SHIP_TO_DIVE()
{
	m_sComment = L"Transition_SHIP_TO_DIVE COMPLETE";

	for (int i = 0; i < 8; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Dive_Player/Idle/Idle0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerIdle", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/Dive_Player/Up_Move/MoveUp0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerMoveUp", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/Dive_Player/Side_Up/MoveSideUp0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerMoveSideUp", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/Dive_Player/Side_Move/MoveSide0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerMoveSide", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/Dive_Player/Side_Down/MoveSideDown0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerMoveSideDown", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/Dive_Player/Down_Move/MoveDown0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerMoveDown", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/Dive_Player/AttackFight/AttackFight0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerAttackFight", CAssetTexture::Create(s.c_str()));
	}
	for (int i = 0; i < 4; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Dive_Player/Tanning/Tanning0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerTanning", CAssetTexture::Create(s.c_str()));
	}

	for (int i = 0; i < 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Dive_Player/Attack/AttackReady0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerAttackReady", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/Dive_Player/PickUp/PickUp0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerPickUp", CAssetTexture::Create(s.c_str()));
	}
	for (int i = 0; i < 4; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Dive_Player/MeleeDaggerAtk/MeleeDaggerAtk0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerMeleeDaggerAttack", CAssetTexture::Create(s.c_str()));
	}
	for (int i = 0; i < 3; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Dive_Player/Open/Open0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerOpen", CAssetTexture::Create(s.c_str()));
	}

	CAssetMgr::GetInstance()->AddAsset(L"Tex_AttackReadyArm", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/Attack/AttackReadyArms.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_TargetCurveStart", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/Attack_Effect/Target_CurveStart.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_TargetArrow", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/Attack_Effect/Target_Arrow.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_HarpoonGun", CAssetTexture::Create(L"../Bin/Resource/Texture/Weapon/HarpoonGun.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_HarpoonProjectile", CAssetTexture::Create(L"../Bin/Resource/Texture/Weapon/HarpoonProjectile.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_BasicRifle", CAssetTexture::Create(L"../Bin/Resource/Texture/Weapon/BasicRifle.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Bullet", CAssetTexture::Create(L"../Bin/Resource/Texture/Weapon/Bullet.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerAttackFire", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/AttackFire/AttackFire01.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerAttackFail", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/AttackFail/AttackFail01.png"));

	// DiveItemBox
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Chest_A", CAssetTexture::Create(L"../Bin/Resource/Texture/Box/Chest_A.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Chest_Aopen", CAssetTexture::Create(L"../Bin/Resource/Texture/Box/Chest_Aopen.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Chest_Box", CAssetTexture::Create(L"../Bin/Resource/Texture/Box/Chest_Box.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Chest_Box_open", CAssetTexture::Create(L"../Bin/Resource/Texture/Box/Chest_Box_open.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Chest_Weapon", CAssetTexture::Create(L"../Bin/Resource/Texture/Box/Chest_Weapon.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Chest_WeaponOpen", CAssetTexture::Create(L"../Bin/Resource/Texture/Box/Chest_WeaponOpen.png"));

	// Item
	CAssetMgr::GetInstance()->AddAsset(L"Tex_O2Capsule", CAssetTexture::Create(L"../Bin/Resource/Texture/Item/Item_O2Capsule.png"));


	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerIdleTexture", Engine::CTexture::Create(L"Tex_DivePlayerIdle"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerMoveUpTexture", Engine::CTexture::Create(L"Tex_DivePlayerMoveUp"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerMoveSideUpTexture", Engine::CTexture::Create(L"Tex_DivePlayerMoveSideUp"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerMoveSideTexture", Engine::CTexture::Create(L"Tex_DivePlayerMoveSide"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerMoveSideDownTexture", Engine::CTexture::Create(L"Tex_DivePlayerMoveSideDown"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerMoveDownTexture", Engine::CTexture::Create(L"Tex_DivePlayerMoveDown"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerTanningTexture", Engine::CTexture::Create(L"Tex_DivePlayerTanning"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerOpenTexture", Engine::CTexture::Create(L"Tex_DivePlayerOpen"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerPickUpTexture", Engine::CTexture::Create(L"Tex_DivePlayerPickUp"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerAttackReadyTexture", Engine::CTexture::Create(L"Tex_DivePlayerAttackReady"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerAttackFireTexture", Engine::CTexture::Create(L"Tex_DivePlayerAttackFire"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerAttackFightTexture", Engine::CTexture::Create(L"Tex_DivePlayerAttackFight"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerAttackFailTexture", Engine::CTexture::Create(L"Tex_DivePlayerAttackFail"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerMeleeDaggerAttack", Engine::CTexture::Create(L"Tex_DivePlayerMeleeDaggerAttack"))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_AttackReadyArmTexture", Engine::CTexture::Create(L"Tex_AttackReadyArm"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HarpoonTexture", Engine::CTexture::Create(L"Tex_HarpoonGun"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HarpoonProjectileTexture", Engine::CTexture::Create(L"Tex_HarpoonProjectile"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BasicRifleTexture", Engine::CTexture::Create(L"Tex_BasicRifle"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BulletTexture", Engine::CTexture::Create(L"Tex_Bullet"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_AttackReadyArmBuffer", Engine::CAttackReadyArmTex::Create())))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TargetArrowBuffer", Engine::CTargetArrowTex::Create())))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TargetCurveStartTexture", Engine::CTexture::Create(L"Tex_TargetCurveStart"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TargetArrowGunTexture", Engine::CTexture::Create(L"Tex_TargetArrow"))))
		return E_FAIL;


	// 작살 줄
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_LineBuffer", Engine::CLineBuffer::Create())))
		return E_FAIL;

	//테스트용
	CAssetMgr::GetInstance()->AddAsset(L"Tex_TestFish", CAssetTexture::Create(L"../Bin/Resource/Texture/Player1.jpg"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TestFishTexture", Engine::CTexture::Create(L"Tex_TestFish"))))
		return E_FAIL;


	CAssetMgr::GetInstance()->LoadAsset();
#ifdef _DEBUG
	//Sleep(500);
#endif // DEBUG
	m_bFinish = true;

	return S_OK;
}

HRESULT CTransition::Transition_DIVE_TO_SHIP()
{
	m_sComment = L"Transition_DIVE_TO_SHIP COMPLETE";
//#ifdef _DEBUG
//	Sleep(500);
//#endif // DEBUG
	m_bFinish = true;

	return S_OK;
}

HRESULT CTransition::Transition_SHIP_TO_SUSHI()
{
	m_sComment = L"Transition_SHIP_TO_SUSHI COMPLETE";
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Background", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/background.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BackgroundTex", Engine::CTexture::Create(L"Tex_Background"))))
		return E_FAIL;

	CAssetMgr::GetInstance()->AddAsset(L"Tex_Roof", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/Roof.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RoofTex", Engine::CTexture::Create(L"Tex_Roof"))))
		return E_FAIL;

	////////////
	

	CAssetMgr::GetInstance()->AddAsset(L"Tex_Light", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/light.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_LightTex", Engine::CTexture::Create(L"Tex_Light"))))
		return E_FAIL; 

	CAssetMgr::GetInstance()->AddAsset(L"Tex_Fence", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/fence.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_FenceTex", Engine::CTexture::Create(L"Tex_Fence"))))
		return E_FAIL;

	CAssetMgr::GetInstance()->AddAsset(L"Tex_Wood", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/wood.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WoodTex", Engine::CTexture::Create(L"Tex_Wood"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Hood", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/Sushi_Hood.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HoodTex", Engine::CTexture::Create(L"Tex_Hood"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Table", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/Table.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TableTex", Engine::CTexture::Create(L"Tex_Table"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Menu", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/Menu.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_MenuTex", Engine::CTexture::Create(L"Tex_Menu"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Frame", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/PatternFrame.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_PatternFrameTex", Engine::CTexture::Create(L"Tex_Frame"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Sign", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/Sign.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SignTex", Engine::CTexture::Create(L"Tex_Sign"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Chair", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/Chair.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ChairTex", Engine::CTexture::Create(L"Tex_Chair"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Kitchen", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/Kitchen.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_KitchenTex", Engine::CTexture::Create(L"Tex_Kitchen"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Speaker", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/Speaker.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SpeakerTex", Engine::CTexture::Create(L"Tex_Speaker"))))
		return E_FAIL;
	/////////애니메이션
	for (int i = 1; i < 10; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/BackGround/tank/Sushibar_tank0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishTank", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_FishTankTexture", Engine::CTexture::Create(L"Tex_FishTank"))))
		return E_FAIL;

	for (int i = 0; i < 11; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/BackGround/Open/Open" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Open", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_OpenTexture", Engine::CTexture::Create(L"Tex_Open"))))
		return E_FAIL;

	for (int i = 1; i < 4; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Bancho/Bancho_Cook0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Cook", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CookTexture", Engine::CTexture::Create(L"Tex_Cook"))))
		return E_FAIL;

	for (int i = 1; i < 6; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Bancho/Bancho_Idle00" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_BanchoIdle", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BanchoIdleTexture", Engine::CTexture::Create(L"Tex_BanchoIdle"))))
		return E_FAIL;

	for (int i = 0; i < 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Dave/dave_idle" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DaveIdle", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DaveIdleTexture", Engine::CTexture::Create(L"Tex_DaveIdle"))))
		return E_FAIL;
	for (int i = 0; i < 8; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Dave/dave_run" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DaveRun", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DaveRunTexture", Engine::CTexture::Create(L"Tex_DaveRun"))))
		return E_FAIL;
	for (int i = 0; i < 8; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Dave/dave_walk" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DaveWalk", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DaveWalkTexture", Engine::CTexture::Create(L"Tex_DaveWalk"))))
		return E_FAIL;
	for (int i = 0; i < 8; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Dave/dave_tired" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DaveTired", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DaveTiredTexture", Engine::CTexture::Create(L"Tex_DaveTired"))))
		return E_FAIL;
	for (int i = 0; i < 8; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Dave/dave_sushiRun" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DaveSushiRun", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DaveSushiRunTexture", Engine::CTexture::Create(L"Tex_DaveSushiRun"))))
		return E_FAIL;
	for (int i = 0; i < 8; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Dave/dave_sushiTired" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DaveSushiTired", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DaveSushiTiredTexture", Engine::CTexture::Create(L"Tex_DaveSushiTired"))))
		return E_FAIL;
	for (int i = 0; i < 8; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Dave/dave_sushiWalk" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DaveSushiWalk", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DaveSushiWalkTexture", Engine::CTexture::Create(L"Tex_DaveSushiWalk"))))
		return E_FAIL;

	for (int i = 1; i < 9; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer1/Walk0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer1Walk", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer1WalkTexture", Engine::CTexture::Create(L"Tex_Customer1Walk"))))
		return E_FAIL;
	for (int i = 1; i < 4; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer1/Wait0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer1Wait", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer1WaitTexture", Engine::CTexture::Create(L"Tex_Customer1Wait"))))
		return E_FAIL;
	for (int i = 1; i < 4; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer1/Eat0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer1Eat", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer1EatTexture", Engine::CTexture::Create(L"Tex_Customer1Eat"))))
		return E_FAIL;
	for (int i = 1; i < 3; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer1/Happy0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer1Happy", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer1HappyTexture", Engine::CTexture::Create(L"Tex_Customer1Happy"))))
		return E_FAIL;
	for (int i = 1; i < 3; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer1/Anger0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer1Anger", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer1AngerTexture", Engine::CTexture::Create(L"Tex_Customer1Anger"))))
		return E_FAIL;
	///////
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer1Menu", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/Customer/Customer1/Menu.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer1MenuTex", Engine::CTexture::Create(L"Tex_Customer1Menu"))))
		return E_FAIL;

	CAssetMgr::GetInstance()->AddAsset(L"Tex_AddMenu", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/AddMenuButton.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_AddMenuTex", Engine::CTexture::Create(L"Tex_AddMenu"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_SelectMenu", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/SelectFrame.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SelectTex", Engine::CTexture::Create(L"Tex_SelectMenu"))))
		return E_FAIL;

	//메뉴
	CAssetMgr::GetInstance()->AddAsset(L"Tex_MenuFrame", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/MenuBar.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_MenuFrameTex", Engine::CTexture::Create(L"Tex_MenuFrame"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_AddButton", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/AddButton.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_AddButtonTex", Engine::CTexture::Create(L"Tex_AddButton"))))
		return E_FAIL;


	CAssetMgr::GetInstance()->AddAsset(L"Tex_Recipe", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/recipe.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RecipeTex", Engine::CTexture::Create(L"Tex_Recipe"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_List", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/sushi_list.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SushiListTex", Engine::CTexture::Create(L"Tex_List"))))
		return E_FAIL;

	CAssetMgr::GetInstance()->AddAsset(L"Tex_DaveSushiIdle", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/Dave/dave_sushiIdle0.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DaveSushiIdleTex", Engine::CTexture::Create(L"Tex_DaveSushiIdle"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Sakura", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/sakura_stick.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SakuraTex", Engine::CTexture::Create(L"Tex_Sakura"))))
		return E_FAIL;
	//스시 아이콘
	CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiFrame", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/SushiBox.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SushiFrameTex", Engine::CTexture::Create(L"Tex_SushiFrame"))))
		return E_FAIL;

	CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiSelected", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Sushi_Focus.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SushiSelectedTex", Engine::CTexture::Create(L"Tex_SushiSelected"))))
		return E_FAIL;

	CAssetMgr::GetInstance()->AddAsset(L"Tex_Bluejong", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Sushi/Sushi_RedtoothedTriggerfish.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BluejongTex", Engine::CTexture::Create(L"Tex_Bluejong"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_ClownFish", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Sushi/Sushi_ClownFish.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ClownFishTex", Engine::CTexture::Create(L"Tex_ClownFish"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_TunaAkami", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Sushi/Sushi_YellowFin_Tuna_Akami.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TunaAkamiTex", Engine::CTexture::Create(L"Tex_TunaAkami"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_YellowTang", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Sushi/Sushi_YellowTang.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_YellowTangTex", Engine::CTexture::Create(L"Tex_YellowTang"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_YellowBack", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Sushi/Sushi_YellowbackFusilier.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_YellowBackTex", Engine::CTexture::Create(L"Tex_YellowBack"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->LoadAsset();
//#ifdef _DEBUG
//	Sleep(500);
//#endif // DEBUG
	m_bFinish = true;

	return S_OK;
}

HRESULT CTransition::Transition_SUSHI_TO_SHIP()
{
	m_sComment = L"Transition_SUSHI_TO_SHIP COMPLETE";
#ifdef _DEBUG
	Sleep(500);
#endif // DEBUG
	m_bFinish = true;

	return S_OK;
}

HRESULT CTransition::Common_SHIP_Load()
{
	for (int i = 1; i <= 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Ship/Dave_Idle_/Dave_Idle_0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_ShipDave_Idle", CAssetTexture::Create(s));
	}
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_ShipDave_Idle");

	for (int i = 1; i <= 6; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Ship/Dave_Walk_/Dave_Walk_0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_ShipDave_Walk", CAssetTexture::Create(s));
	}
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_ShipDave_Walk");

	for (int i = 1; i <= 12; ++i)
	{
		wstring s;
		if (i > 9)
		{
			s = L"../Bin/Resource/Texture/Ship/Dave_DiveReady/Dave_DiveReady" + ::to_wstring(i) + L".png";
		}
		else
		{
			s = L"../Bin/Resource/Texture/Ship/Dave_DiveReady/Dave_DiveReady0" + ::to_wstring(i) + L".png";
		}
		
		CAssetMgr::GetInstance()->AddAsset(L"Tex_ShipDave_DiveReady", CAssetTexture::Create(s));
	}
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_ShipDave_DiveReady");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_DiverBoxInvenTmp", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/DiverBoxTmp.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_DiverBoxInvenTmp");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_PhoneIcon", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/UI_MainMenu_Phone.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_PhoneIcon");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_PhoneBG", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/PhoneBG.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_PhoneBG");

	for (int i = 1; i <= 22; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Ship/PhoneApp_2/SmartPhoneApp" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_PhoneApp", CAssetTexture::Create(s));
	}
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_PhoneApp");

	for (int i = 1; i <= 22; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Ship/PhoneApp_2Alpha/SmartPhoneApp" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_PhoneAppAlpha", CAssetTexture::Create(s));
	}
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_PhoneAppAlpha");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_DiverBox", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/UI_PotionCraft_DiverBox/UI_PotionCraft_DiverBox.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_DiverBox");

	//Tex_PhoneIDiverBG
	CAssetMgr::GetInstance()->AddAsset(L"Tex_PhoneIDiverBG", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/iDiver/iDiverBG.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_PhoneIDiverBG");

	//UI_IDiverItem.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_UI_IDiverItem", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/iDiver/UI_IDiverItem.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_UI_IDiverItem");

	//UI_IDiverItem.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_UI_IDiverUpgrade", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/iDiver/IDiverUpgradeUI.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_UI_IDiverUpgrade");

	//UI_IDiverItem.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_BG", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/ShipBG/Tmp2.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_BG");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_FakeBG", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/ShipBG/Tmp3.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_FakeBG");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_UI_DiveBtn", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/DiveBtn/DiveBtn.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_UI_DiveBtn");

	for (int i = 1; i <= 38; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Ship/DiveBtn/Sheet/BtnMake-Sheet_" + to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_UI_DiveSpriteBtn", CAssetTexture::Create(s));
	}
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_UI_DiveSpriteBtn");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_UI_GoBtn", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/GoBoatBtn/UI_LobbyExit_Normal.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_UI_GoBtn");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_UI_GoBtnAlpha", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/GoBoatBtn/UI_LobbyExit_NormalAlpha.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_UI_GoBtnAlpha");

	//Space_Key_Dark_Symbol
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_UI_SpaceKey", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/Space_Key_Dark_Symbol.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_UI_SpaceKey");

	//MoneyUI
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_UI_Money", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/MoneyUI/MoneyUI.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_UI_Money");

	// DiverBoxDave
	for (int i = 1; i <= 8; ++i)
	{
		//02_Dave_01
		wstring s = L"../Bin/Resource/Texture/Ship/DiverBoxDave/02_Dave_0" + to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_DiverBoxDave", CAssetTexture::Create(s));
	}
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_DiverBoxDave");


	//InventoryBoxEdge
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_InventoryBoxEdge", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/DiverBoxInventory/InventoryBoxEdge2.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_InventoryBoxEdge");


	//Jaksal
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_InventoryUpperItem_Jaksal", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/DiverBoxInventory/upperItem/Jaksal.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_InventoryUpperItem_Jaksal");

	//Gun
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_InventoryUpperItem_Gun", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/DiverBoxInventory/upperItem/Gun.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_InventoryUpperItem_Gun");

	//Knief
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_InventoryUpperItem_Knief", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/DiverBoxInventory/upperItem/Knief.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_InventoryUpperItem_Knief");

	//Jusin
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_InventoryUpperItem_Jusin", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/DiverBoxInventory/upperItem/Jusin.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_InventoryUpperItem_Jusin");


	//ItemDescUI.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_Inventory_ItemDescUI", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/DiverBoxInventory/ItemDescUI.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_Inventory_ItemDescUI");

	//WoodPanel.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_InventoryUpperItem_WoodPanel", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/DiverBoxInventory/lowerItem/WoodPanel.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_InventoryUpperItem_WoodPanel");


	// Item_Sanso.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_IDiver_Item_Sanso", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/iDiver/Item_Sanso.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_IDiver_Item_Sanso");

	// Item_Clothes.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_IDiver_Item_Clothes", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/iDiver/Item_Clothes.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_IDiver_Item_Clothes");

	// Item_Clothes.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_IDiver_Item_Cage", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/iDiver/Item_Cage.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_IDiver_Item_Cage");

	// Item_Jaksal.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_IDiver_Item_Jaksal", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/iDiver/Item_Jaksal.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_IDiver_Item_Jaksal");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_IDiver_Edge", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/iDiver/Edge.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_IDiver_Edge");

	//UpgradeSuccess.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_IDiver_UpgradeSuccess", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/iDiver/UpgradeSuccess.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_IDiver_UpgradeSuccess");

	//UpgrddeBtn.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_IDiver_UpgradeBtn", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/iDiver/UpgrddeBtn.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_IDiver_UpgradeBtn");

	//UpgrddeBtnAlpha.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_IDiver_UpgradeBtnAlpha", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/iDiver/UpgrddeBtnAlpha.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_IDiver_UpgradeBtnAlpha");

	// 
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_Weapon_BG", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/Weapon/PhoneBg_Duff.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_Weapon_BG");

	//UI_WeaponCraft_Logo.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_Weapon_Logo", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/Weapon/UI_WeaponCraft_Logo.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_Weapon_Logo");

	//Close.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_Weapon_Close", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/Weapon/Close.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_Weapon_Close");

	//UI_Area.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_Weapon_UI_Area", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/Weapon/UI_Area.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_Weapon_UI_Area");

	//UI_CreateBtn.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_Weapon_UI_CreateBtn", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/Weapon/UI_CreateBtn.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_Weapon_UI_CreateBtn");

	//Edge.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_Weapon_UI_Edge", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/Weapon/Edge.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_Weapon_UI_Edge");

	//GusikRifle.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_Weapon_GukikRifle", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/Weapon/GusikRifle.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_Weapon_GukikRifle");

	//NewWeapon.png
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_Weapon_UI_NewWeapon", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/Weapon/NewWeapon.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_Weapon_UI_NewWeapon");

	return S_OK;
}

HRESULT CTransition::Common_SHIP_Unload()
{
	return S_OK;
}

HRESULT CTransition::Common_Logo_Env_Load()
{
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Logo_BG", CAssetTexture::Create(L"../Bin/Resource/Texture/Logo/DR_Illust.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Logo_BG");
	
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Logo_Title", CAssetTexture::Create(L"../Bin/Resource/Texture/Logo/DaveTheDiver_TitleLogo.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Logo_Title");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_Logo_Black1pxAlpha", CAssetTexture::Create(L"../Bin/Resource/Texture/Logo/Black1pxAlpha.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Logo_Black1pxAlpha");
	return S_OK;
}

HRESULT CTransition::Common_Logo_Env_Unload()
{


	return S_OK;
}

HRESULT CTransition::Ready_Scene()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	D3DXMATRIX matView, matProj;
	D3DXVECTOR3 vEye(0.0f, 0.0f, -2.0f);
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, (float)WINCX / (float)WINCY, 0.1f, 1000.0f);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);


	InitializeCriticalSection(&m_Crt);
	m_hThread = (HANDLE)_beginthreadex(NULL, // 보안 속성(핸들의 상속 여부, NULL인 경우 상속에서 제외)
		0,  // 디폴트 스탯 사이즈(1 바이트)
		Thread_Main, // 구동할 쓰레드 함수
		this,          // 3번 매개 변수 함수를 통해 가공할 데이터 주소
		0,             // 쓰레드 생성 및 실행을 조정하기 위한 옵션
		NULL);         // 쓰레드 ID


	if (FAILED(Ready_Environment_Layer(L"0_Environment_Layer")))
		return E_FAIL;


	m_bFadeEnd = false;
	if (m_eSrcScene == SCENE_INIT)
	{
		this->m_bFadeEnd = true;
	}
	else
	{
		AddFadeIn(this, [=]() {
			this->m_bFadeEnd = true;
			});
	}
	

	
	return S_OK;
}

_int CTransition::Update_Scene(const _float& fTimeDelta)
{
	CScene::Update_Scene(fTimeDelta);

	if (m_bFadeEnd && m_bFinish)
	{
		m_bFadeEnd = false;
		if (m_eDstScene == SCENE_LOGO)
		{
			CManagement::GetInstance()->Set_Scene(CLogo::Create());
		}
		else if (m_eDstScene == SCENE_SHIP)
		{
			AddFadeOut(this, [=]() { 
				auto pScene = CShip::Create();
				AddFadeIn(pScene);
				CManagement::GetInstance()->Set_Scene(pScene);
				});
		}
		else if (m_eDstScene == SCENE_SUSHI)
		{
			CManagement::GetInstance()->Set_Scene(CSushi::Create());
		}
		else if (m_eDstScene == SCENE_DIVE)
		{
			CManagement::GetInstance()->Set_Scene(CDive::Create());
		}
	}
	


	

	return 0;
}

void CTransition::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

HRESULT			CTransition::Ready_Environment_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CTransitionBG* pTransitionBG = CTransitionBG::Create(0.f, 0.f);
	if (nullptr == pTransitionBG)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"TransitionBG", pTransitionBG)))
		return E_FAIL;

	if (m_eDstScene == SCENE_SHIP)
	{
		CTransitionImg* pTransitionImg = CTransitionImg::Create(0.f, 0.1f);
		pTransitionImg->Set_AssetName(L"Tex_Transition_BG_Lobby");

		pTransitionImg->Set_CustomScaleX(0.1f);
		pTransitionImg->Set_CustomScaleY(0.1f);

		pTransitionImg->Ready_AfterCreate();


		if (nullptr == pTransitionImg)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"TransitionImg", pTransitionImg)))
			return E_FAIL;
		
	}

	{
		CTransitionFace* pFace = CTransitionFace::Create(-0.1f, -0.5f);
		pFace->Set_Delay(0.1f);
		if (nullptr == pFace)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"TransitionFace", pFace)))
			return E_FAIL;
	}
	{
		CTransitionFace* pFace = CTransitionFace::Create(0.f, -0.5f);
		pFace->Set_Delay(0.2f);
		if (nullptr == pFace)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"TransitionFace", pFace)))
			return E_FAIL;
	}
	{
		CTransitionFace* pFace = CTransitionFace::Create(0.1f, -0.5f);
		pFace->Set_Delay(0.3f);
		if (nullptr == pFace)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"TransitionFace", pFace)))
			return E_FAIL;
	} 

	{
		CTransitionTxt* pTxt = CTransitionTxt::Create(0.f, 0.6f);
		pTxt->Set_Txt(L"이것은 제목이여");
		pTxt->Set_Opt(DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
		if (nullptr == pTxt)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"TransitionTipTxt", pTxt)))
			return E_FAIL;
	}



	{
		CTransitionTxt* pTxt = CTransitionTxt::Create(0.f, -0.6f);
		pTxt->Set_Txt(L"ASDfASDFASDFASDFASDFASDFASDFASDFASDFASDFASDFF");
		pTxt->Set_Opt(DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
		if (nullptr == pTxt)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"TransitionTipTxt", pTxt)))
			return E_FAIL;

		CTransitionImg* pTransitionTip = CTransitionImg::Create(-0.6f, -0.6f);
		pTransitionTip->Set_AssetName(L"Tex_Transition_Tip");
		pTransitionTip->Set_CustomScaleX(0.2f);
		pTransitionTip->Set_CustomScaleY(0.2f);
		pTransitionTip->Ready_AfterCreate();
		if (nullptr == pTransitionTip)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"TransitionTip", pTransitionTip)))
			return E_FAIL;
	}

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });
	return S_OK;
}

void CTransition::Render_Scene()
{
	_vec2	vPos{ 0.f, 0.f };
	CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Default");
	pDefFont->Render_Font(m_sComment, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

CTransition* CTransition::Create(SCENE_ID eSrcScene, SCENE_ID eDstScene)
{
	CTransition* pTransition = new CTransition{ eSrcScene , eDstScene };

	if (FAILED(pTransition->Ready_Scene()))
	{
		wstring str = L"pTransition Create Failed. SrcScene: " + to_wstring(pTransition->Get_SrcScene()) + L"DstScene: " + to_wstring(pTransition->Get_DstScene());
		MSG_BOX_STR(str.c_str());
		Safe_Release(pTransition);
		return nullptr;
	}

	return pTransition;
}

void CTransition::Free()
{
	CScene::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Crt);

}

void CTransition::FadedTransition(SCENE_ID eSrcScene, SCENE_ID eDstScene)
{
	auto p = CManagement::GetInstance()->Get_Scene()->Get_Layer();
	if (auto pLayers = CManagement::GetInstance()->Get_Scene()->Get_Layer())
	{
		if (!pLayers->empty())
		{
			CTransitionFade* pFade = CTransitionFade::Create(0.f, 0.f, CTransitionFade::FADE_OUT);
			pFade->Set_OnEnd([=]() {
				CManagement::GetInstance()->Set_Scene(CTransition::Create(eSrcScene, eDstScene));
				});
			for (auto& p : *pLayers)
			{
				p.second->Add_GameObject(L"99_FADE", pFade);
				break;
			}
		}
	}
}

void CTransition::AddFadeIn(CScene* pScene, function<void()> funcOnEnd )
{
	if (auto pLayers = pScene->Get_Layer())
	{
		if (!pLayers->empty())
		{
			CTransitionFade* pFade = CTransitionFade::Create(0.f, 0.f, CTransitionFade::FADE_IN);
			pFade->Set_OnEnd(funcOnEnd);
			for (auto& p : *pLayers)
			{
				p.second->Add_GameObject(L"99_FADE", pFade);
				break;
			}
		}
	}
}

void CTransition::AddFadeOut(CScene* pScene, function<void()> funcOnEnd)
{
	if (auto pLayers = pScene->Get_Layer())
	{
		if (!pLayers->empty())
		{
			CTransitionFade* pFade = CTransitionFade::Create(0.f, 0.f, CTransitionFade::FADE_OUT);
			pFade->Set_OnEnd(funcOnEnd);
			for (auto& p : *pLayers)
			{
				p.second->Add_GameObject(L"99_FADE", pFade);
				break;
			}
		}
	}
}

unsigned int CTransition::Thread_Main(void* pArg)
{
	CTransition* pTransition = reinterpret_cast<CTransition*>(pArg);

	int iFlag(0);

	EnterCriticalSection(pTransition->Get_Crt());

	SCENE_ID eSrc = pTransition->Get_SrcScene();
	SCENE_ID eDst = pTransition->Get_DstScene();

	if (eSrc == SCENE_INIT)
	{
		if (eDst == SCENE_LOGO)
		{
			pTransition->Transition_INIT_TO_LOGO();
		}
	}
	else if (eSrc == SCENE_LOGO)
	{
		if (eDst == SCENE_SHIP)
		{
			pTransition->Transition_LOGO_TO_SHIP();
		}
	}
	else if (eSrc == SCENE_SHIP)
	{
		if (eDst == SCENE_DIVE)
		{
			pTransition->Transition_SHIP_TO_DIVE();
		}
		else if (eDst == SCENE_SUSHI)
		{
			pTransition->Transition_SHIP_TO_SUSHI();
		}
		else if (eDst == SCENE_LOGO)
		{
			pTransition->Transition_SHIP_TO_LOGO();
		}
	}
	else if (eSrc == SCENE_DIVE)
	{
		if (eDst == SCENE_SHIP)
		{
			pTransition->Transition_DIVE_TO_SHIP();
		}
	}
	else if (eSrc == SCENE_SUSHI)
	{
		if (eDst == SCENE_SHIP)
		{
			pTransition->Transition_SUSHI_TO_SHIP();
		}
	}


	LeaveCriticalSection(pTransition->Get_Crt());

	//_endthreadex(0);

	return iFlag;       // 0 리턴 시, _endthreadex가 자동 호출
}