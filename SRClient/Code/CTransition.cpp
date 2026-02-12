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




	m_sComment = L"Transition_LOGO_TO_SHIP COMPLETE";
#ifdef _DEBUG
	//Sleep(500);
#endif // DEBUG
	m_bFinish = true;

	return S_OK;
}

HRESULT CTransition::Transition_SHIP_TO_LOGO()
{
	m_sComment = L"Transition_SHIP_TO_LOGO COMPLETE";
#ifdef _DEBUG
	//Sleep(500);
#endif // DEBUG
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
	}

	for (int i = 0; i < 2; ++i)
	{
		wstring s = L"../Bin/Resource/Texture/Dive_Player/Attack/AttackReady0" + ::to_wstring(i + 1) + L".png";
		CAssetMgr::GetInstance()->AddAsset(L"Tex_DivePlayerAttackReady", CAssetTexture::Create(s.c_str()));
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
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DivePlayerAttackReadyTexture", Engine::CTexture::Create(L"Tex_DivePlayerAttackReady"))))
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
#ifdef _DEBUG
	Sleep(500);
#endif // DEBUG
	m_bFinish = true;

	return S_OK;
}

HRESULT CTransition::Transition_SHIP_TO_SUSHI()
{
	m_sComment = L"Transition_SHIP_TO_SUSHI COMPLETE";
#ifdef _DEBUG
	Sleep(500);
#endif // DEBUG
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