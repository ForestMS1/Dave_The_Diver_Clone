#include "CHarpoonProjectile.h"
#include "CManagement.h"
#include "CGraphicDev.h"
#include "CDiveDave.h"
#include "CHelper.h"
#include "CColliderMgr.h"
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
	return S_OK;
}

_int CHarpoonProjectile::Update_GameObject(const _float& fTimeDelta)
{
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DiveState::ATTACK)
		return 0;

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	// 충돌체 그룹에 넣어줘야한다.
	CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Dive", m_pAABB);
	m_pAABB->Transform(m_pTransformCom->Get_World());
	

	if (m_eState == READY)
	{
		Set_ParentTransform();
		Rotate_ToMouse();
		Shot_ToMouse();
	}
	else if(m_eState == FIRE)
	{
		Go_ToDir(fTimeDelta);
	}
	Change_ProjectileState();

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	return iExit;
}

void CHarpoonProjectile::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DiveState::ATTACK)
		return;

	CGameObject::LateUpdate_GameObject(fTimeDelta);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
}

void CHarpoonProjectile::Render_GameObject()
{
	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() != DiveState::ATTACK)
		return;

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

void CHarpoonProjectile::Set_ParentTransform()
{
	_vec3 vParentPos;
	m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vParentPos);
	_vec3 vOffSet = { 0.f, 0.5f, 0.f };
	vOffSet.y *= m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->m_vScale.y;
	vParentPos += vOffSet;
	m_pTransformCom->Set_Pos(vParentPos.x, vParentPos.y, vParentPos.z);
}

void CHarpoonProjectile::Rotate_ToMouse()
{
	_vec3 vMousePos, vPlayerPos;

	CHelper::GetMousePointInWorld(&vMousePos);
	m_pTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	_vec3 vDir = vMousePos - vPlayerPos;

	float fLengthSq = vDir.x * vDir.x + vDir.y * vDir.y;

	// 너무 가까우면 회전 유지
	if (fLengthSq < 0.001f)
		return;

	float fDegree = D3DXToDegree(atan2f(vDir.y, vDir.x));

	if (vDir.x < 0.f)
		m_pTransformCom->m_vAngle.x = 180.f;
	else
		m_pTransformCom->m_vAngle.x = 0.f;

	m_pTransformCom->m_vAngle.z = fDegree;
}

void CHarpoonProjectile::Shot_ToMouse()
{
	_vec3 vMousePos, vProjectilePos;
	
	CHelper::GetMousePointInWorld(&vMousePos);
	m_pTransformCom->Get_Info(INFO_POS, &vProjectilePos);
	
	m_vDir = vMousePos - vProjectilePos;
}

void CHarpoonProjectile::Go_ToDir(const _float& fTimeDelta)
{
	if (m_fAccRange < m_fRange)
	{
		m_pTransformCom->Move_Pos(&m_vDir, m_fSpeed, fTimeDelta);
		m_fAccRange += m_fSpeed * fTimeDelta;
	}
}

void CHarpoonProjectile::Change_ProjectileState()
{
	if (m_fAccRange >= m_fRange)
	{
		m_fAccRange = 0.f;
		m_eState = READY;
		static_cast<CDiveDave*>(m_pParentGameObject)->Set_State(DiveState::IDLE);
	}


	// TODO
	// if(물고기랑 충돌했으면)
	// m_eState = HIT
	// static_cast<CDiveDave*>(m_pParentGameObject)->Set_FishCaught(true);
	// else if(최대 거리까지 왔는데도 물고기랑 충돌 못했으면)
	// m_eState = NONE_HIT
	// static_cast<CDiveDave*>(m_pParentGameObject)->Set_FishCaught(false);

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
	CGameObject::Free();
	Safe_Release(m_pAABB);
}
