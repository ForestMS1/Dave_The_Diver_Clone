#include "CDiveDaveBullet.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CFishGameObject.h"
#include "CJohn.h"
#include "CJohn2.h"
CDiveDaveBullet::CDiveDaveBullet(_vec3 vOrigin, _vec3 vDir, _float fZAngle)
    : m_vDir(vDir)
	, m_fZAngle(fZAngle)
	, m_vOrigin(vOrigin)
{
}

CDiveDaveBullet::CDiveDaveBullet(const CDiveDaveBullet& rhs)
    : CGameObject(rhs)
{
}

CDiveDaveBullet::~CDiveDaveBullet()
{
}

HRESULT CDiveDaveBullet::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(m_vOrigin.x, m_vOrigin.y, m_vOrigin.z);
	m_pTransformCom->Rotation(ROT_Z, m_fZAngle);

	_float fWidth = 15.f;
	_float fHeight = 7.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	vScale *= 0.1f;
	m_pTransformCom->Multiply_Scale(&vScale);

	//-------------AABB Collider----------------
	_vec3 vExtents = { 1.0f, 1.0f, 1.0f };

	_vec3 vPos = { 0.0f, 0.0f, 0.0f };

	m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_DiveDaveBullet", this);
	return S_OK;
}

_int CDiveDaveBullet::Update_GameObject(const _float& fTimeDelta)
{
	if (m_fLifeTime > 3.f)
	{
		m_bDead = true;
		return 0;
	}
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	// 충돌체 그룹에 넣어줘야한다.
	CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Ship", m_pAABB);
	m_pAABB->Transform(m_pTransformCom->Get_World());


	m_fLifeTime += fTimeDelta;
	m_pTransformCom->Move_Pos(&m_vDir, 10.f, fTimeDelta);

	CGameObject::Update_GameObject(fTimeDelta);

    return _int();
}

void CDiveDaveBullet::LateUpdate_GameObject(const _float& fTimeDelta)
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


	// [LSY] 총알이랑 물고기
	if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_FishesHitbox"))
	{
		for (auto& pCollider : *pColliders)
		{
				if (m_pAABB->Intersect(pCollider))
				{
					if (pCollider->Get_Tag() == L"AABB_FishHitbox")
					{
						reinterpret_cast<CFishGameObject*>(pCollider->Get_VoidPtr())->Damaged(1);
					}
				}
		}
	}
	// [KDS] 총알이랑 보스
	if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_JohnWithGuided"))
	{
		for (auto& pCollider : *pColliders)
		{
			if (m_pAABB->Intersect(pCollider))
			{
				if (pCollider->Get_Tag() == L"AABB_JohnWithGuided")
				{
					reinterpret_cast<CJohn*>(pCollider->Get_VoidPtr())->On_Hit(10.f);
					m_bDead = true;
				}
				if (pCollider->Get_Tag() == L"AABB_John2WithGuided")
				{
					reinterpret_cast<CJohn2*>(pCollider->Get_VoidPtr())->On_Hit(10.f);
					m_bDead = true;
				}
			}
		}
	}
}

void CDiveDaveBullet::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDiveDaveBullet::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 텍스쳐
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_BulletTexture", L"Com_Texture", &m_pTextureCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

CDiveDaveBullet* CDiveDaveBullet::Create(_vec3 vOrigin, _vec3 vDir, _float fZAngle)
{
	CDiveDaveBullet* pBullet = new CDiveDaveBullet(vOrigin, vDir, fZAngle);

	if (FAILED(pBullet->Ready_GameObject()))
	{
		Safe_Release(pBullet);
		MSG_BOX("DiveDaveBullet Create Failed");
		return nullptr;
	}
	return pBullet;
}

void CDiveDaveBullet::Free()
{
	Safe_Release(m_pAABB);
	CGameObject::Free();
}
