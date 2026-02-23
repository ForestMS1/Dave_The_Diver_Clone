#include "pch.h"
#include "CBackGroundSea.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
CBackGroundSea::CBackGroundSea()
	: CGameObject()
{
}

CBackGroundSea::CBackGroundSea(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CBackGroundSea::~CBackGroundSea()
{
}

HRESULT CBackGroundSea::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;



	return S_OK;
}

_int CBackGroundSea::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CBackGroundSea::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CBackGroundSea::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();



	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());



	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	D3DXMATRIX matTmp;
	D3DXMatrixIdentity(&matTmp);
	pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

}

HRESULT CBackGroundSea::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;
	// Buffer
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;
	// texture
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_BackGroundSea", L"Com_Texture", &m_pTextureCom))))
		return E_FAIL;

	// Transform
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;



	return S_OK;
}


CBackGroundSea* CBackGroundSea::Create()
{
	CBackGroundSea* pBackGround = new CBackGroundSea;

	if (FAILED(pBackGround->Ready_GameObject()))
	{
		Safe_Release(pBackGround);
		MSG_BOX("CBackGroundSea Create Failed");
		return nullptr;
	}

	return pBackGround;
}

void CBackGroundSea::Free()
{
	CGameObject::Free();
}
