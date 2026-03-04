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
#include "CAssetFmodSound.h"
#include "CAssetSpine.h"
#include "CParticleMgr.h"
#include "CLeah.h"

 bool CTransition::s_LogoAssetLoaded = false;
 bool CTransition::s_ShipAssetLoaded = false;
 bool CTransition::s_DiveAssetLoaded = false;
 bool CTransition::s_SushiAssetLoaded = false;

CTransition::CTransition(SCENE_ID eSrcScene, SCENE_ID eDstScene)
	: m_eSrcScene(eSrcScene)
	, m_eDstScene(eDstScene)
	, m_bFinish(false)
	, m_Crt({})
	, m_hThread(nullptr)
	, m_reserveTransfer({})
	, m_bLoadingStart(false)
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
	
	CAssetMgr::GetInstance()->AddAsset(L"Json_Map", CAssetJson::Create(L"../Bin/Resource/Data/Map1.json"));
	CAssetMgr::GetInstance()->LoadAsset(L"Json_Map");



	CAssetMgr::GetInstance()->AddAsset(L"Terrian1_Collision", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian1_Collision.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"Terrian1_Collision");

	CProtoMgr::GetInstance()->Ready_Prototype(L"Terrian1_Collision", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian1_Collision")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian1_Collision")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian1_Collision")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian1_Collision")->Get_Index()));

	CAssetMgr::GetInstance()->AddAsset(L"GLB_Terrian1", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian1.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"GLB_Terrian1");

	CProtoMgr::GetInstance()->Ready_Prototype(L"GLB_Terrian1", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian1")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian1")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian1")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian1")->Get_Index()));


	CAssetMgr::GetInstance()->AddAsset(L"BackGround_GLB_File", CAssetGlb::Create(L"../Bin/Resource/Glb/BackGround.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"BackGround_GLB_File");

	CProtoMgr::GetInstance()->Ready_Prototype(L"BackGround_GLB_File", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"BackGround_GLB_File")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"BackGround_GLB_File")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"BackGround_GLB_File")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"BackGround_GLB_File")->Get_Index()));


	// ---------------------------------------------------------------------------------------------------------------------------------
	CAssetMgr::GetInstance()->AddAsset(L"Terrian2_Collision", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian2_Collision.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"Terrian2_Collision");

	CProtoMgr::GetInstance()->Ready_Prototype(L"Terrian2_Collision", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian2_Collision")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian2_Collision")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian2_Collision")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian2_Collision")->Get_Index()));

	CAssetMgr::GetInstance()->AddAsset(L"GLB_Terrian2", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian2.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"GLB_Terrian2");

	CProtoMgr::GetInstance()->Ready_Prototype(L"GLB_Terrian2", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian2")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian2")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian2")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian2")->Get_Index()));	
	
	
	// ---------------------------------------------------------------------------------------------------------------------------------
	CAssetMgr::GetInstance()->AddAsset(L"Terrian3_Collision", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian3_Collision.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"Terrian3_Collision");

	CProtoMgr::GetInstance()->Ready_Prototype(L"Terrian3_Collision", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian3_Collision")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian3_Collision")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian3_Collision")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian3_Collision")->Get_Index()));

	CAssetMgr::GetInstance()->AddAsset(L"GLB_Terrian3", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian3.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"GLB_Terrian3");

	CProtoMgr::GetInstance()->Ready_Prototype(L"GLB_Terrian3", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian3")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian3")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian3")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian3")->Get_Index()));

	// ---------------------------------------------------------------------------------------------------------------------------------
	CAssetMgr::GetInstance()->AddAsset(L"Terrian4_Collision", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian4_Collision.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"Terrian4_Collision");

	CProtoMgr::GetInstance()->Ready_Prototype(L"Terrian4_Collision", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian4_Collision")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian4_Collision")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian4_Collision")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian4_Collision")->Get_Index()));

	CAssetMgr::GetInstance()->AddAsset(L"GLB_Terrian4", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian4.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"GLB_Terrian4");

	CProtoMgr::GetInstance()->Ready_Prototype(L"GLB_Terrian4", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian4")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian4")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian4")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian4")->Get_Index()));

	// ---------------------------------------------------------------------------------------------------------------------------------
	CAssetMgr::GetInstance()->AddAsset(L"Terrian5_Collision", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian5_Collision.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"Terrian5_Collision");

	CProtoMgr::GetInstance()->Ready_Prototype(L"Terrian5_Collision", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian5_Collision")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian5_Collision")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian5_Collision")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian5_Collision")->Get_Index()));

	CAssetMgr::GetInstance()->AddAsset(L"GLB_Terrian5", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian5.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"GLB_Terrian5");

	CProtoMgr::GetInstance()->Ready_Prototype(L"GLB_Terrian5", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian5")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian5")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian5")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian5")->Get_Index()));

	// ---------------------------------------------------------------------------------------------------------------------------------
	CAssetMgr::GetInstance()->AddAsset(L"Terrian6_Collision", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian6_Collision.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"Terrian6_Collision");

	CProtoMgr::GetInstance()->Ready_Prototype(L"Terrian6_Collision", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian6_Collision")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian6_Collision")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian6_Collision")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian6_Collision")->Get_Index()));

	CAssetMgr::GetInstance()->AddAsset(L"GLB_Terrian6", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian6.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"GLB_Terrian6");

	CProtoMgr::GetInstance()->Ready_Prototype(L"GLB_Terrian6", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian6")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian6")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian6")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian6")->Get_Index()));

	// ---------------------------------------------------------------------------------------------------------------------------------
	CAssetMgr::GetInstance()->AddAsset(L"Terrian7_Collision", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian7_Collision.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"Terrian7_Collision");

	CProtoMgr::GetInstance()->Ready_Prototype(L"Terrian7_Collision", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian7_Collision")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian7_Collision")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian7_Collision")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian7_Collision")->Get_Index()));

	CAssetMgr::GetInstance()->AddAsset(L"GLB_Terrian7", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian7.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"GLB_Terrian7");

	CProtoMgr::GetInstance()->Ready_Prototype(L"GLB_Terrian7", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian7")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian7")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian7")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian7")->Get_Index()));

	// ---------------------------------------------------------------------------------------------------------------------------------
	CAssetMgr::GetInstance()->AddAsset(L"Terrian8_Collision", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian8_Collision.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"Terrian8_Collision");

	CProtoMgr::GetInstance()->Ready_Prototype(L"Terrian8_Collision", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian8_Collision")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian8_Collision")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian8_Collision")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian8_Collision")->Get_Index()));

	CAssetMgr::GetInstance()->AddAsset(L"GLB_Terrian8", CAssetGlb::Create(L"../Bin/Resource/Glb/Terrian8.glb"));
	CAssetMgr::GetInstance()->LoadAsset(L"GLB_Terrian8");

	CProtoMgr::GetInstance()->Ready_Prototype(L"GLB_Terrian8", CGlbTex::Create(
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian8")->Get_VtxCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian8")->Get_TriCnt(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian8")->Get_vertices(),
		CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"GLB_Terrian8")->Get_Index()));


	CAssetMgr::GetInstance()->AddAsset(L"BackGround", CAssetTexture::Create(L"../Bin/Resource/Texture/Map/BackGround1.png"));


	

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BackGroundSea", CTexture::Create(L"BackGround"))))
		return E_FAIL;



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
	//Common_Logo_Env_Unload();

	if (FAILED(Common_SHIP_Load()))
	{
		return E_FAIL;
	}


	//stockMarket
	{
		CAssetMgr::GetInstance()->AddAsset(L"Tex_stockMarket", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/stockMarket/stockMarket1.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_stockMarket");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_stockMarket1", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/stockMarket/stockMarket2.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_stockMarket1");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_stockMarket2", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/stockMarket/stockMarket3.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_stockMarket2");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_stockMarket3", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/stockMarket/stockMarket4.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_stockMarket3");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_stockMarket4", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/stockMarket/stockMarket5.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_stockMarket4");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_stockMarket5", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/stockMarket/stockMarket6.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_stockMarket5");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_stockMarket6", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/stockMarket/stockMarket7.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_stockMarket6");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_stockMarket7", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/stockMarket/stockMarket8.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_stockMarket7");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_stockMarket8", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/stockMarket/stockMarket9.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_stockMarket8");
		
		CAssetMgr::GetInstance()->AddAsset(L"Tex_stockMarket9", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/stockMarket/stockMarket10.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_stockMarket9");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_stockMarket10", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/stockMarket/stockMarket11.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_stockMarket10");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_stockMarket11", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/stockMarket/stockMarket12.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_stockMarket11");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_stockMarket12", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/stockMarket/stockMarket13.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_stockMarket12");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_stockMarket13", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/stockMarket/stockMarket14.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_stockMarket13");
	}

	// items
	{
		// Item_Bone.png
		CAssetMgr::GetInstance()->AddAsset(L"Tex_UIITem_Bone", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/items/Item_Bone.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_UIITem_Bone");

		// Item_DeepseaCoral.png
		CAssetMgr::GetInstance()->AddAsset(L"Tex_UIITem_DeepseaCoral", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/items/Item_DeepseaCoral.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_UIITem_DeepseaCoral");

		// Item_Fragment.png
		CAssetMgr::GetInstance()->AddAsset(L"Tex_UIITem_Fragment", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/items/Item_Fragment.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_UIITem_Fragment");

		// Item_JadeThurible.png
		CAssetMgr::GetInstance()->AddAsset(L"Tex_UIITem_JadeThurible", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/items/Item_JadeThurible.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_UIITem_JadeThurible");

		// Item_Rope.png
		CAssetMgr::GetInstance()->AddAsset(L"Tex_UIITem_Rope", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/items/Item_Rope.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_UIITem_Rope");

		// Item_RubyRing.png
		CAssetMgr::GetInstance()->AddAsset(L"Tex_UIITem_RubyRing", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/items/Item_RubyRing.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_UIITem_RubyRing");

		// Item_Umbellula.png
		CAssetMgr::GetInstance()->AddAsset(L"Tex_UIITem_Umbellula", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/items/Item_Umbellula.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_UIITem_Umbellula");

		// Item_watch.png
		CAssetMgr::GetInstance()->AddAsset(L"Tex_UIITem_Watch", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/items/Item_watch.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_UIITem_Watch");

		// Item_Wood.png
		CAssetMgr::GetInstance()->AddAsset(L"Tex_UIITem_Wood", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/items/Item_Wood.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_UIITem_Wood");

		// Item_woodplate.png
		CAssetMgr::GetInstance()->AddAsset(L"Tex_UIITem_Woodplate", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/items/Item_woodplate.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_UIITem_Woodplate");
	}

	// dive get weapon ui
	{
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DiveGetWeaponUI", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/GetWeaponUI.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_DiveGetWeaponUI");
	}

	// dive item desc ui
	{
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DiveItemDescUI", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/DiveItemDescUI.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_DiveItemDescUI");
	}


	// Sushi Thumbnail
	{
		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_Asian_Sheepshead", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_Asian_Sheepshead.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_Asian_Sheepshead");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_Blobfish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_Blobfish.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_Blobfish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_Bluetang", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_Bluetang.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_Bluetang");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_ClownFish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_ClownFish.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_ClownFish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_Devil_ScorpionFish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_Devil_ScorpionFish.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_Devil_ScorpionFish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_DuskyGrouper", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_DuskyGrouper.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_DuskyGrouper");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_GiantTrevally", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_GiantTrevally.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_GiantTrevally");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_GreatSpiderCrab", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_GreatSpiderCrab.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_GreatSpiderCrab");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_GreenHumpheadParrotfish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_GreenHumpheadParrotfish.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_GreenHumpheadParrotfish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_GreyTriggerfish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_GreyTriggerfish.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_GreyTriggerfish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_Humboldt_Squid", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_Humboldt_Squid.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_Humboldt_Squid");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_JuvenileCircularBatFishFry", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_JuvenileCircularBatFishFry.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_JuvenileCircularBatFishFry");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_RedLionfish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_RedLionfish.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_RedLionfish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_SmallspottedDart", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_SmallspottedDart.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_SmallspottedDart");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_SpearSquid", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_SpearSquid.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_SpearSquid");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_TitanTriggerfish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_TitanTriggerfish.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_TitanTriggerfish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_YellowbackFusilier", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_YellowbackFusilier.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_YellowbackFusilier");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiThumb_YellowTang", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedSushiThumbnail/Sushi_YellowTang.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_SushiThumb_YellowTang");
	}

	// Fish Thumbnails
	{
		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Asian_Sheepshead", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Asian_Sheepshead_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Asian_Sheepshead");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Blobfish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Blobfish_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Blobfish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Bluetang", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Bluetang_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Bluetang");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_ClownFish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/ClownFish_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_ClownFish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Devil_ScorpionFish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Devil_ScorpionFish_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Devil_ScorpionFish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Dusky_Grouper", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Dusky_Grouper_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Dusky_Grouper");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Giant_Trevally", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Giant_Trevally_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Giant_Trevally");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Great_Spider_Crab", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Great_Spider_Crab_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Great_Spider_Crab");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Green_Humphead_Parrotfish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Green_Humphead_Parrotfish_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Green_Humphead_Parrotfish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Grey_Triggerfish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Grey_Triggerfish_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Grey_Triggerfish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Humboldt_Squid", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Humboldt_Squid_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Humboldt_Squid");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Juvenile_Circular_BatFish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Juvenile_Circular_BatFish_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Juvenile_Circular_BatFish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Leafy_Seadragon", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Leafy_Seadragon_Ingame.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Leafy_Seadragon");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Red_Lionfish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Red_Lionfish_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Red_Lionfish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Smallspotted_dart", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Smallspotted_dart_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Smallspotted_dart");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_SpearSquid", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/SpearSquid_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_SpearSquid");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Titan_Triggerfish", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Titan_Triggerfish_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Titan_Triggerfish");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Yellow_Tang", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Yellow_Tang_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Yellow_Tang");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishThumb_Yellowback_Fusilier", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/usedThumbnail/Yellowback_Fusilier_Thumbnail.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishThumb_Yellowback_Fusilier");
	}

	// DaveConversation
	{
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DaveConversation", CAssetTexture::Create(L"../Bin/Resource/Texture/Conversation/DaveConversation.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_DaveConversation");
	}

	// BackToShip
	{
		CAssetMgr::GetInstance()->AddAsset(L"Tex_BackToShipUI", CAssetTexture::Create(L"../Bin/Resource/Texture/BackToShip/BackToShip.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_BackToShipUI");
	}

	// DiveResultUI
	{
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DiveResultUI", CAssetTexture::Create(L"../Bin/Resource/Texture/DiveResultInShip/DiveResult.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_DiveResultUI");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_ToSushiUI", CAssetTexture::Create(L"../Bin/Resource/Texture/DiveResultInShip/ToSushi.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_ToSushiUI");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_ToSushiUIItem", CAssetTexture::Create(L"../Bin/Resource/Texture/DiveResultInShip/ToSushiItem.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_ToSushiUIItem");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_GoToSushiUI", CAssetTexture::Create(L"../Bin/Resource/Texture/DiveResultInShip/GoToSushi.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_GoToSushiUI");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_Bancho_Good", CAssetTexture::Create(L"../Bin/Resource/Texture/DiveResultInShip/Bancho_Good.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_Bancho_Good");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_DiveResult_Best", CAssetTexture::Create(L"../Bin/Resource/Texture/DiveResultInShip/Best.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_DiveResult_Best");
	}

	// 스파인
	{
		CAssetMgr::GetInstance()->AddAsset(L"Spine_Fish_BlueTang", CAssetSpine::Create(L"../Bin/Resource/Spine/BlueTang/BlueTang"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_Fish_BlueTang");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_Clownfish", CAssetSpine::Create(L"../Bin/Resource/Spine/Clownfish/Clownfish"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_Clownfish");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_SmallspottedDart", CAssetSpine::Create(L"../Bin/Resource/Spine/SmallspottedDart/Smallspotted_dart"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_SmallspottedDart");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_YellowbackFusilier", CAssetSpine::Create(L"../Bin/Resource/Spine/YellowbackFusilier/Yellowback_Fusilier"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_YellowbackFusilier");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_YellowTang", CAssetSpine::Create(L"../Bin/Resource/Spine/YellowTang/Yellow_Tang"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_YellowTang");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_TitanTriggerfish", CAssetSpine::Create(L"../Bin/Resource/Spine/TitanTriggerfish/Titan_Triggerfish"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_TitanTriggerfish");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_Blobfish", CAssetSpine::Create(L"../Bin/Resource/Spine/Blobfish/Blobfish"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_Blobfish");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_HumboldtSquid", CAssetSpine::Create(L"../Bin/Resource/Spine/HumboldtSquid/Humboldt_Squid"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_HumboldtSquid");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_GreatSpiderCrab", CAssetSpine::Create(L"../Bin/Resource/Spine/GreatSpiderCrab/Great_Spider_Crab"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_GreatSpiderCrab");

		// 오류남
		//CAssetMgr::GetInstance()->AddAsset(L"Spine_DuskyGrouper", CAssetSpine::Create(L"../Bin/Resource/Spine/DuskyGrouper/Dusky_Grouper"));
		//CAssetMgr::GetInstance()->LoadAsset(L"Spine_DuskyGrouper");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_AsianSheepshead", CAssetSpine::Create(L"../Bin/Resource/Spine/AsianSheepshead/Asian_Sheepshead"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_AsianSheepshead");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_GiantTrevally", CAssetSpine::Create(L"../Bin/Resource/Spine/GiantTrevally/Giant_Trevally"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_GiantTrevally");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_RedLionfish", CAssetSpine::Create(L"../Bin/Resource/Spine/RedLionfish/Red_Lionfish"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_RedLionfish");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_SpearSquid", CAssetSpine::Create(L"../Bin/Resource/Spine/SpearSquid/SpearSquid"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_SpearSquid");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_JuvenileCircularBatFish", CAssetSpine::Create(L"../Bin/Resource/Spine/JuvenileCircularBatFish/Juvenile_Circular_BatFish"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_JuvenileCircularBatFish");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_LeafySeadragonIngame", CAssetSpine::Create(L"../Bin/Resource/Spine/LeafySeadragonIngame/Leafy_Seadragon_Ingame"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_LeafySeadragonIngame");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_GreenHumpheadParrotfish", CAssetSpine::Create(L"../Bin/Resource/Spine/GreenHumpheadParrotfish/Green_Humphead_Parrotfish"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_GreenHumpheadParrotfish");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_DevilScorpionFish", CAssetSpine::Create(L"../Bin/Resource/Spine/DevilScorpionFish/Devil_ScorpionFish"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_DevilScorpionFish");

		CAssetMgr::GetInstance()->AddAsset(L"Spine_GreyTriggerfish", CAssetSpine::Create(L"../Bin/Resource/Spine/GreyTriggerfish/Grey_Triggerfish"));
		CAssetMgr::GetInstance()->LoadAsset(L"Spine_GreyTriggerfish");
	}

	// Fish 유아이
	{
		CAssetMgr::GetInstance()->AddAsset(L"Tex_GetItemUI", CAssetTexture::Create(L"../Bin/Resource/Texture/FishUI/GetItemUI.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_GetItemUI");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_GetItemUIStar", CAssetTexture::Create(L"../Bin/Resource/Texture/FishUI/GetItemUIStar.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_GetItemUIStar");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_HoldFishUI", CAssetTexture::Create(L"../Bin/Resource/Texture/FishUI/HoldFishUI.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_HoldFishUI");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_HoldFishItemArea", CAssetTexture::Create(L"../Bin/Resource/Texture/FishUI/HoldFishItemArea.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_HoldFishItemArea");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_HoldFishItemAreaEdge", CAssetTexture::Create(L"../Bin/Resource/Texture/FishUI/HoldFishItemAreaEdge.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_HoldFishItemAreaEdge");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_HoldFishUIDropPanel", CAssetTexture::Create(L"../Bin/Resource/Texture/FishUI/HoldFishUIDropPanel.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_HoldFishUIDropPanel");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_AggresiveFishPoint", CAssetTexture::Create(L"../Bin/Resource/Texture/FishUI/AggresiveFishPoint.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_AggresiveFishPoint");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_AgressiveMark", CAssetTexture::Create(L"../Bin/Resource/Texture/FishUI/AgressiveMark.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_AgressiveMark");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_FishUIJaksalChock", CAssetTexture::Create(L"../Bin/Resource/Texture/FishUI/JaksalChock.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_FishUIJaksalChock");

		CAssetMgr::GetInstance()->AddAsset(L"Tex_HoldFishUIMask", CAssetTexture::Create(L"../Bin/Resource/Texture/FishUI/HoldFishUIMask.png"));
		CAssetMgr::GetInstance()->LoadAsset(L"Tex_HoldFishUIMask");
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
	//Common_SHIP_Unload();

	Common_Logo_Env_Load();


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
	for (int i = 0; i < 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Dive_Player/Hit/Hit0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerHit", CAssetTexture::Create(s.c_str()));
	}
	for (int i = 0; i < 23; ++i)
	{
		wstring s;
		if (i + 1 < 10)
			s = L"../Bin/Resource/Texture/Dive_Player/Die/Die0" + ::to_wstring(i + 1) + L".png";
		else
			s = L"../Bin/Resource/Texture/Dive_Player/Die/Die" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerDie", CAssetTexture::Create(s.c_str()));
	}

	CAssetMgr::GetInstance()->AddAsset(L"Tex_AttackReadyArm", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/Attack/AttackReadyArms.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_TargetCurveStart", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/Attack_Effect/Target_CurveStart.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_TargetArrow", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/Attack_Effect/Target_Arrow.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_HarpoonGun", CAssetTexture::Create(L"../Bin/Resource/Texture/Weapon/HarpoonGun.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_HarpoonProjectile", CAssetTexture::Create(L"../Bin/Resource/Texture/Weapon/HarpoonProjectile.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_BasicRifle", CAssetTexture::Create(L"../Bin/Resource/Texture/Weapon/BasicRifle.png"));
	CAssetMgr::GetInstance()->AddAsset(L"TripleAxel", CAssetTexture::Create(L"../Bin/Resource/Texture/Weapon/Triple.png"));
	CAssetMgr::GetInstance()->AddAsset(L"PentaAxel", CAssetTexture::Create(L"../Bin/Resource/Texture/Weapon/Penta.png"));
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

	// UI
	CAssetMgr::GetInstance()->AddAsset(L"Tex_GaugeBar", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/Gauge_Bar_Normal.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_GaugeColor", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/Gauge_Bar_Full.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_DaggerBox", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/UI_WP_Dagger_Box.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_DaggerBoxStroke", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/UI_WP_Dagger_Stroke.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_DaggerThumbnail", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/BasicDagger_Thumbnail.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_DaggerBoxMouseLKey", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/Mouse_Left_Key_Dark.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_ItemBox", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/UI_WP_Item_Box.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_ItemBoxStroke", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/UI_WP_Item_Stroke.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_ChangeTab", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/UI_WP_LB.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_RKeyUI", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/R_Key_Dark.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_CKeyUI", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/C_Key_Dark.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_WPMainBox", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/UI_WP_Main_Box.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_WPMainBoxStroke", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/UI_WP_Main_Stroke.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_WPMainSubBox", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/UI_WP_Sub_Box.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_WPMainSubBoxStroke", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/UI_WP_Sub_Stroke.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_TabKeyUI", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/Tab_Key_Dark.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_O2FrameUI", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/UI_O2_Frame_New.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_O2Stroke", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/UI_O2_Stroke_re.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_O2StrokeFull", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/UI_Watch_Time_Night_all.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_WeightIcon", CAssetTexture::Create(L"../Bin/Resource/Texture/Dive_Player/UI/Icon_weight.png"));

	for (int i = 0; i < 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Dive_Player/UI/UI_Overloaded0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"UI_Overloaded", CAssetTexture::Create(s.c_str()));
	}
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
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerHitTexture", Engine::CTexture::Create(L"Tex_DivePlayerHit"))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerDieTexture", Engine::CTexture::Create(L"Tex_DivePlayerDie"))))
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


	// 보스
	for (int i = 0; i < 8; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson/Idle/Idle0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_JohnIdle", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/JohnWatson/Move/MoveDown0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_JohnMoveDown", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/JohnWatson/Move/MoveSide0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_JohnMoveSide", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/JohnWatson/Move/MoveSideDown0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_JohnMoveSideDown", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/JohnWatson/Move/MoveSideUp0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_JohnMoveSideUp", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/JohnWatson/Move/MoveUp0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_JohnMoveUp", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/JohnWatson/MeleeSkill01_Ready/MeleeSkill01_Ready0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_JohnMeleeSkill01_Ready", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/JohnWatson/Angry/Angry0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_JohnAngry", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/JohnWatson/Surprised/Surprised0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"JohnSurprised", CAssetTexture::Create(s.c_str()));
	}

	for (int i = 0; i < 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson/TL_Aim/TL_Aim0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_JohnTL_Aim", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/JohnWatson/TL_Shoot/TL_Shoot0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_JohnTL_Shoot", CAssetTexture::Create(s.c_str()));
	}


	for (int i = 0; i < 5; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson/MeleeSkill01_Attack/MeleeSkill01_Attack_01_0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"MeleeSkill01_Attack_01", CAssetTexture::Create(s.c_str()));
	}
	for (int i = 0; i < 3; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson/MeleeSkill01_Attack/MeleeSkill01_Attack_02_0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"MeleeSkill01_Attack_02", CAssetTexture::Create(s.c_str()));
	}

	// 보스 인트로
	for (int i = 0; i < 138; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson/Intro/" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Boss_JW_Intro", CAssetTexture::Create(s.c_str()));
	}


	// 보스 유도탄
	for (int i = 0; i < 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson/Bullet/JohnWatson2_GunSkill01_Missile_0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"JohnBulletA", CAssetTexture::Create(s.c_str()));
	}
	// 보스 유도탄 터지기 직전
	for (int i = 2; i < 4; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson/Bullet/JohnWatson2_GunSkill01_Missile_0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"JohnBulletB", CAssetTexture::Create(s.c_str()));
	}
	// 보스 유도탄 터짐
	for (int i = 0; i < 9; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson/Bullet/explosion0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"JohnBulletExplosion", CAssetTexture::Create(s.c_str()));
	}
	// 보스 중간에 빡쳐서 폭탄난사
	for (int i = 0; i < 7; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson/battle_angry/battle_angry0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"JohnBattleAngry", CAssetTexture::Create(s.c_str()));
	}
	// 폭탄
	for (int i = 0; i < 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson/Bullet/JohnWatson2_Mine_0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"JohnMine", CAssetTexture::Create(s.c_str()));
	}
	wstring s = L"../Bin/Resource/Texture/JohnWatson/Hit01.png";
	CAssetMgr::GetInstance()->AddAsset(L"JohnHit", CAssetTexture::Create(s.c_str()));

	// 보스2 인트로
	for (int i = 0; i < 142; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson2/Intro/" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Boss_JW2_Intro", CAssetTexture::Create(s.c_str()));
	}

	// 보스2
	for (int i = 0; i < 8; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson2/IDLE/JohnWatson2_Idle_0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"John2Idle", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/JohnWatson2/Move/JohnWatson2_Move_0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"John2Move", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/JohnWatson2/GunSkill01_Aim/JohnWatson2_GunSkill01_Aim_0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"John2GunSkill01_Aim", CAssetTexture::Create(s.c_str()));
	}
	for (int i = 0; i < 4; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson2/GunSkill01_Shoot/JohnWatson2_GunSkill01_Shoot_0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"John2GunSkill01_Shoot", CAssetTexture::Create(s.c_str()));
	}

	for (int i = 0; i < 3; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson2/GunSkill02_Aim/JohnWatson2_GunSkill02_Aim_0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"John2GunSkill02_Aim", CAssetTexture::Create(s.c_str()));
	}
	for (int i = 0; i < 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/JohnWatson2/MeleeSkill01_Ready/JohnWatson2_MeleeSkill01_Ready_0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"John2MeleeSkill01_Ready", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/JohnWatson2/MeleeSkill01_Attack/JohnWatson2_MeleeSkill01_Attack_0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"John2MeleeSkill01_Attack", CAssetTexture::Create(s.c_str()));

		s = L"../Bin/Resource/Texture/JohnWatson2/MeleeSkill01_Return/JohnWatson2_MeleeSkill01_Return_0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"John2MeleeSkill01_Return", CAssetTexture::Create(s.c_str()));
	}

	s = L"../Bin/Resource/Texture/JohnWatson2/GunSkill02_Shoot/JohnWatson2_GunSkill02_Shoot_01.png";
	CAssetMgr::GetInstance()->AddAsset(L"John2GunSkill02_Shoot", CAssetTexture::Create(s.c_str()));

	for (int i = 0; i < 12; ++i)
	{
		wstring s;
		if(i < 9)
			s = L"../Bin/Resource/Texture/JohnWatson2/Angry/JohnWatson2_Angry_0" + ::to_wstring(i + 1) + L".png";
		else if(i >= 9)
			s = L"../Bin/Resource/Texture/JohnWatson2/Angry/JohnWatson2_Angry_" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"John2Angry", CAssetTexture::Create(s.c_str()));
	}

	// 잠수함(플레이어)
	s = L"../Bin/Resource/Texture/Dive_Player/subMarine.png";
	CAssetMgr::GetInstance()->AddAsset(L"SubMarine", CAssetTexture::Create(s.c_str()));

	// 작살 줄
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_LineBuffer", Engine::CLineBuffer::Create())))
		return E_FAIL;

	//사운드
	{
		CAssetMgr::GetInstance()->AddAsset(L"Sound_DiveInGameBGM", CAssetFmodSound::Create(L"../Bin/Resource/Sound/DiveDave/BGM/BGM_InGame.wav"));

	}

	//테스트용
	CAssetMgr::GetInstance()->AddAsset(L"Tex_TestFish", CAssetTexture::Create(L"../Bin/Resource/Texture/Item/Item_O2Capsule.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TestFishTexture", Engine::CTexture::Create(L"Tex_TestFish"))))
		return E_FAIL;

	// Particle
	for (int i = 0; i < 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Particle/Bubble" + ::to_wstring(i) + L".png";
	
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Bubble", CAssetTexture::Create(s.c_str()));
	}

	{
		wstring s = L"../Bin/Resource/Texture/Particle/SeaBubble" + ::to_wstring(0) + L".png";

		CAssetMgr::GetInstance()->AddAsset(L"Tex_SeaBubble", CAssetTexture::Create(s.c_str()));
	}

	{
		wstring s = L"../Bin/Resource/Texture/Particle/Coin" + ::to_wstring(0) + L".png";
	
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Coin", CAssetTexture::Create(s.c_str()));
	}
	{
		wstring s = L"../Bin/Resource/Texture/Particle/BloomBubble" + ::to_wstring(2) + L".png";
	
		CAssetMgr::GetInstance()->AddAsset(L"Tex_BloomBubble", CAssetTexture::Create(s.c_str()));
	}

	for (int i = 0; i < 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Particle/Blood" + ::to_wstring(i) + L".png";

		CAssetMgr::GetInstance()->AddAsset(L"Tex_Blood", CAssetTexture::Create(s.c_str()));
	}

	CParticleMgr::GetInstance()->Ready_Particle(CInfoMgr::GetInstance()->Get_HWND());
	for (int i = 0; i < 12; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Coral/Coral" + ::to_wstring(i) + L".png";
		wstring nameCoral = L"Tex_Coral" + ::to_wstring(i);
		CAssetMgr::GetInstance()->AddAsset(nameCoral, CAssetTexture::Create(s.c_str()));
	}


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

	/////////////손님 2

	for (int i = 0; i < 8; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer2/black_walk" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer2Walk", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer2WalkTexture", Engine::CTexture::Create(L"Tex_Customer2Walk"))))
		return E_FAIL;
	for (int i = 0; i < 3; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer2/black_wait" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer2Wait", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer2WaitTexture", Engine::CTexture::Create(L"Tex_Customer2Wait"))))
		return E_FAIL;
	for (int i = 0; i < 3; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer2/black_eat" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer2Eat", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer2EatTexture", Engine::CTexture::Create(L"Tex_Customer2Eat"))))
		return E_FAIL;
	for (int i = 0; i < 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer2/black_happy" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer2Happy", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer2HappyTexture", Engine::CTexture::Create(L"Tex_Customer2Happy"))))
		return E_FAIL;
	for (int i = 0; i < 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer2/black_anger" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer2Anger", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer2AngerTexture", Engine::CTexture::Create(L"Tex_Customer2Anger"))))
		return E_FAIL;
	///////
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer2Menu", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/Customer/Customer2/black_menu0.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer2MenuTex", Engine::CTexture::Create(L"Tex_Customer2Menu"))))
		return E_FAIL;
	//손님 3
	for (int i = 1; i < 9; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer3/Walk0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer3Walk", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer3WalkTexture", Engine::CTexture::Create(L"Tex_Customer3Walk"))))
		return E_FAIL;
	for (int i = 1; i < 4; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer3/Wait0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer3Wait", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer3WaitTexture", Engine::CTexture::Create(L"Tex_Customer3Wait"))))
		return E_FAIL;
	for (int i = 1; i < 4; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer3/Eat0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer3Eat", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer3EatTexture", Engine::CTexture::Create(L"Tex_Customer3Eat"))))
		return E_FAIL;
	for (int i = 1; i < 3; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer3/Happy0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer3Happy", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer3HappyTexture", Engine::CTexture::Create(L"Tex_Customer3Happy"))))
		return E_FAIL;
	for (int i = 1; i < 3; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/Customer/Customer3/Anger0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer3Anger", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer3AngerTexture", Engine::CTexture::Create(L"Tex_Customer3Anger"))))
		return E_FAIL;
	///////
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Customer3Menu", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/Customer/Customer3/Menu.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Customer3MenuTex", Engine::CTexture::Create(L"Tex_Customer3Menu"))))
		return E_FAIL;
	//여기까지
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
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Dart", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Sushi/Sushi_SmallspottedDart.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DartTex", Engine::CTexture::Create(L"Tex_Dart"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_YellowTang", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Sushi/Sushi_YellowTang.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_YellowTangTex", Engine::CTexture::Create(L"Tex_YellowTang"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_YellowBack", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Sushi/Sushi_YellowbackFusilier.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_YellowBackTex", Engine::CTexture::Create(L"Tex_YellowBack"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_BanchoSushi", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Sushi/bancho_sushi.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BanchoSushi", Engine::CTexture::Create(L"Tex_BanchoSushi"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Fishcell", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/UI_Fishcell.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Fishcell", Engine::CTexture::Create(L"Tex_Fishcell"))))
		return E_FAIL;

	//물고기 사진
	CAssetMgr::GetInstance()->AddAsset(L"Tex_BluejongP", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Fish/Bluetang.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_BluejongP", Engine::CTexture::Create(L"Tex_BluejongP"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_ClownFishP", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Fish/Clownfish.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_ClownFishP", Engine::CTexture::Create(L"Tex_ClownFishP"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_DartP", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Fish/Smallspotted_dart.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DartP", Engine::CTexture::Create(L"Tex_DartP"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_YellowTangP", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Fish/Yellow_Tang.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_YellowTangP", Engine::CTexture::Create(L"Tex_YellowTangP"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_YellowBackP", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Fish/Yellowback_Fusilier.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_YellowBackP", Engine::CTexture::Create(L"Tex_YellowBackP"))))
		return E_FAIL;

	CAssetMgr::GetInstance()->AddAsset(L"Tex_HeadP", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Fish/JohnHead.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_HeadP", Engine::CTexture::Create(L"Tex_HeadP"))))
		return E_FAIL;
	//깊이 사진
	CAssetMgr::GetInstance()->AddAsset(L"Tex_DepthP", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/depth.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DepthTex", Engine::CTexture::Create(L"Tex_DepthP"))))
		return E_FAIL;
	//강화 , 확인
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Upgrade", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/upgrade.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_UpgradeTex", Engine::CTexture::Create(L"Tex_Upgrade"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Okay", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/okay.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_OkayTex", Engine::CTexture::Create(L"Tex_Okay"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_fishConfirm", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/fishConfirm.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_fishConfirmTex", Engine::CTexture::Create(L"Tex_fishConfirm"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_arrowRight", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/arrowRight.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_arrowRightTex", Engine::CTexture::Create(L"Tex_arrowRight"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_arrowLeft", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/arrowLeft.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_arrowLeftTex", Engine::CTexture::Create(L"Tex_arrowLeft"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_close", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/CloseButton.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CloseButtonTex", Engine::CTexture::Create(L"Tex_close"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_max", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/maxButton.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_maxButtonTex", Engine::CTexture::Create(L"Tex_max"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_overlay", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/overlay.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_OverlayTex", Engine::CTexture::Create(L"Tex_overlay"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_SelectFrame", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/SelectedMenu.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SelectFrameTex", Engine::CTexture::Create(L"Tex_SelectFrame"))))
		return E_FAIL;
	//요리 강화 
	CAssetMgr::GetInstance()->AddAsset(L"Tex_upgradeFrame", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/upgradeFrame.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_upgradeFrameTex", Engine::CTexture::Create(L"Tex_upgradeFrame"))))
		return E_FAIL;

	CAssetMgr::GetInstance()->AddAsset(L"Tex_upgradeConfirm", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/upgradeConfirm.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_upgradeConfirmTex", Engine::CTexture::Create(L"Tex_upgradeConfirm"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_SmallMenu", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/smallMenu1.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SmallMenu", Engine::CTexture::Create(L"Tex_SmallMenu"))))
		return E_FAIL;
	
	//영업 시작
	CAssetMgr::GetInstance()->AddAsset(L"Tex_OpenShop", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/openShop.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_OpenShopTex", Engine::CTexture::Create(L"Tex_OpenShop"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_menuBubble", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/menuBubble.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_MenuBubbleTex", Engine::CTexture::Create(L"Tex_menuBubble"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Red", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/red.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RedTex", Engine::CTexture::Create(L"Tex_Red"))))
		return E_FAIL;
	//Wasabi_01

	for (int i = 1; i < 9; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/SushiBar/UI/Wasabi_0" + ::to_wstring(i) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_WasabiAnim", CAssetTexture::Create(s.c_str()));
	}
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WasabiAnim", Engine::CTexture::Create(L"Tex_WasabiAnim"))))
		return E_FAIL;

	CAssetMgr::GetInstance()->AddAsset(L"Tex_Puke", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/puke.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_CookingBox", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/UI_Sushi_Cooking_Box2.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_CookingComplete", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/UI_Sushi_Cooking_Box_Complete1.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_CookingIcon", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/UI_Sushi_Cooking_Icon.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Yellow", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Yellow.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_CookingGauge", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/UI_Sushi_Cooking_Gauge1.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_MenuChoosing", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Menu_Choosing.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_SushiBox2", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/SushiBox2.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Happy", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Happy.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Angry", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Angry.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_FKey", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/F_Key_Dark.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_KKey", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/K_Key_Dark.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_WasabiFrame", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/UI_QTE_WasabiBG.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_WasabiGaugeBar", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/WasabiGaugeBar.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_WasabiGaugeStancil", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/WasabiGaugeStancil.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_WasabiIcon", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/WasabiIcon.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_UpgradeArrow", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/upgradeArrow1.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_UpgradeArrow", Engine::CTexture::Create(L"Tex_UpgradeArrow"))))
		return E_FAIL;
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Wasabi", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/wasabi.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Good", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/good.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Orange", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Orange.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_BrownFrame", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/brownFrame.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Coin", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/Customer/Effect/Coin16.png"));

	CAssetMgr::GetInstance()->AddAsset(L"Tex_WasabiObject", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/wasabiObject.png"));
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WasabiObjectTex", Engine::CTexture::Create(L"Tex_WasabiObject"))))
		return E_FAIL;

	//플레이어 게이지
	CAssetMgr::GetInstance()->AddAsset(L"Tex_DaveGauge", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/Dave/DashGauge.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_DaveGaugeStancil", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/Dave/DashGaugeStancil.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Purple", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Purple.png"));

	//오픈 이미지
	CAssetMgr::GetInstance()->AddAsset(L"Tex_OpenImage", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/UI_SushiOpenText.png"));
	// 차
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Pot", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Pot.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_CupBack", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/CupBack.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_CupFront", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/CupFront.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_CupFrame", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/CupFrame.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Green", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Green.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_TeaLiquid", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/TeaLiquid.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_TeaLiquidStancil", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/TeaLiquidStancil3.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Brown", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Brown.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Perfect", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/Perfect.png"));
	CAssetMgr::GetInstance()->AddAsset(L"Tex_TeaPicture", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/teaPicture.png"));

	//게임 끝 이미지
	CAssetMgr::GetInstance()->AddAsset(L"Tex_CloseShop", CAssetTexture::Create(L"../Bin/Resource/Texture/SushiBar/UI/CloseShop.png"));

	//사운드
	{
		CAssetMgr::GetInstance()->AddAsset(L"Sound_BGM", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/BGM_SushiBar_Night.ogg"));
		CAssetMgr::GetInstance()->AddAsset(L"Sound_ClospShop", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/ui_sushibar_close.ogg"));
		CAssetMgr::GetInstance()->AddAsset(L"Sound_OpenShop", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/ui_sushibar_open.ogg"));
		CAssetMgr::GetInstance()->AddAsset(L"Sound_Pay", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/sound_sushibar_pay.ogg"));

		//와사비
		CAssetMgr::GetInstance()->AddAsset(L"Sound_Grind", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/sushi_wasabi_grind.ogg"));
		CAssetMgr::GetInstance()->AddAsset(L"Sound_WasabiGood", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/sushi_wasabi_good.ogg"));

		//차
		CAssetMgr::GetInstance()->AddAsset(L"Sound_TeaPouring", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/sushi_tea_pouring.ogg"));
		CAssetMgr::GetInstance()->AddAsset(L"Sound_Perfect", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/sushi_drink_perfect.ogg"));
		//손님 
		CAssetMgr::GetInstance()->AddAsset(L"Sound_Served", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/sushi_customer_served.ogg"));
		CAssetMgr::GetInstance()->AddAsset(L"Sound_Eat", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/sushi_customer_eat_04.ogg"));
		//데이브
		CAssetMgr::GetInstance()->AddAsset(L"Sound_DaveWalk", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/sound_dave_foot_01.ogg"));
		CAssetMgr::GetInstance()->AddAsset(L"Sound_DaveRun", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/sound_dave_dash_02.ogg"));
		CAssetMgr::GetInstance()->AddAsset(L"Sound_DaveTired", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/sound_dave_tired_01.ogg"));

		//버튼
		CAssetMgr::GetInstance()->AddAsset(L"Sound_OpenButton", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/ui_button_click_open_01.ogg"));
		CAssetMgr::GetInstance()->AddAsset(L"Sound_CloseButton", CAssetFmodSound::Create(L"../Bin/Resource/Sound/SushiBar/ui_button_click_close_01.ogg"));

	}


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

	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_NightBG", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/ShipBG/ShipNightBG.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_NightBG");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_NightFakeBG", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/ShipBG/ShipNighFakeBG.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_NightFakeBG");

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

	// GoSushiBtn
	CAssetMgr::GetInstance()->AddAsset(L"Tex_Ship_GoSushiBtn", CAssetTexture::Create(L"../Bin/Resource/Texture/Ship/GoSushiBtn.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_Ship_GoSushiBtn");




	CAssetMgr::GetInstance()->AddAsset(L"Tex_UI_Gun_Basic", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/gun_basic.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_UI_Gun_Basic");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_UI_Gun_Triple_Accel", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/gun_triple_accel.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_UI_Gun_Triple_Accel");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_UI_Gun_Quadro_Accel", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/gun_quadro_accel.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_UI_Gun_Quadro_Accel");

	CAssetMgr::GetInstance()->AddAsset(L"Tex_UI_Gun_Penta_Accel", CAssetTexture::Create(L"../Bin/Resource/Texture/UI/gun_penta_accel.png"));
	CAssetMgr::GetInstance()->LoadAsset(L"Tex_UI_Gun_Penta_Accel");

	return S_OK;
}

HRESULT CTransition::Common_SHIP_Unload()
{
	
	CAssetMgr::GetInstance()->DelAsset(L"Tex_ShipDave_Idle");

	CAssetMgr::GetInstance()->DelAsset(L"Tex_ShipDave_Walk");

	CAssetMgr::GetInstance()->DelAsset(L"Tex_ShipDave_DiveReady");

	CAssetMgr::GetInstance()->DelAsset(L"Tex_DiverBoxInvenTmp");

	CAssetMgr::GetInstance()->DelAsset(L"Tex_PhoneIcon");

	CAssetMgr::GetInstance()->DelAsset(L"Tex_PhoneBG");

	CAssetMgr::GetInstance()->DelAsset(L"Tex_PhoneApp");

	CAssetMgr::GetInstance()->DelAsset(L"Tex_PhoneAppAlpha");

	CAssetMgr::GetInstance()->DelAsset(L"Tex_DiverBox");

	//Tex_PhoneIDiverBG
	CAssetMgr::GetInstance()->DelAsset(L"Tex_PhoneIDiverBG");

	//UI_IDiverItem.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_UI_IDiverItem");

	//UI_IDiverItem.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_UI_IDiverUpgrade");

	//UI_IDiverItem.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_BG");

	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_FakeBG");

	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_UI_DiveBtn");

	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_UI_DiveSpriteBtn");

	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_UI_GoBtn");

	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_UI_GoBtnAlpha");

	//Space_Key_Dark_Symbol
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_UI_SpaceKey");

	//MoneyUI
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_UI_Money");

	// DiverBoxDave
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_DiverBoxDave");


	//InventoryBoxEdge
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_InventoryBoxEdge");


	//Jaksal
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_InventoryUpperItem_Jaksal");

	//Gun
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_InventoryUpperItem_Gun");

	//Knief
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_InventoryUpperItem_Knief");

	//Jusin
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_InventoryUpperItem_Jusin");


	//ItemDescUI.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_Inventory_ItemDescUI");

	//WoodPanel.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_InventoryUpperItem_WoodPanel");


	// Item_Sanso.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_IDiver_Item_Sanso");

	// Item_Clothes.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_IDiver_Item_Clothes");

	// Item_Clothes.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_IDiver_Item_Cage");

	// Item_Jaksal.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_IDiver_Item_Jaksal");

	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_IDiver_Edge");

	//UpgradeSuccess.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_IDiver_UpgradeSuccess");

	//UpgrddeBtn.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_IDiver_UpgradeBtn");

	//UpgrddeBtnAlpha.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_IDiver_UpgradeBtnAlpha");

	// 
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_Weapon_BG");

	//UI_WeaponCraft_Logo.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_Weapon_Logo");

	//Close.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_Weapon_Close");

	//UI_Area.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_Weapon_UI_Area");

	//UI_CreateBtn.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_Weapon_UI_CreateBtn");

	//Edge.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_Weapon_UI_Edge");

	//GusikRifle.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_Weapon_GukikRifle");

	//NewWeapon.png
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Ship_Weapon_UI_NewWeapon");
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
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Logo_BG");
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Logo_Title");
	CAssetMgr::GetInstance()->DelAsset(L"Tex_Logo_Black1pxAlpha");
	return S_OK;
}

HRESULT CTransition::Ready_Scene()
{


	Update_Camera();





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
	Update_Camera();
	if (!m_bLoadingStart)
	{
		m_bLoadingStart = true;

		InitializeCriticalSection(&m_Crt);
		m_hThread = (HANDLE)_beginthreadex(NULL, // 보안 속성(핸들의 상속 여부, NULL인 경우 상속에서 제외)
			0,  // 디폴트 스탯 사이즈(1 바이트)
			Thread_Main, // 구동할 쓰레드 함수
			this,          // 3번 매개 변수 함수를 통해 가공할 데이터 주소
			0,             // 쓰레드 생성 및 실행을 조정하기 위한 옵션
			NULL);         // 쓰레드 ID
	}

	CScene::Update_Scene(fTimeDelta);

	if (m_bFadeEnd && m_bFinish)
	{
		m_bFadeEnd = false;
		if (m_eDstScene == SCENE_LOGO)
		{
			AddFadeOut(this, [=]() {
				auto pLogo = CLogo::Create();
				AddFadeIn(pLogo);
				CManagement::GetInstance()->Set_Scene(pLogo);
				});
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
			AddFadeOut(this, [=]() {
				auto pLogo = CSushi::Create();
				AddFadeIn(pLogo);
				CManagement::GetInstance()->Set_Scene(pLogo);
				});
		}
		else if (m_eDstScene == SCENE_DIVE)
		{
			AddFadeOut(this, [=]() {
				auto pLogo = CDive::Create();
				AddFadeIn(pLogo);
				CManagement::GetInstance()->Set_Scene(pLogo);
				});
		}
	}
	


	

	return 0;
}

void CTransition::LateUpdate_Scene(const _float& fTimeDelta)
{
	Update_Camera();
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

	// 봉인
	//CLeah* pLeah = CLeah::Create();
	//pLeah->Set_Parent(pTransitionBG);
	//if (FAILED(pLayer->Add_GameObject(L"Leah", pLeah)))
	//	return E_FAIL;

		
	if (m_eDstScene == SCENE_SHIP)
	{
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
			CTransitionTxt* pTxt = CTransitionTxt::Create(0.f, 0.6f);
			pTxt->Set_Txt(L"로비로비로");
			pTxt->Set_Opt(DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
			if (nullptr == pTxt)
				return E_FAIL;
			if (FAILED(pLayer->Add_GameObject(L"TransitionTipTxt", pTxt)))
				return E_FAIL;
		}
		
	}
	else if (m_eDstScene == SCENE_DIVE)
	{
		{
			CTransitionImg* pTransitionImg = CTransitionImg::Create(0.f, 0.1f);
			pTransitionImg->Set_AssetName(L"Tex_Transition_BG_Dive");

			pTransitionImg->Set_CustomScaleX(0.1f);
			pTransitionImg->Set_CustomScaleY(0.1f);

			pTransitionImg->Ready_AfterCreate();


			if (nullptr == pTransitionImg)
				return E_FAIL;
			if (FAILED(pLayer->Add_GameObject(L"TransitionImg", pTransitionImg)))
				return E_FAIL;
		}

		{
			CTransitionTxt* pTxt = CTransitionTxt::Create(0.f, 0.6f);
			pTxt->Set_Txt(L"두다이브");
			pTxt->Set_Opt(DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
			if (nullptr == pTxt)
				return E_FAIL;
			if (FAILED(pLayer->Add_GameObject(L"TransitionTipTxt", pTxt)))
				return E_FAIL;
		}
	}
	else if (m_eDstScene == SCENE_SUSHI)
	{
		{
			CTransitionImg* pTransitionImg = CTransitionImg::Create(0.f, 0.1f);
			pTransitionImg->Set_AssetName(L"Tex_Transition_BG_Sushi");

			pTransitionImg->Set_CustomScaleX(0.1f);
			pTransitionImg->Set_CustomScaleY(0.1f);

			pTransitionImg->Ready_AfterCreate();


			if (nullptr == pTransitionImg)
				return E_FAIL;
			if (FAILED(pLayer->Add_GameObject(L"TransitionImg", pTransitionImg)))
				return E_FAIL;
		}

		{
			CTransitionTxt* pTxt = CTransitionTxt::Create(0.f, 0.6f);
			pTxt->Set_Txt(L"회식은갓파스시?");
			pTxt->Set_Opt(DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
			if (nullptr == pTxt)
				return E_FAIL;
			if (FAILED(pLayer->Add_GameObject(L"TransitionTipTxt", pTxt)))
				return E_FAIL;
		}
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
		wstring tipTxt[3];
		tipTxt[0] = L"그거 아시나요? 거북이는 엉덩이로 숨을 쉴 수 있어서 물속에서 오래 버텨요.";
		tipTxt[1] = L"그거 아시나요? 굴은 살면서 필요에 따라 암수 성별을 자유자재로 바꿀 수 있답니다.";
		tipTxt[2] = L"그거 아시나요? 흰수염고래의 혀 무게만 해도 코끼리 한 마리랑 맞먹을 정도로 커요.";

		CTransitionTxt* pTxt = CTransitionTxt::Create(0.f, -0.6f);
		pTxt->Set_Txt(tipTxt[rand() % 3]);
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
			if (!CTransition::s_LogoAssetLoaded)
			{
				CTransition::s_LogoAssetLoaded = true;
				pTransition->Transition_INIT_TO_LOGO();
			}
			else
			{
				pTransition->Set_Finish();
			}
		}
	}
	else if (eSrc == SCENE_LOGO)
	{
		if (eDst == SCENE_SHIP)
		{
			if (!CTransition::s_ShipAssetLoaded)
			{
				CTransition::s_ShipAssetLoaded = true;
				pTransition->Transition_LOGO_TO_SHIP();
			}
			else
			{
				pTransition->Set_Finish();
			}
		}
	}
	else if (eSrc == SCENE_SHIP)
	{
		if (eDst == SCENE_DIVE)
		{
			if (!CTransition::s_DiveAssetLoaded)
			{
				CTransition::s_DiveAssetLoaded = true;
				pTransition->Transition_SHIP_TO_DIVE();
			}
			else
			{
				pTransition->Set_Finish();
			}
		}
		else if (eDst == SCENE_SUSHI)
		{
			if (!CTransition::s_SushiAssetLoaded)
			{
				CTransition::s_SushiAssetLoaded = true;
				pTransition->Transition_SHIP_TO_SUSHI();
			}
			else
			{
				pTransition->Set_Finish();
			}
			
		}
		else if (eDst == SCENE_LOGO)
		{
			if (!CTransition::s_LogoAssetLoaded)
			{
				CTransition::s_LogoAssetLoaded = true;
				pTransition->Transition_SHIP_TO_LOGO();
			}
			else
			{
				pTransition->Set_Finish();
			}
		}
	}
	else if (eSrc == SCENE_DIVE)
	{
		if (eDst == SCENE_SHIP)
		{
			if (!CTransition::s_ShipAssetLoaded)
			{
				CTransition::s_ShipAssetLoaded = true;
				pTransition->Transition_DIVE_TO_SHIP();
			}
			else
			{
				pTransition->Set_Finish();
			}
		}
	}
	else if (eSrc == SCENE_SUSHI)
	{
		if (eDst == SCENE_SHIP)
		{
			if (!CTransition::s_ShipAssetLoaded)
			{
				CTransition::s_ShipAssetLoaded = true;
				pTransition->Transition_SUSHI_TO_SHIP();
			}
			else
			{
				pTransition->Set_Finish();
			}
		}
	}


	LeaveCriticalSection(pTransition->Get_Crt());

	//_endthreadex(0);

	return iFlag;       // 0 리턴 시, _endthreadex가 자동 호출
}

void CTransition::Update_Camera()
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
}