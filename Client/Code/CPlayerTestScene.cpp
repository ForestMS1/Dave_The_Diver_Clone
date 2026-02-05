#include "pch.h"
#include "CPlayerTestScene.h"
#include "CPlayerCam.h"
#include "CDSPlayer.h"
#include "CTerrain.h"

CPlayerTestScene::CPlayerTestScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CPlayerTestScene::~CPlayerTestScene()
{
}

HRESULT CPlayerTestScene::Ready_Scene()
{
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
	pGameObject = CPlayerCam::Create(m_pGraphicDev, &vEye, &vAt, &vUp);

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"PlayerCam", pGameObject)))
		return E_FAIL;

	// Player
	pGameObject = CDSPlayer::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Player", pGameObject)))
		return E_FAIL;

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
		return E_FAIL;

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });
	return S_OK;
}

HRESULT	CPlayerTestScene::Ready_GameLogic_Layer(std::wstring_view svLayerTag)
{
	return S_OK;

}
HRESULT	CPlayerTestScene::Ready_UI_Layer(std::wstring_view svLayerTag)
{
	return S_OK;
}

HRESULT	 CPlayerTestScene::Ready_Light()
{
	return S_OK;

}

CPlayerTestScene* CPlayerTestScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerTestScene* pInstance = new CPlayerTestScene(pGraphicDev);
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