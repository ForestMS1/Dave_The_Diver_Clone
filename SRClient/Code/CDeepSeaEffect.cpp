#include "CDeepSeaEffect.h"
#include "CColliderMgr.h"
#include "CGraphicDev.h"
#include "CLightMgr.h"
#include "CParticleMgr.h"
CDeepSeaEffect::CDeepSeaEffect() : CGameObject()
{
}

CDeepSeaEffect::CDeepSeaEffect(const CGameObject& rhs) : CGameObject(rhs)
{
}

CDeepSeaEffect::~CDeepSeaEffect()
{
}

HRESULT CDeepSeaEffect::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	
	//CLightMgr::GetInstance()->Add_Light()
	_vec3 vPos{-30,-20,0};
	//m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CParticleMgr::GetInstance()->spwan_Particle(PARTICLE_BLOOMBUBBLE, vPos,15);
	return S_OK;
}




_int CDeepSeaEffect::Update_GameObject(const _float& fTimeDelta)
{


	
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);


	_vec3 vPos{};
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
	return iExit;
	



}

void CDeepSeaEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{


	CGameObject::LateUpdate_GameObject(fTimeDelta);





}

void CDeepSeaEffect::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pBufferCom->Render_Buffer();

	D3DXMATRIX matTmp;
	D3DXMatrixIdentity(&matTmp);
	pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);


	
}

HRESULT CDeepSeaEffect::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// Transform
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

}


CDeepSeaEffect* CDeepSeaEffect::Create( wstring _objectName)
{
	CDeepSeaEffect* pDeepSeaEffect = new CDeepSeaEffect( );

	if (FAILED(pDeepSeaEffect->Ready_GameObject()))
	{
		Safe_Release(pDeepSeaEffect);
		MSG_BOX("pDeepSeaEffect Create Failed");
		return nullptr;
	}

	return pDeepSeaEffect;
}

void CDeepSeaEffect::Free()
{

	CGameObject::Free();
}