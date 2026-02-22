#include "pch.h"
#include "CTriggerAABB.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CColliderMgr.h"

CTriggerAABB::CTriggerAABB(wstring_view svColliderTag, wstring_view svColliderGroupTag)
    : CGameObject()
    //, m_fPosX(fPosX)
    //, m_fPosY(fPosY)
    , m_pTransformCom(nullptr)
    , m_pAABB(nullptr)
    , m_sColliderTag(svColliderTag)
    , m_sColliderGroupTag(svColliderGroupTag)
    , m_funcOnEnter(nullptr)
    , m_funcOnExit(nullptr)
    , m_funcOnStay(nullptr)
{
}

CTriggerAABB::~CTriggerAABB()
{
}


HRESULT		CTriggerAABB::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vPos = { 0.0f, 0.0f, 0.0f };
    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };
    //m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    
    m_pAABB = CAABB::Create(&vPos, &vExtents, m_sColliderTag, this);
    return S_OK;
}

_int		CTriggerAABB::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    CColliderMgr::GetInstance()->AddColliderGroup(m_sColliderGroupTag, m_pAABB);
    m_pAABB->Transform(m_pTransformCom->Get_World());

    return iExit;
}

void		CTriggerAABB::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(m_sColliderGroupTag))
    {
        //list<CCollider*> copyTriggeredList = m_TriggeredList;
        // 만약 트리거드 리스트에 없으면
        // on enter 호출해주고 넣어준다.

        //만약 트리거드 리스트에 있는데 충돌하고 있으면
        // 스테이 호출해준다.

        //만약 트리거드 리스트에 있는데 충돌안왔으면 제거하면서 exit 해준다.


        for (auto& pTrigger : m_mapTrigger)
        {
            pTrigger.second = false;
        }

        // 충돌체 순회
        for (auto& pCollider : *pColliders)
        {
            // 내가 아닌것들과 체크
            if (m_pAABB != pCollider)
            {
                // 충돌체 끼리 충돌 체크
                if (m_pAABB->Intersect(pCollider))
                {
                    auto iter = m_mapTrigger.find(pCollider);
                    if (iter == m_mapTrigger.end()) {
                        m_funcOnEnter(pCollider);
                        if (m_funcOnEnter)
                        {
                            m_mapTrigger.insert({ pCollider, true });
                        }
                    }
                    else
                    {
                        iter->second = true;
                        if (m_funcOnStay)
                        {
                            m_funcOnStay(iter->first);
                        }
                    }
                }
            }
        }

        vector<CCollider*> delVec;
        for (auto& pTrigger : m_mapTrigger)
        {
            if (!pTrigger.second)
            {
                delVec.push_back(pTrigger.first);
                if (m_funcOnExit)
                {
                    //m_mapTrigger.erase(pTrigger.first);
                    m_funcOnExit(pTrigger.first);
                }
            }
        }

        for (auto& pDel : delVec)
        {
            m_mapTrigger.erase(pDel);
        }


    }
}

void		CTriggerAABB::Render_GameObject()
{

}

HRESULT			CTriggerAABB::Ready_Component()
{
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CTriggerAABB* CTriggerAABB::Create(wstring_view svColliderTag, wstring_view svColliderGroupTag)
{
    CTriggerAABB* pAABBTrigger = new CTriggerAABB{ svColliderTag , svColliderGroupTag };

    if (FAILED(pAABBTrigger->Ready_GameObject()))
    {
        Safe_Release(pAABBTrigger);
        MSG_BOX("pAABBTrigger Create Failed");
        return nullptr;
    }

    return pAABBTrigger;
}

void CTriggerAABB::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}