#include "pch.h"
#include "CPlayerTestScene.h"
#include "CPlayerCam.h"
#include "CDSPlayer.h"
#include "CTerrain.h"
#include "CPlayerGun.h"
#include "CCameraMgr.h"
#include "CFreeCam.h"
#include "CGraphicDev.h"
#include "CLightMgr.h"
CPlayerTestScene::CPlayerTestScene()
{
}

CPlayerTestScene::~CPlayerTestScene()
{
}

HRESULT CPlayerTestScene::Ready_Scene()
{
	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
		return E_FAIL;

	if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
		return E_FAIL;

	if (FAILED(Ready_UI_Layer(L"UI_Layer")))
		return E_FAIL;

	if (FAILED(Ready_Light()))
		return E_FAIL;

	return S_OK;
}

_int CPlayerTestScene::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = CScene::Update_Scene(fTimeDelta);

	return iExit;
}

void CPlayerTestScene::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CPlayerTestScene::Render_Scene()
{
	ImGui::Begin("FPS");
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::Text("DT: %.5f", ImGui::GetIO().DeltaTime);
	ImGui::End();

	CCameraMgr::GetInstance()->Render_Camera();
}

HRESULT CPlayerTestScene::Ready_Environment_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	_vec3	vEye{ 0.f, 10.f, -10.f };
	_vec3	vAt{ 0.f, 0.f, 1.f };
	_vec3	vUp{ 0.f, 1.f, 0.f };

	// PlayerCam
	CCamera* pCamera = CPlayerCam::Create(&vEye, &vAt, &vUp);

	if (nullptr == pCamera)
		return E_FAIL;

	CCameraMgr::GetInstance()->Set_Camera(L"PlayerCam", pCamera);

	// TestCam1
	vEye = { 0.f, 30.f, -10.f };
	pCamera = CFreeCam::Create(&vEye, &vAt, &vUp);

	if (nullptr == pCamera)
		return E_FAIL;

	CCameraMgr::GetInstance()->Set_Camera(L"TestCam1", pCamera);

	// TestCam2
	vEye = { 0.f, 20.f, -20.f };
	pCamera = CFreeCam::Create(&vEye, &vAt, &vUp);

	if (nullptr == pCamera)
		return E_FAIL;

	CCameraMgr::GetInstance()->Set_Camera(L"TestCam2", pCamera);


	// Player
	pGameObject = CDSPlayer::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Player", pGameObject)))
		return E_FAIL;

	// PlayerGun
	pGameObject = CPlayerGun::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"PlayerGun", pGameObject)))
		return E_FAIL;

	// Terrain
	pGameObject = CTerrain::Create();

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
		return E_FAIL;

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });
	return S_OK;
}

HRESULT	CPlayerTestScene::Ready_GameLogic_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });
	return S_OK;

}
HRESULT	CPlayerTestScene::Ready_UI_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });
	return S_OK;
}

HRESULT	 CPlayerTestScene::Ready_Light()
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

	return S_OK;
}

CPlayerTestScene* CPlayerTestScene::Create()
{
	CPlayerTestScene* pInstance = new CPlayerTestScene;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		MSG_BOX("PlayerTestScene Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CPlayerTestScene::Free()
{
	CScene::Free();
}