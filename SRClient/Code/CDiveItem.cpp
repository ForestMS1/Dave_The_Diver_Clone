#include "CDiveItem.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CDiveDave.h"
#include "CManagement.h"
#include "CDiveItemDescUI.h"

CDiveItem::CDiveItem(_vec3 vOriginPos)
    : m_vOriginPos(vOriginPos)
{
}

CDiveItem::CDiveItem(const CDiveItem& rhs)
    : CGameObject(rhs)
{

}

CDiveItem::~CDiveItem()
{

}

void CDiveItem::FSM(const _float& fTimeDelta)
{
    switch (m_eCurState)
    {
    case ITEMSTATE::STARTDROP:
        CDiveItem::StartDrop(fTimeDelta);
        break;

    case ITEMSTATE::DROPPED:
        break;

    case ITEMSTATE::ACQUIRED:
        break;

    default:
        break;
    }
}

void CDiveItem::StartDrop(const _float& fTimeDelta)
{ 
    _vec3 vUpDir = { 0.f, 1.f, 0.f };
    _float v0 = 5.f;
    m_fAccTime += fTimeDelta;
    _float v = v0 - 9.8f * m_fAccTime;
    m_pTransformCom->Move_Pos(&vUpDir, v, fTimeDelta);

    _vec3 vCurPos;
    m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
    if (v < 0.f && vCurPos.y - m_vOriginPos.y < 0.01f)
    {
        m_eCurState = ITEMSTATE::DROPPED;
        return;
    }
}

// [LSY] derive 들은 이거 호출
void CDiveItem::GetItem()
{
    if (m_eCurState == ITEMSTATE::DROPPED)
        m_eCurState = ITEMSTATE::ACQUIRED;

    // 아이템 먹으면 desc ui 끄기
    if (auto pUI = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer")->Get_GameObjectFirst<CDiveItemDescUI>(L"DiveItemDescUI"))
    {
        pUI->Set_Render(false);
    }
}

void CDiveItem::Collision_With_DiveDave()
{
	// Test 레이어에있는 충돌체 리스트를 들고온다. 널체크
	if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_DiveDaveWithItem"))
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
                    if (pCollider->Get_Tag() == L"AABB_DiveDaveWithItem")
                        OnCollisionEnter(pCollider);
                }
                else if (m_bIsCollWithMe) //나랑 플레이어랑 충돌중이었다가 벗어났을 때
                    OnCollisionExit(pCollider);
			}
		}
	}
}

void	CDiveItem::OnCollisionEnter(CCollider* pCollider)
{
    m_bIsCollWithMe = true; // 나랑 플레이어랑 충돌중임
    CDiveDave* pDiveDave = static_cast<CDiveDave*>(pCollider->Get_VoidPtr());
    pDiveDave->Set_IsOnItem(true);
    pDiveDave->Set_CurOnItem(this);

    // [LSY] 아이템이 다 떨어졌을때 desc ui를 활성화 한다.
    if (m_eCurState == ITEMSTATE::DROPPED)
    {
        OpenItemDesc();
        //if (auto pUI = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer")->Get_GameObjectFirst<CDiveItemDescUI>(L"DiveItemDescUI"))
        //{
        //    pUI->Set_Title(L"아이템명");
        //    pUI->Set_Desc(L"아이템설명^^");
        //    pUI->Set_Render(true);
        //}
    }
}
void	CDiveItem::OnCollisionStay(CCollider* pCollider)
{

}
void	CDiveItem::OnCollisionExit(CCollider* pCollider)
{
    m_bIsCollWithMe = false;
    CDiveDave* pDiveDave = static_cast<CDiveDave*>(pCollider->Get_VoidPtr());
    pDiveDave->Set_IsOnItem(false);
    pDiveDave->Set_CurOnItem(nullptr);

    // [LSY] 아이템 벗어나면 desc ui 끄기
    if (auto pUI = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer")->Get_GameObjectFirst<CDiveItemDescUI>(L"DiveItemDescUI"))
    {
        pUI->Set_Render(false);
    }
}

void CDiveItem::Free()
{
    Safe_Release(m_pAABB);
}
