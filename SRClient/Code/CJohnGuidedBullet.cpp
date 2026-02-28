#include "CJohnGuidedBullet.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CFishGameObject.h"
#include "CJohnGuidedBulletChase.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CManagement.h"
#include "CDiveDave.h"
#include "CJohn.h"
CJohnGuidedBullet::CJohnGuidedBullet(_vec3 vOrigin, _vec3 vDir, _float fZAngle)
	: m_vDir(vDir)
	, m_fZAngle(fZAngle)
	, m_vOrigin(vOrigin)
{
}

CJohnGuidedBullet::CJohnGuidedBullet(const CJohnGuidedBullet& rhs)
	: CGameObject(rhs)
{
}

CJohnGuidedBullet::~CJohnGuidedBullet()
{
}

HRESULT CJohnGuidedBullet::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	m_pTransformCom->Set_Pos(m_vOrigin.x, m_vOrigin.y, m_vOrigin.z);
	m_pTransformCom->Rotation(ROT_Z, m_fZAngle);

	_float fWidth = 28.f;
	_float fHeight = 17.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	vScale *= 0.5f;
	m_pTransformCom->Multiply_Scale(&vScale);

	//-------------AABB Collider----------------
	_vec3 vExtents = { 1.0f, 1.0f, 1.0f };

	_vec3 vPos = { 0.0f, 0.0f, 0.0f };

	m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_DiveDaveBullet", this);

	m_wsTexName = L"JohnBulletA";
	return S_OK;
}

_int CJohnGuidedBullet::Update_GameObject(const _float& fTimeDelta)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	// 충돌체 그룹에 넣어줘야한다.
	CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Ship", m_pAABB);
	m_pAABB->Transform(m_pTransformCom->Get_World());


	FSM(fTimeDelta);
	_vec3 rot = { 0.f, 0.f, m_fZAngle };
	m_pTransformCom->Set_Rotation(&rot);
	CGameObject::Update_GameObject(fTimeDelta);

	return _int();
}

void CJohnGuidedBullet::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	// Test 레이어에있는 충돌체 리스트를 들고온다. 널체크
	if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_Ship"))
	{
		// 충돌체 순회
		for (auto& pCollider : *pColliders)
		{
			// 내가 아닌것들과 체크
			if (m_pAABB != pCollider)
			{
				// 충돌체 끼리 충돌 체크
				if (m_pAABB->Intersect(pCollider))
				{
					// Some Logic
					// 

					if (pCollider->Get_Tag() == L"AABB_Boat")
					{
						CCollisionMgr::COLL_RECT_EX_INFO info;
						if (CCollisionMgr::GetInstance()->Collision_RectEx(m_pAABB, dynamic_cast<CAABB*>(pCollider), &info))
						{
							_vec3 vPos;
							m_pTransformCom->Get_Info(INFO_POS, &vPos);
							if (info.eDir == CCollisionMgr::DIR_DOWN)
							{
								vPos.y += info.fDistance;
								m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
							}
							else if (info.eDir == CCollisionMgr::DIR_UP)
							{
								vPos.y -= info.fDistance;
								m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
							}
							else if (info.eDir == CCollisionMgr::DIR_LEFT)
							{
								vPos.x -= info.fDistance;
								m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
							}
							else if (info.eDir == CCollisionMgr::DIR_RIGHT)
							{
								vPos.x += info.fDistance;
								m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
							}

							m_bDead = true;
							m_pTransformCom->Update_Component(fTimeDelta);
							m_pAABB->Transform(m_pTransformCom->Get_World());
						}
					}
				}
			}
		}
	}


	if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_DiveDaveWithItemBox"))
	{
		for (auto& pCollider : *pColliders)
		{
			if (m_pAABB->Intersect(pCollider))
			{
				if (pCollider->Get_Tag() == L"AABB_DiveDaveWithItemBox")
				{
					//reinterpret_cast<CFishGameObject*>(pCollider->Get_VoidPtr())->Damaged(1);
					if (m_eCurState == CHASE)
					{
						_float fWidth = 28.f;
						_float fHeight = 17.f;
						_float fAspect = fWidth + fHeight;
						fAspect /= 2.f;

						_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
						vScale *= 0.5f;
						m_pTransformCom->Multiply_Scale(&vScale);
						//---------------------------------------------------------------
						m_eCurState = EXLPOSION;
						//---------------------------------------------------------------
						fWidth = 51.f;
						fHeight = 51.f;
						fAspect = fWidth + fHeight;
						fAspect /= 2.f;

						vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
						vScale *= 5.f;
						m_pTransformCom->Multiply_Scale(&vScale);
					}
					if (m_eCurState == STOP)
					{
						if(static_cast<CDiveDave*>(pCollider->Get_VoidPtr())->Get_State() == DIVEDAVESTATE::MELEEATTACK)
						//---------------------------------------------------------------
						m_eCurState = RETURN;
						//---------------------------------------------------------------
					}
					if (m_eCurState == EXLPOSION)
					{
						static_cast<CDiveDave*>(pCollider->Get_VoidPtr())->On_Hit(10.f);
					}
					//m_bDead = true;
				}
			}
		}
	}

	if (m_eCurState == RETURN)
	{
		if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_JohnWithGuided"))
		{
			for (auto& pCollider : *pColliders)
			{
				if (m_pAABB->Intersect(pCollider))
				{
					if (pCollider->Get_Tag() == L"AABB_JohnWithGuided")
					{
						_float fWidth = 28.f;
						_float fHeight = 17.f;
						_float fAspect = fWidth + fHeight;
						fAspect /= 2.f;

						_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
						vScale *= 0.5f;
						m_pTransformCom->Multiply_Scale(&vScale);
						//---------------------------------------------------------------
						m_eCurState = EXLPOSION;
						//---------------------------------------------------------------
						fWidth = 51.f;
						fHeight = 51.f;
						fAspect = fWidth + fHeight;
						fAspect /= 2.f;

						vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
						vScale *= 5.f;
						m_pTransformCom->Multiply_Scale(&vScale);
					}
					if (m_eCurState == EXLPOSION)
					{
						static_cast<CJohn*>(pCollider->Get_VoidPtr())->On_Hit(50.f);
					}
				}
			}
		}
	}
}

void CJohnGuidedBullet::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());



	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_wsTexName))
	{
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at((_uint)m_fFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}


	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CJohnGuidedBullet::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CJohnGuidedBullet::FSM(const _float& fTimeDelta)
{
	switch (m_eCurState)
	{
	case CHASE:
		m_wsTexName = L"JohnBulletA";
		AddFrame(fTimeDelta, 5.f, 2, true);
		Chase(fTimeDelta);
		break;
	case STOP:
		m_wsTexName = L"JohnBulletB";
		AddFrame(fTimeDelta, 5.f, 2, true);
		StopReady(fTimeDelta);
		break;
	case RETURN:
		m_wsTexName = L"JohnBulletA";
		AddFrame(fTimeDelta, 5.f, 2, true);
		Return(fTimeDelta);
		break;
	case EXLPOSION:
		m_wsTexName = L"JohnBulletExplosion";
		AddFrame(fTimeDelta, 5.f, 9, false);
		Explosion(fTimeDelta);
		break;
	default:
		break;
	}
}

void CJohnGuidedBullet::Chase(const _float& fTimeDelta)
{
	m_fChaseTime += fTimeDelta;

	if (m_fChaseTime > 3.f)
	{
		m_eCurState = CJohnGuidedBullet::STATE::STOP;
		m_fChaseTime = 0.f;
		return;
	}
	m_pTargetTransform = static_cast<CTransform*>
		(CManagement::GetInstance()->Get_FirstObjectComponent(ID_DYNAMIC, L"0_GameLogic_Layer", L"DiveDave", L"Com_Transform"));
	if (m_pTargetTransform == nullptr)
		return;

	_vec3 vTargetPos;
	m_pTargetTransform->Get_Info(INFO_POS, &vTargetPos);
	m_pTransformCom->Chase_Target(&vTargetPos, 5.f, fTimeDelta);


	_vec3 vCurPos, vNorToTarget;
	m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
	vNorToTarget = vTargetPos - vCurPos;
	D3DXVec3Normalize(&vNorToTarget, &vNorToTarget);

	_vec3 vAxisX = { 1.f, 0.f, 0.f };

	_float dot = vAxisX.x * vNorToTarget.x + vAxisX.y * vNorToTarget.y;
	_float cross = vAxisX.x * vNorToTarget.y - vAxisX.y * vNorToTarget.x;
	_float fAngle = D3DXToDegree(atan2(cross, dot));
	m_fZAngle = fAngle;
}

void CJohnGuidedBullet::StopReady(const _float& fTimeDelta)
{
	m_fExplosionReadyTime += fTimeDelta;
	if (m_fExplosionReadyTime > 3.f)
	{
		//m_eCurState = CJohnGuidedBullet::STATE::RETURN;
		m_fExplosionReadyTime = 0.f;
		_float fWidth = 28.f;
		_float fHeight = 17.f;
		_float fAspect = fWidth + fHeight;
		fAspect /= 2.f;

		_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
		vScale *= 0.5f;
		m_pTransformCom->Multiply_Scale(&vScale);
		//---------------------------------------------------------------
		m_eCurState = EXLPOSION;
		//---------------------------------------------------------------
		fWidth = 51.f;
		fHeight = 51.f;
		fAspect = fWidth + fHeight;
		fAspect /= 2.f;

		vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
		vScale *= 5.f;
		m_pTransformCom->Multiply_Scale(&vScale);
		return;
	}



}

void CJohnGuidedBullet::Return(const _float& fTimeDelta)
{
	m_pReturnTargetTransform = static_cast<CTransform*>
		(CManagement::GetInstance()->Get_FirstObjectComponent(ID_DYNAMIC, L"0_GameLogic_Layer", L"John", L"Com_Transform"));
	if (m_pReturnTargetTransform == nullptr)
		return;


	_vec3 vTargetPos;
	m_pReturnTargetTransform->Get_Info(INFO_POS, &vTargetPos);
	m_pTransformCom->Chase_Target(&vTargetPos, 5.f, fTimeDelta);


	_vec3 vCurPos, vNorToTarget;
	m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
	vNorToTarget = vTargetPos - vCurPos;
	D3DXVec3Normalize(&vNorToTarget, &vNorToTarget);

	_vec3 vAxisX = { 1.f, 0.f, 0.f };

	_float dot = vAxisX.x * vNorToTarget.x + vAxisX.y * vNorToTarget.y;
	_float cross = vAxisX.x * vNorToTarget.y - vAxisX.y * vNorToTarget.x;
	_float fAngle = D3DXToDegree(atan2(cross, dot));
	m_fZAngle = fAngle;
}

void CJohnGuidedBullet::Explosion(const _float& fTimeDelta)
{
	m_fExplosionTime += fTimeDelta;
	if (m_fExplosionTime > 1.f)
	{
		m_bDead = true;
		return;
	}

}


CJohnGuidedBullet* CJohnGuidedBullet::Create(_vec3 vOrigin, _vec3 vDir, _float fZAngle)
{
	CJohnGuidedBullet* pBullet = new CJohnGuidedBullet(vOrigin, vDir, fZAngle);

	if (FAILED(pBullet->Ready_GameObject()))
	{
		Safe_Release(pBullet);
		MSG_BOX("DiveDaveBullet Create Failed");
		return nullptr;
	}
	return pBullet;
}

void CJohnGuidedBullet::Free()
{
	Safe_Release(m_pAABB);
	CGameObject::Free();
}


void CJohnGuidedBullet::AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size, _bool loop)
{
	m_fFrame += fSpeed * fTimeDelta;
	if (loop)
	{
		if (m_fFrame > size)
			m_fFrame = 0.f;
	}
	else
	{
		if (m_fFrame > size)
			m_fFrame = size - 1;
	}
}
