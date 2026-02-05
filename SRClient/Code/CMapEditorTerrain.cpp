#include "pch.h"
#include "CMapEditorTerrain.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CMapEditorTerrain::CMapEditorTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMapEditorTerrain::CMapEditorTerrain(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CMapEditorTerrain::~CMapEditorTerrain()
{
}

HRESULT CMapEditorTerrain::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;



	return S_OK;
}

_int CMapEditorTerrain::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CMapEditorTerrain::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CMapEditorTerrain::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());


	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if (FAILED(Ready_Material()))
		return;

	//m_pTextureCom->Set_Texture(0);
	
	m_pBufferCom->Render_Buffer();



	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

HRESULT CMapEditorTerrain::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// buffer 
	pComponent = m_pBufferCom =
		dynamic_cast<Engine::CMapTerrainTex*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_MapTerrainTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	// texture 
	pComponent = m_pTextureCom =
		dynamic_cast<Engine::CTexture*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_TerrainTexture"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	// Transform
	pComponent = m_pTransformCom =
		dynamic_cast<Engine::CTransform*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

HRESULT CMapEditorTerrain::Ready_Material()
{
	D3DMATERIAL9			tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);

	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	tMtrl.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}

CMapEditorTerrain* CMapEditorTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMapEditorTerrain* pMapEditorTerrian = new CMapEditorTerrain(pGraphicDev);

	if (FAILED(pMapEditorTerrian->Ready_GameObject()))
	{
		Safe_Release(pMapEditorTerrian);
		MSG_BOX("pMapEditorTerrian Create Failed");
		return nullptr;
	}

	return pMapEditorTerrian;
}

void CMapEditorTerrain::Free()
{
	CGameObject::Free();
}
