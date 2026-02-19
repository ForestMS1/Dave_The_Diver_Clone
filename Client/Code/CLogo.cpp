#include "pch.h"
#include "CLogo.h"
#include "CBackGround.h"
#include "CProtoMgr.h"
#include "CStage.h"
#include "CManagement.h"
#include "CFontMgr.h"
#include "CDInputMgr.h"
#include "CLog.h"
#include "CSoundMgr.h"
#include "CHelper.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetDefaultFont.h"

CLogo::CLogo()
	: CScene(), m_pLoading(NULL)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	if (FAILED(Ready_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
		return E_FAIL;

	//if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
	//	return E_FAIL;

	m_pLoading = CLoading::Create(CLoading::LOADING_STAGE);
	if (nullptr == m_pLoading)
		return E_FAIL;

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	if (m_pLoading->Get_Finish())
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			Engine::CScene* pStage = CStage::Create();

			if (nullptr == pStage)
				return -1;

			if (FAILED(CManagement::GetInstance()->Set_Scene(pStage)))
			{
				MSG_BOX("Stage Scene Change Failed");
				return -1;
			}
		}
			
		// 입력 테스트
		{
			if (CDInputMgr::GetInstance()->Key_Down(DIK_Q))
			{
				CLog::Debug(L"Q DOWN\n");
			}
			if (CDInputMgr::GetInstance()->Key_Pressing(DIK_Q))
			{
				CLog::Debug(L"Q PRESSED\n");
			}
			if (CDInputMgr::GetInstance()->Key_Up(DIK_Q))
			{
				CLog::Debug(L"Q UP\n");
			}

			if (CDInputMgr::GetInstance()->Key_Down(DIK_LEFT))
			{
				CLog::Debug(L"DIK_LEFT DOWN\n");
			}
			if (CDInputMgr::GetInstance()->Key_Pressing(DIK_LEFT))
			{
				CLog::Debug(L"DIK_LEFT PRESSED\n");
			}
			if (CDInputMgr::GetInstance()->Key_Up(DIK_LEFT))
			{
				CLog::Debug(L"DIK_LEFT UP\n");
			}

			if (CDInputMgr::GetInstance()->Mouse_Down(DIM_RB))
			{
				CLog::Debug(L"M LB DOWN\n");
			}
			if (CDInputMgr::GetInstance()->Mouse_Pressing(DIM_RB))
			{
				CLog::Debug(L"M LB PRESSINg\n");
			}
			if (CDInputMgr::GetInstance()->Mouse_Up(DIM_RB))
			{
				CLog::Debug(L"M LB UP\n");
			}
		}
	}

	// 사운드 테스트
	{
		ImGui::Begin("SOUND TEST");
		_tchar wText[] = L"타다";
		char utf8Buf[256];
		CHelper::ConvertTCharToUTF8(wText, utf8Buf);
		if (ImGui::Button(utf8Buf))
		{
			CLog::Debug(L"Clicked!1\n");
			CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_TADA", CSoundMgr::SFX, 1.f);
		}
			
		if (ImGui::Button("BGM"))
		{
			CLog::Debug(L"Clicked!2\n");
			CSoundMgr::GetInstance()->PlaySoundLoop(L"Sound_Success", CSoundMgr::BGM, 1.f);
		}	
		ImGui::End();
	}


	return iExit;
}

void CLogo::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CLogo::Render_Scene()
{
	// debug 용 출력

	_vec2	vPos{ 0.f, 0.f };
	CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Griun");
	pDefFont->Render_Font(m_pLoading->Get_String(), &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	_vec2	vPos2{ 0.f, 30.f };
	CAssetDefaultFont* pDefFont2 = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Default");
	pDefFont2->Render_Font(m_pLoading->Get_String(), &vPos2, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	_vec2	vPos3{ 0.f, 60.f };
	CAssetDefaultFont* pDefFont3 = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Jinji");
	pDefFont3->Render_Font(m_pLoading->Get_String(), &vPos3, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


}

HRESULT CLogo::Ready_Environment_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;


	// BackGround
	pGameObject = CBackGround::Create();
	
	if (nullptr == pGameObject)
		return E_FAIL;
	
	if(FAILED(pLayer->Add_GameObject(L"BackGround", pGameObject)))
		return E_FAIL;

	
	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

HRESULT CLogo::Ready_Prototype()
{
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcTex", Engine::CRcTex::Create())))
		return E_FAIL;

	CAssetMgr::GetInstance()->AddAsset(L"Tex_LOGO", CAssetTexture::Create(L"../Bin/Resource/Texture/Logo/IU.jpg"));
	CAssetMgr::GetInstance()->LoadAsset();
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_LogoTexture", Engine::CTexture::Create(L"Tex_LOGO"))))
		return E_FAIL;

	return S_OK;
}

CLogo* CLogo::Create()
{
	CLogo* pLogo = new CLogo;

	if (FAILED(pLogo->Ready_Scene()))
	{
		Safe_Release(pLogo);
		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pLogo;
}

void CLogo::Free()
{
	Safe_Release(m_pLoading);

	CScene::Free();
}
