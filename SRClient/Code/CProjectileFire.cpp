#include "CProjectileFire.h"
#include "CHarpoonProjectile.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CDiveDave.h"
#include "CFishGameObject.h"
CProjectileFire::CProjectileFire(CHarpoonProjectile* pOwner)
    : CBaseState<CHarpoonProjectile>(pOwner)
{
}

CProjectileFire::~CProjectileFire()
{
}

void CProjectileFire::Enter()
{
}

void CProjectileFire::Input(const _float& fTimeDelta)
{
}

_int CProjectileFire::Update_State(const _float& fTimeDelta)
{
	Go_ToDir(fTimeDelta);
    return 0;
}

void CProjectileFire::LateUpdate_State(const _float& fTimeDelta)
{
	CHarpoonProjectile* pProjectile = static_cast<CHarpoonProjectile*>(m_pOwner);

	// Test 레이어에있는 충돌체 리스트를 들고온다. 널체크
	if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_Ship"))
	{
		// 충돌체 순회
		for (auto& pCollider : *pColliders)
		{
			// 내가 아닌것들과 체크
			if (pProjectile->m_pAABB != pCollider)
			{
				// 충돌체 끼리 충돌 체크
				if (pProjectile->m_pAABB->Intersect(pCollider))
				{
					// Some Logic
					// 

					if (pCollider->Get_Tag() == L"AABB_Boat")
					{
						CCollisionMgr::COLL_RECT_EX_INFO info;
						if (CCollisionMgr::GetInstance()->Collision_RectEx(pProjectile->m_pAABB, dynamic_cast<CAABB*>(pCollider), &info))
						{
							_vec3 vPos;
							pProjectile->m_pTransformCom->Get_Info(INFO_POS, &vPos);
							if (info.eDir == CCollisionMgr::DIR_DOWN)
							{
								vPos.y += info.fDistance;
								pProjectile->m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
							}
							else if (info.eDir == CCollisionMgr::DIR_UP)
							{
								vPos.y -= info.fDistance;
								pProjectile->m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
							}
							else if (info.eDir == CCollisionMgr::DIR_LEFT)
							{
								vPos.x -= info.fDistance;
								pProjectile->m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
							}
							else if (info.eDir == CCollisionMgr::DIR_RIGHT)
							{
								vPos.x += info.fDistance;
								pProjectile->m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
							}

							m_bIsHitFish = true;
							pProjectile->m_pCaughtFish = static_cast<CGameObject*>(pCollider->Get_VoidPtr()); // 충돌한 물고기의 포인터 들고 옴
							pProjectile->m_pTransformCom->Update_Component(fTimeDelta);
							pProjectile->m_pAABB->Transform(pProjectile->m_pTransformCom->Get_World());
						}
					}
				}
			}
		}
	}

	// [LSY] 작살하고 물고기
	if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_FishesHitbox"))
	{
		for (auto& pCollider : *pColliders)
		{
			if (pProjectile->m_pAABB->Intersect(pCollider))
			{
				if (pCollider->Get_Tag() == L"AABB_FishHitbox")
				{
					m_bIsHitFish = true;
					auto pFish = static_cast<CFishGameObject*>(pCollider->Get_VoidPtr()); // 충돌한 물고기의 포인터 들고 옴
					pProjectile->m_pCaughtFish = pFish;
					pProjectile->m_pTransformCom->Update_Component(fTimeDelta);
					pProjectile->m_pAABB->Transform(pProjectile->m_pTransformCom->Get_World());
					pFish->Stop();
					break;
				}
			}
		}
	}

	if (m_bIsHitFish)
	{
		pProjectile->Set_State(PROJECTILESTATE::HIT);
		static_cast<CDiveDave*>(pProjectile->m_pParentGameObject)->Set_FishCaught(true);
	}
	else if(pProjectile->m_fAccRange >= pProjectile->m_fRange)
	{
		pProjectile->Set_State(PROJECTILESTATE::RETURN);
	}
}

void CProjectileFire::Render_State()
{
}

void CProjectileFire::Exit()
{
	Clear();
}

void CProjectileFire::Clear()
{
	CHarpoonProjectile* pProjectile = static_cast<CHarpoonProjectile*>(m_pOwner);
	pProjectile->m_fAccRange = 0.f;
	m_bIsHitFish = false;
}

CProjectileFire* CProjectileFire::Create(CHarpoonProjectile* pOwner)
{
    CProjectileFire* pState = new CProjectileFire(pOwner);

    return pState;
}

void CProjectileFire::Free()
{
}


void CProjectileFire::Go_ToDir(const _float& fTimeDelta)
{
	CHarpoonProjectile* pProjectile = static_cast<CHarpoonProjectile*>(m_pOwner);
	if (pProjectile->m_fAccRange < pProjectile->m_fRange)
	{
		pProjectile->m_pTransformCom->Move_Pos(&pProjectile->m_vDir, pProjectile->m_fSpeed, fTimeDelta);
		pProjectile->m_fAccRange += pProjectile->m_fSpeed * fTimeDelta;
	}
}