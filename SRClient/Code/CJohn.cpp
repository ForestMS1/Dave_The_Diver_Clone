#include "CJohn.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CJohnIdle.h"
CJohn::CJohn()
{
}

CJohn::CJohn(const CJohn& rhs)
	:CGameObject(rhs)
{
}

CJohn::~CJohn()
{
}

HRESULT CJohn::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	if (FAILED(Add_State()))
		return E_FAIL;

	Set_State(JOHNSTATE::IDLE);

	return S_OK;
}

_int CJohn::Update_GameObject(const _float& fTimeDelta)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);


	CGameObject::Update_GameObject(fTimeDelta);

	m_pFSM->Update_State(fTimeDelta);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

	return 0;
}

void CJohn::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	m_pFSM->LateUpdate_State(fTimeDelta);

}

void CJohn::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pFSM->Render_State();

	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CJohn::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT	CJohn::Add_State()
{
	m_pFSM = CFSM<CJohn, JOHNSTATE>::Create(this);
	if (m_pFSM == nullptr)
		return E_FAIL;

	m_pFSM->Add_State<CJohnIdle>(JOHNSTATE::IDLE);

	return S_OK;
}

void CJohn::AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size)
{
	m_fFrame += fSpeed * fTimeDelta;
	if (m_fFrame > size)
		m_fFrame = 0.f;
}

CJohn* CJohn::Create()
{
	CJohn* pBoss = new CJohn;
	if (FAILED(pBoss->Ready_GameObject()))
	{
		Safe_Release(pBoss);
		MSG_BOX("CJohn Create Failed");
		return nullptr;
	}
	return pBoss;

}

void CJohn::Free()
{
	//Safe_Release(m_pAABB);
	Safe_Release(m_pFSM);
	CGameObject::Free();
}
