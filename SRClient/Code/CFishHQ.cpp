#include "pch.h"
#include "CFishHQ.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CManagement.h"
#include "FishInclude.h"
#include "CDiveDave.h"

CFishHQ::CFishHQ()
    : CGameObject()
{
}

CFishHQ::~CFishHQ()
{
}

HRESULT CFishHQ::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_fTimer = 0.f;
    m_iCnt = 0;
    return S_OK;
}

_int CFishHQ::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
    m_fTimer += fTimeDelta;

    if (m_fTimer > 0.1f)
    {
        if (m_iCnt < 5)
        {
            ++m_iCnt;
            {
                float randX = rand() % 10;
                float randY = rand() % 5;
                if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
                {
                    Fish::AddLayer_BlueTang(pLayer, randX, randY, 0.3f, this);
                   
                }
            }

            {
                float randX = rand() % 10;
                float randY = rand() % 5;
                if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
                {
                    Fish::AddLayer_TitanTriggerfish(pLayer, randX, randY, 0.3f, this);
                   
                }
            }

            {
                float randX = rand() % 10;
                float randY = rand() % 5;
                if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
                {
                    Fish::AddLayer_YellowTang(pLayer, randX, randY, 0.3f, this);
                   
                }
            }

            {
                float randX = rand() % 10;
                float randY = rand() % 5;
                if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
                {
                    Fish::AddLayer_Clownfish(pLayer, randX, randY, 0.3f, this);
                   
                }
            }

            {
                float randX = rand() % 10;
                float randY = rand() % 5;
                if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
                {
                    Fish::AddLayer_SmallspottedDart(pLayer, randX, randY, 0.3f, this);
                   
                }
            }

            {
                float randX = rand() % 10;
                float randY = rand() % 5;
                if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
                {
                    Fish::AddLayer_YellowbackFusilier(pLayer, randX, randY, 0.3f, this);
                   
                }
            }
        }
        
        m_fTimer = 0.f;
    }



    return iExit;
}

void CFishHQ::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    // 데이브가 물고기 디텍스박스 충돌 체크
    if (auto pDaveExistsColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_DiveDaveWithItemBox"))
    {
        for (auto pDaveCollider : *pDaveExistsColliders)
        {
            if (pDaveCollider->Get_Tag() == L"AABB_DiveDaveWithItemBox")
            {
                // 데이브와 물고기 디텍트 박스 충돌체크
                if (auto pFishDetectBoxColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_FishesDetectBox"))
                {
                    for (auto pFishDetectBoxCollider : *pFishDetectBoxColliders)
                    {
                        if (pFishDetectBoxCollider->Get_Tag() == L"AABB_FishDetectbox")
                        {
                            if (pDaveCollider->Intersect(pFishDetectBoxCollider))
                            {
                                CDiveDave* pDave = reinterpret_cast<CDiveDave*>(pDaveCollider->Get_VoidPtr());
                                CFishGameObject* pFish = reinterpret_cast<CFishGameObject*>(pFishDetectBoxCollider->Get_VoidPtr());
                                Fish::FISH_TYPE eFishType = pFish->Get_FishType();
                                if (eFishType == Fish::FT_NORMAL)
                                {
                                    CTransform* pDaveTransform = pDave->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform");
                                    _vec3 vDavePos;
                                    pDaveTransform->Get_Info(INFO_POS, &vDavePos);
                                    pFish->RunFrom(&vDavePos);
                                }
                                else if (eFishType == Fish::FT_AGRESSIVE)
                                {
                                    CTransform* pDaveTransform = pDave->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform");
                                    _vec3 vDavePos;
                                    pDaveTransform->Get_Info(INFO_POS, &vDavePos);
                                    pFish->AttackTo(&vDavePos);
                                }
                            }
                        }
                    }
                }

                // 데이브와 물고기 칼질
                // TODO: 마우스 클릭시점이 아니라 공격시점으로
                if (auto pFishHitBoxColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_FishesHitbox"))
                {
                    for (auto pFishHitBoxCollider : *pFishHitBoxColliders)
                    {
                        if (pFishHitBoxCollider->Get_Tag() == L"AABB_FishHitbox")
                        {
                            if (pDaveCollider->Intersect(pFishHitBoxCollider))
                            {
                                CDiveDave* pDave = reinterpret_cast<CDiveDave*>(pDaveCollider->Get_VoidPtr());
                                CFishGameObject* pFish = reinterpret_cast<CFishGameObject*>(pFishHitBoxCollider->Get_VoidPtr());
                                if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
                                {
                                    pFish->Damaged(1);
                                    break;
                                }

                                if (pFish->Get_FishType() == Fish::FT_AGRESSIVE)
                                {
                                    if (pFish->Get_FishState() == Fish::FS_ATTACKTO)
                                    {
                                        // TODO: 데이브쪽에서 무적시간 존재해야함
                                        //pDave->On_Hit(1);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
    }
}

void CFishHQ::Render_GameObject()
{
}

HRESULT CFishHQ::Ready_Component()
{
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CFishHQ* CFishHQ::Create()
{
    CFishHQ* pHQ = new CFishHQ;

    if (FAILED(pHQ->Ready_GameObject()))
    {
        Safe_Release(pHQ);
        MSG_BOX("pHQ Create Failed");
        return nullptr;
    }

    return pHQ;
}

void CFishHQ::Free()
{
    CGameObject::Free();
}
