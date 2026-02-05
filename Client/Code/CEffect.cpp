#include "pch.h"
#include "CEffect.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CEffect::CEffect(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CEffect::~CEffect()
{
}

HRESULT CEffect::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(_float(rand() % 20), 0.f, _float(rand() % 20));

	m_fFrame = 0.f;
	m_fDelTimer = 0.f;

	return S_OK;
}

_int CEffect::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_fDelTimer += fTimeDelta;

	m_fFrame += 90.f * fTimeDelta;

	if (90.f < m_fFrame)
		m_fFrame = 0.f;

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	if (m_fDelTimer > 1.f)
	{
		if (rand() % 2)
		{
			return OBJ_DEAD;
		}
		m_fDelTimer = 0.f;
	}

	return iExit;
}

void CEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_matrix		matBill, matWorld, matView;

	matWorld = *m_pTransformCom->Get_World();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);

	// y축 회전만 제거
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	// 주의 할 것
	matWorld = matBill * matWorld;

	m_pTransformCom->Set_World(&matWorld);

	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);

	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CEffect::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CEffect::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// buffer 
	pComponent = m_pBufferCom =
		dynamic_cast<Engine::CRcTex*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	// texture 
	pComponent = m_pTextureCom =
		dynamic_cast<Engine::CTexture*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_EffectTexture"));
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


CEffect* CEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect* pBackGround = new CEffect(pGraphicDev);

	if (FAILED(pBackGround->Ready_GameObject()))
	{
		Safe_Release(pBackGround);
		MSG_BOX("pBackGround Create Failed");
		return nullptr;
	}

	return pBackGround;
}

void CEffect::Free()
{
	CGameObject::Free();
}
