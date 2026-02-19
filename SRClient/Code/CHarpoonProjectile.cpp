#include "CHarpoonProjectile.h"
#include "CManagement.h"
#include "CGraphicDev.h"
#include "CDiveDave.h"
#include "CHelper.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CProjectileReady.h"
#include "CProjectileFire.h"
#include "CProjectileHit.h"
#include "CProjectileReturn.h"
CHarpoonProjectile::CHarpoonProjectile()
{
}

CHarpoonProjectile::CHarpoonProjectile(const CHarpoonProjectile& rhs)
	: CGameObject(rhs)
{
}

CHarpoonProjectile::~CHarpoonProjectile()
{
}

HRESULT CHarpoonProjectile::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	_vec3 vScale = { 0.4f, 0.4f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	_float fWidth = 33.f;
	_float fHeight = 5.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);


	//-------------AABB Collider----------------
	_vec3 vExtents = { 1.0f, 1.0f, 1.0f };

	_vec3 vPos = { 00.0f, 0.0f, 0.0f };

	m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Projectile", this);
	//CColliderMgr::GetInstance()->Set_Render(true);


	//-------------State Create----------------
	m_mapState.insert({ PROJECTILESTATE::READY, CProjectileReady::Create(this) });
	m_mapState.insert({ PROJECTILESTATE::FIRE, CProjectileFire::Create(this) });
	m_mapState.insert({ PROJECTILESTATE::HIT, CProjectileHit::Create(this) });
	m_mapState.insert({ PROJECTILESTATE::RETURN, CProjectileReturn::Create(this) });

	Set_State(PROJECTILESTATE::READY);
	return S_OK;
}

_int CHarpoonProjectile::Update_GameObject(const _float& fTimeDelta)
{
	//if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DiveState::ATTACK)
	//	return 0;

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	// 충돌체 그룹에 넣어줘야한다.
	CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Ship", m_pAABB);
	m_pAABB->Transform(m_pTransformCom->Get_World());
	

	m_pState->Update_State(fTimeDelta);

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CHarpoonProjectile::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DiveState::ATTACK)
		return;

	m_pState->LateUpdate_State(fTimeDelta);

	CGameObject::LateUpdate_GameObject(fTimeDelta);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
}

void CHarpoonProjectile::Render_GameObject()
{
	if (m_eState != PROJECTILESTATE::RETURN)
	{
		if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DiveState::ATTACK)
			return;
	}

	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CHarpoonProjectile::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CAttackReadyArmTex, ID_STATIC>(L"Proto_AttackReadyArmBuffer", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 텍스쳐
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_HarpoonProjectileTexture", L"Com_Texture", &m_pTextureCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

CHarpoonProjectile* CHarpoonProjectile::Create()
{
	CHarpoonProjectile* pHarpoonProjectile = new CHarpoonProjectile;

	if (FAILED(pHarpoonProjectile->Ready_GameObject()))
	{
		Safe_Release(pHarpoonProjectile);
		MSG_BOX("HarpoonProjectile Create Failed");
		return nullptr;
	}
	return pHarpoonProjectile;
}

void CHarpoonProjectile::Free()
{
	for_each(m_mapState.begin(), m_mapState.end(), CDeleteMap());
	m_mapState.clear();

	CGameObject::Free();
	Safe_Release(m_pAABB);
}

void CHarpoonProjectile::Set_State(PROJECTILESTATE state)
{
	if (m_mapState[state] == m_pState)
		return;

	if (m_pState != nullptr)
		m_pState->Exit();

	m_pState = m_mapState[state];
	m_eState = state;

	m_pState->Enter();
}