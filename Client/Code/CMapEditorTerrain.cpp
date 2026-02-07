#include "pch.h"
#include "CMapEditorTerrain.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"

CMapEditorTerrain::CMapEditorTerrain()
	: CGameObject()
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

	if (m_bRender) {

		CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	}

	return iExit;
}

void CMapEditorTerrain::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (m_bRender) {
		CGameObject::LateUpdate_GameObject(fTimeDelta);
	}
}

void CMapEditorTerrain::Render_GameObject()
{
	if (m_bRender) {
		CGraphicDev::GetInstance()->Get_GraphicDev()->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());


		//CGraphicDev::GetInstance()->Get_GraphicDev()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		CGraphicDev::GetInstance()->Get_GraphicDev()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);


		if (FAILED(Ready_Material()))
			return;

		m_pTextureCom->Set_Texture(1);

		m_pBufferCom->Render_Buffer();



		CGraphicDev::GetInstance()->Get_GraphicDev()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);


		//CGraphicDev::GetInstance()->Get_GraphicDev()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	

}

HRESULT CMapEditorTerrain::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// buffer 
	pComponent = m_pBufferCom =
		dynamic_cast<Engine::CMapTerrainTex*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_MapTerrainTex"));
	if (nullptr == pComponent) {
		MSG_BOX("CMapTerrainTex Proto_MapTerrainTex Clone_Prototype Failed");
		return E_FAIL;
	}
	

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom =
		dynamic_cast<Engine::CTexture*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_TerrainTexture"));
	if (nullptr == pComponent) {
		MSG_BOX("CMapTerrainTex Proto_TerrainTexture Clone_Prototype Failed");
		return E_FAIL;
	}
		

	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	// Transform
	pComponent = m_pTransformCom =
		dynamic_cast<Engine::CTransform*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent) {
		MSG_BOX("CMapTerrainTex Proto_Transform Clone_Prototype Failed");
		return E_FAIL;
	}
	

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

	CGraphicDev::GetInstance()->Get_GraphicDev()->SetMaterial(&tMtrl);

	return S_OK;
}

CMapEditorTerrain* CMapEditorTerrain::Create()
{
	CMapEditorTerrain* pMapEditorTerrian = new CMapEditorTerrain();

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
