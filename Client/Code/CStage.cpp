#include "pch.h"
#include "CStage.h"
#include "CProtoMgr.h"
#include "CDynamicCamera.h"
#include "CSkyBox.h"
#include "CLightMgr.h"
#include "CEffect.h"
#include "CBackGround.h"
#include "CCube.h"
#include "CDInputMgr.h"
#include "Firework.h"
#include "CParticleMgr.h"
#include "CGraphicDev.h"
#include "CTestCube.h"
#include "CColliderMgr.h"
#include "CUIMgr.h"

CStage::CStage()
	: CScene()
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
		return E_FAIL;

	if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
		return E_FAIL;
	
	if (FAILED(Ready_UI_Layer(L"UI_Layer")))
		return E_FAIL;


	return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	if (auto pCube = Get_Layer(L"GameLogic_Layer")->Get_GameObjectFirst<CCube>(L"Cube1"))
	{
		pCube->Get_Transform()->Rotation(ROT_X,  D3DXToRadian(10.f));
	}

	if (auto pCube = Get_Layer(L"GameLogic_Layer")->Get_GameObjectFirst<CCube>(L"Cube2"))
	{
		pCube->Get_Transform()->Rotation(ROT_Y, D3DXToRadian(10.f));
	}

	if (ImGui::Button("DeadTest"))
	{
		if (CCube* pCube = dynamic_cast<CCube*>(this->Get_Layer(L"GameLogic_Layer")->Get_GameObjectFirst(L"Cube2")))
		{
			pCube->Set_Parent(nullptr);
			//pCube->Set_Dead();
		}
	}
	if (ImGui::Button("Toggle Collider"))
	{
		CColliderMgr::GetInstance()->Set_Render(!CColliderMgr::GetInstance()->Get_Render());
	}
	return iExit;
}

void CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CStage::Render_Scene()
{
	// debug ¿ë Ãâ·Â
}

HRESULT CStage::Ready_Environment_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	_vec3	vEye{ 0.f, 10.f, -10.f };
	_vec3	vAt{ 0.f, 0.f, 1.f };
	_vec3	vUp{ 0.f, 1.f, 0.f };

	// DynamicCamera
	pGameObject = CDynamicCamera::Create( &vEye, &vAt, &vUp);
	//CParticleMgr::GetInstance()->set
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"DynamicCamera", pGameObject)))
		return E_FAIL;
	CParticleMgr::GetInstance()->Set_Camera(static_cast<CCamera*>(pGameObject));

	// SkyBox
	pGameObject = CSkyBox::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"SkyBox", pGameObject)))
		return E_FAIL;
	

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

HRESULT CStage::Ready_GameLogic_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;


	// Player
	pGameObject = CPlayer::Create();
	
	if (nullptr == pGameObject)
		return E_FAIL;
	
	if (FAILED(pLayer->Add_GameObject(L"Player", pGameObject)))
		return E_FAIL;

	CGameObject* pTmp = pGameObject;

	pGameObject = CCube::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Cube1", pGameObject)))
		return E_FAIL;
	pGameObject->Set_Parent(pTmp);

	pTmp = pGameObject;
	pGameObject = CCube::Create();
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Cube2", pGameObject)))
		return E_FAIL;
	pGameObject->Set_Parent(pTmp);
	

	{
		_vec3 vPos = {10.f , 15.f, 10.f };
		CTestCube* pGameObject = CTestCube::Create(&vPos, COLL_AABB);
		pGameObject->Set_Speed(13.f);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"TestCube", pGameObject)))
			return E_FAIL;
	}
	{
		_vec3 vPos = { 11.f , 16.f, 11.f };
		CTestCube* pGameObject = CTestCube::Create(&vPos, COLL_OBB);
		
		pGameObject->Set_Speed(7.f);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"TestCube", pGameObject)))
			return E_FAIL;
	}

	{
		_vec3 vPos = { 12.f , 17.f, 12.f };
		CTestCube* pGameObject = CTestCube::Create(&vPos, COLL_SPHERE);

		pGameObject->Set_Speed(5.f);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"TestCube", pGameObject)))
			return E_FAIL;
	}

	{
		_vec3 vPos = { 13.f , 15.f, 13.f };
		CTestCube* pGameObject = CTestCube::Create(&vPos, COLL_FRUSTUM);

		pGameObject->Set_Speed(2.f);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"TestCube", pGameObject)))
			return E_FAIL;
	}


	//
	//// Monster
	//pGameObject = CMonster::Create(m_pGraphicDev);
	//
	//if (nullptr == pGameObject)
	//	return E_FAIL;
	//
	//if (FAILED(pLayer->Add_GameObject(L"Monster", pGameObject)))
	//	return E_FAIL;

	// Terrain
	pGameObject = CTerrain::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
		return E_FAIL;


	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

HRESULT CStage::Ready_UI_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	for (_uint i = 0; i < 50; ++i)
	{
		// Player
		pGameObject = CEffect::Create();

		if (nullptr == pGameObject)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(L"Effect", pGameObject)))
			return E_FAIL;
	}



	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Light()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	D3DLIGHT9	tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tLightInfo.Direction = { 1.f, -1.f, 1.f };

	if (FAILED(CLightMgr::GetInstance()->Ready_Light(pGraphicDev, &tLightInfo, 0)))
		return E_FAIL;


	//tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	//
	//tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//
	//tLightInfo.Direction = { 1.f, -1.f, 1.f };
	//
	//if (FAILED(CLightMgr::GetInstance()->Ready_Light(m_pGraphicDev, &tLightInfo, 1)))
	//	return E_FAIL;

	return S_OK;
}



CStage* CStage::Create()
{
	CStage* pStage = new CStage;

	if (FAILED(pStage->Ready_Scene()))
	{
		Safe_Release(pStage);
		MSG_BOX("Stage Create Failed");
		return nullptr;
	}

	return pStage;
}

void CStage::Free()
{
	CScene::Free();
}
