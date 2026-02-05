#include "pch.h"
#include "CMapEditor.h"

#include "CProtoMgr.h"
#include "CDynamicCamera.h"
 
#include "CLightMgr.h"
#include "CMapEditorTerrain.h"


CMapEditor::CMapEditor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CMapEditor::~CMapEditor()
{
}

HRESULT CMapEditor::Ready_Scene()
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

_int CMapEditor::Update_Scene(const _float& fTimeDelta)
{
	_int		iExit = CScene::Update_Scene(fTimeDelta);

	return iExit;
}

void CMapEditor::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CMapEditor::Render_Scene()
{
	// debug ¿ë Ãâ·Â
}

HRESULT CMapEditor::Ready_Environment_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	_vec3	vEye{ 0.f, 100.f, 0.f };
	_vec3	vAt{ 50.f, -30.f, 50.f };
	_vec3	vUp{ 0.f, 1.f, 0.f };

	// DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &vEye, &vAt, &vUp);

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"DynamicCamera", pGameObject)))
		return E_FAIL;

	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

HRESULT CMapEditor::Ready_GameLogic_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;


	// MapEditorTerrain
	pGameObject = CMapEditorTerrain::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"MapEditorTerrain", pGameObject)))
		return E_FAIL;


	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

HRESULT CMapEditor::Ready_UI_Layer(std::wstring_view svLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;



	m_mapLayer.insert({ std::wstring(svLayerTag), pLayer });

	return S_OK;
}

HRESULT CMapEditor::Ready_Light()
{
	D3DLIGHT9	tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tLightInfo.Direction = { 1.f, -1.f, 1.f };

	if (FAILED(CLightMgr::GetInstance()->Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
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



CMapEditor* CMapEditor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMapEditor* pMapEditor = new CMapEditor(pGraphicDev);

	if (FAILED(pMapEditor->Ready_Scene()))
	{
		Safe_Release(pMapEditor);
		MSG_BOX("CMapEditor Create Failed");
		return nullptr;
	}

	return pMapEditor;
}

void CMapEditor::Free()
{
	CScene::Free();
}
