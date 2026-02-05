#include "pch.h"
#include "CCube.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"

CCube::CCube()
	: CGameObject()
{
}

CCube::CCube(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CCube::~CCube()
{
}

HRESULT CCube::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->m_vInfo[INFO_POS] = {4.f, 4.f, 0.f};
	m_pTransformCom->m_vScale = { 1.f, 1.f, 1.f };

	return S_OK;
}

_int CCube::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_PRIORITY, this);

	

	return iExit;
}

void CCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	//_matrix		matCamWorld;

	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	//D3DXMatrixInverse(&matCamWorld, 0, &matCamWorld);

	//m_pTransformCom->Set_Pos(matCamWorld._41, matCamWorld._42 + 3.f, matCamWorld._43);


}

void CCube::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (CGameObject* pParent = Get_Parent())
	{
		if (CTransform* pTransform = dynamic_cast<CTransform*>(pParent->Get_Component(ID_DYNAMIC, L"Com_Transform")))
		{
			_matrix parentWorld = *pTransform->Get_World();
			_matrix myWorld = *m_pTransformCom->Get_World();
			_matrix world = myWorld * parentWorld;
			m_pTransformCom->Set_World(&world);
			pGraphicDev->SetTransform(D3DTS_WORLD, &world);
		}
	}
	else
	{
		pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	}

	
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(3);

	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CCube::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// buffer 
	pComponent = m_pBufferCom =
		dynamic_cast<Engine::CCubeTex*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CubeTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	// texture 
	pComponent = m_pTextureCom =
		dynamic_cast<Engine::CTexture*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_SkyBoxTexture"));
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


CCube* CCube::Create()
{
	CCube* pCube = new CCube;

	if (FAILED(pCube->Ready_GameObject()))
	{
		Safe_Release(pCube);
		MSG_BOX("pCube Create Failed");
		return nullptr;
	}

	return pCube;
}

void CCube::Free()
{
	CGameObject::Free();
}
