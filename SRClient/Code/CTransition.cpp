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
#include "CAttackReadyArmTex.h"
#include "CAssetTexture.h"
#include "CTargetArrowTex.h"

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

	for (int i = 0; i < 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Dive_Player/Attack/AttackReady0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerAttackReady", CAssetTexture::Create(s.c_str()));
	}
	CAssetMgr::GetInstance()->AddAsset(L"Tex_AttackReadyArm", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/Attack/AttackReadyArms.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_TargetCurveStart", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/Attack_Effect/Target_CurveStart.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_TargetArrow", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/Attack_Effect/Target_Arrow.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_HarpoonGun", CAssetTexture::Create(L"../Bin/Resource/Texture/Weapon/HarpoonGun.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_HarpoonProjectile", CAssetTexture::Create(L"../Bin/Resource/Texture/Weapon/HarpoonProjectile.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerAttackFire", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/AttackFire/AttackFire01.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerAttackFail", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/AttackFail/AttackFail01.png"));

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
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerAttackReadyTexture", Engine::CTexture::Create(L"Tex_DivePlayerAttackReady"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerAttackFireTexture", Engine::CTexture::Create(L"Tex_DivePlayerAttackFire"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerAttackFightTexture", Engine::CTexture::Create(L"Tex_DivePlayerAttackFight"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerAttackFailTexture", Engine::CTexture::Create(L"Tex_DivePlayerAttackFail"))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_AttackReadyArmTexture", Engine::CTexture::Create(L"Tex_AttackReadyArm"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HarpoonTexture", Engine::CTexture::Create(L"Tex_HarpoonGun"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HarpoonProjectileTexture", Engine::CTexture::Create(L"Tex_HarpoonProjectile"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_AttackReadyArmBuffer", Engine::CAttackReadyArmTex::Create())))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TargetArrowBuffer", Engine::CTargetArrowTex::Create())))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TargetCurveStartTexture", Engine::CTexture::Create(L"Tex_TargetCurveStart"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TargetArrowGunTexture", Engine::CTexture::Create(L"Tex_TargetArrow"))))
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
	///////
	CAssetMgr::GetInstance()->AddAsset(L"Tex_DaveSushiIdle", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/Dave/dave_sushiIdle0.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DaveSushiIdleTex", Engine::CTexture::Create(L"Tex_DaveSushiIdle"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Sakura", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/BackGround/sakura_stick.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SakuraTex", Engine::CTexture::Create(L"Tex_Sakura"))))
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

	CAssetMgr::GetInstance()->AddAsset(L"Tex_DiverBoxTmp", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/DiverBoxTmp.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_DiverBoxTmp");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_PhoneIcon", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/UI_MainMenu_Phone.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_PhoneIcon");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_PhoneBG", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/PhoneBG.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_PhoneBG");

	for (int i = 1; i <= 22; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Ship/PhoneApp/SmartPhoneApp" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_PhoneApp", CAssetTexture::Create(s));
	}
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_PhoneApp");

	for (int i = 1; i <= 22; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Ship/PhoneAppAlpha/SmartPhoneAppAlpha" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_PhoneAppAlpha", CAssetTexture::Create(s));
	}
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_PhoneAppAlpha");
	return S_OK;
}

HRESULT CTransition::Common_SHIP_Unload()
{
	return S_OK;
}

HRESULT CTransition::Ready_Scene()
{
	InitializeCriticalSection(&m_Crt);
	m_hThread = (HANDLE)_beginthreadex(NULL, // 보안 속성(핸들의 상속 여부, NULL인 경우 상속에서 제외)
		0,  // 디폴트 스탯 사이즈(1 바이트)
		Thread_Main, // 구동할 쓰레드 함수
		this,          // 3번 매개 변수 함수를 통해 가공할 데이터 주소
		0,             // 쓰레드 생성 및 실행을 조정하기 위한 옵션
		NULL);         // 쓰레드 ID
	return S_OK;
}

_int CTransition::Update_Scene(const _float& fTimeDelta)
{
	if (!m_bFinish) return 0;
	
	if (m_eDstScene == SCENE_LOGO)
	{
		CManagement::GetInstance()->Set_Scene(CLogo::Create());
	}
	else if (m_eDstScene == SCENE_SHIP)
	{
		CManagement::GetInstance()->Set_Scene(CShip::Create());
	}
	else if (m_eDstScene == SCENE_SUSHI)
	{
		CManagement::GetInstance()->Set_Scene(CSushi::Create());
	}
	else if (m_eDstScene == SCENE_DIVE)
	{
		CManagement::GetInstance()->Set_Scene(CDive::Create());
	}

	return 0;
}

void CTransition::LateUpdate_Scene(const _float& fTimeDelta)
{
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
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Crt);
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