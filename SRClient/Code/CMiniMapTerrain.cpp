#include "pch.h"
#include "CMiniMapTerrain.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"

CMiniMapTerrain::CMiniMapTerrain()
	: CGameObject()
{
}

CMiniMapTerrain::CMiniMapTerrain(const CGameObject& rhs)
	: CGameObject(rhs)
{

}

CMiniMapTerrain::~CMiniMapTerrain()
{
}

HRESULT CMiniMapTerrain::Ready_GameObject()
{

	if (FAILED(Add_Component()))
		return E_FAIL;


	m_iTypeNum = 1;


	return S_OK;
}

_int CMiniMapTerrain::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CMiniMapTerrain::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CMiniMapTerrain::Render_GameObject()
{
	CGraphicDev::GetInstance()->Get_GraphicDev()->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	if (FAILED(Ready_Material()))
		return;

	CGraphicDev::GetInstance()->Get_GraphicDev()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	switch (m_iTypeNum) {
	case 0:
		m_pTextureCom->Set_Texture(0);
		break;
		
	case 1:
		m_pTextureCom->Set_Texture(1);

		break;

	case 2:

		break;
	default:

		break;

	}


	m_pBufferCom->Render_Buffer();
	CGraphicDev::GetInstance()->Get_GraphicDev()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	CGraphicDev::GetInstance()->Get_GraphicDev()->SetRenderState(D3DRS_POINTSIZE, true);


}

HRESULT CMiniMapTerrain::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// buffer 
	pComponent = m_pBufferCom =
		dynamic_cast<Engine::CMapTerrainTex*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_MiniMapTerrainTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	//Texture
	pComponent = m_pTextureCom =
		dynamic_cast<Engine::CTexture*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_MiniMapBlankTexture"));
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

HRESULT CMiniMapTerrain::Ready_Material()
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

CMiniMapTerrain* CMiniMapTerrain::Create()
{
	CMiniMapTerrain* pMiniMapTerrain = new CMiniMapTerrain();

	if (FAILED(pMiniMapTerrain->Ready_GameObject()))
	{
		Safe_Release(pMiniMapTerrain);
		MSG_BOX("pMiniMapTerrain Create Failed");
		return nullptr;
	}

	return pMiniMapTerrain;
}

void CMiniMapTerrain::Free()
{
	CGameObject::Free();
}
