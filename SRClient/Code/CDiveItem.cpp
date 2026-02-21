#include "CDiveItem.h"

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

void CDiveItem::Free()
{
    CGameObject::Free();
}
