#include "pch.h"
#include "CShipDave.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGameMemMgr.h"
#include "CTimerMgr.h"
#include "CManagement.h"
#include "CDaveConversation.h"

CShipDave::CShipDave()
    : CGameObject()
{
}

CShipDave::CShipDave(const CShipDave& rhs)
    : CGameObject(rhs)
{
}

CShipDave::~CShipDave()
{
}

HRESULT CShipDave::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vExtents = { 0.3f, 0.7f, 0.3f };

    _vec3 vPos = { 00.0f, 0.0f, 0.0f };

    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Dave", this);

    m_pTransformCom->Set_Pos(-5.f, -2.6f, 0.f);
  
    m_iFrame = 0;
    m_fAccFrameDelta = 0.f;
    m_fConvAppearTimer = 0.f;
    m_fTransitionTimer = 0.f;
    m_sCurrentMotion = L"Tex_ShipDave_Idle";
    m_bSeeRight = true;
    m_bTalking = false;
    m_bDiveReady = false;

    return S_OK;
}

_int CShipDave::Update_GameObject(const _float& fTimeDelta)
{
    
    if (m_bDiveReady)
    {
        Motion_Change(L"Tex_ShipDave_DiveReady");
    }
    else
    {
        Key_Input(fTimeDelta);
    }
   
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    m_fAccFrameDelta += fTimeDelta;

    if (m_fAccFrameDelta > 0.2f)
    {
        ++m_iFrame;
        m_fAccFrameDelta = 0;
    }
   
   auto zz = CAssetMgr::GetInstance()->Get_Asset(m_sCurrentMotion)->size();
    if (CAssetMgr::GetInstance()->Get_Asset(m_sCurrentMotion)->size() <= m_iFrame)
    {
        if (L"Tex_ShipDave_DiveReady" == m_sCurrentMotion)
        {
            m_bDiveReady = false;
            m_iFrame = CAssetMgr::GetInstance()->Get_Asset(m_sCurrentMotion)->size() - 1;
        }
        else
        {
            m_iFrame = 0;
        }
        
    }

    

    string s = "Frame" + ::to_string(m_iFrame);
    ImGui::Text(s.c_str());
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    // 충돌체 그룹에 넣어줘야한다.
    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_ShipDave", m_pAABB);
    m_pAABB->Transform(m_pTransformCom->Get_World());

    return iExit;
}

void CShipDave::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    // Test 레이어에있는 충돌체 리스트를 들고온다. 널체크
    if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_ShipDave"))
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

                            m_pTransformCom->Update_Component(fTimeDelta);
                            m_pAABB->Transform(m_pTransformCom->Get_World());
                        }
                    }
                }
            }
        }
    }
}

void CShipDave::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sCurrentMotion))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(m_iFrame)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    

    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CShipDave::DoDiveReady()
{
    if (CGameMemMgr::GetInstance()->Get_DiveInfos().size() == 1) {
        if (!m_bTalking) {
            m_fConvAppearTimer += CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_FPS60");
            if (m_fConvAppearTimer > 1.f) {
                if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer"))
                {
                    if (auto pObj = pLayer->Get_GameObjectFirst(L"DaveConversation"))
                    {
                        pObj->Set_DeadCascade();
                    }
                    else
                    {
                        CDaveConversation* pDaveConversation = CDaveConversation::Create(0.f, -2.f);
                        pDaveConversation->SetCurrentConversation(CDaveConversation::CONVERSATION::CONV_3);
                        pLayer->Add_GameObject(L"DaveConversation", pDaveConversation);
                        m_fConvAppearTimer = 0;
                        m_bTalking = true;
                    }
                }
            }
        }
        else {
            m_fTransitionTimer += CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_FPS60");
            if (m_fTransitionTimer > 3.f) {
                CTransition::FadedTransition(CTransition::SCENE_SHIP, CTransition::SCENE_DIVE);
                m_bDiveReady = true;
            }
        }
    }
    else {
        CTransition::FadedTransition(CTransition::SCENE_SHIP, CTransition::SCENE_DIVE);
        m_bDiveReady = true;
    }
   

}

HRESULT CShipDave::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

void CShipDave::Key_Input(const _float& fTimeDelta)
{
    _vec3 vDirUp = {0.f, 1.f, 0.f};
    _vec3 vDirRight = {1.f, 0.f, 0.f };
    m_pTransformCom->Get_Info(INFO_UP, &vDirUp);
    m_pTransformCom->Get_Info(INFO_RIGHT, &vDirRight);
    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    bool m_bKeyInput = false;;
    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_W))
    {
        //m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vDirUp, &vDirUp), 3.f, fTimeDelta);
    }

    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_S))
    {
        //m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vDirUp, &vDirUp), -3.f, fTimeDelta);
    }
    // -6.9, -2.4
    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_A))
    {
        m_bKeyInput = true;
        Motion_Change(L"Tex_ShipDave_Walk");

        if (vPos.x > -6.9f)
        {
            m_pTransformCom->Move_Pos(&vDirRight, 3.f, fTimeDelta);
            if (m_bSeeRight)
            {
                m_bSeeRight = false;
                m_pTransformCom->Rotation(ROT_Y, 180.f);
            }
        }
        
    }

    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_D))
    {
        m_bKeyInput = true;
        Motion_Change(L"Tex_ShipDave_Walk");

        if (vPos.x < -2.4f)
        {
            m_pTransformCom->Move_Pos(&vDirRight, 3.f, fTimeDelta);
            if (!m_bSeeRight)
            {
                m_bSeeRight = true;
                m_pTransformCom->Rotation(ROT_Y, 180.f);
            }
        }
        
    }

    if (!m_bKeyInput)
    {
        Motion_Change(L"Tex_ShipDave_Idle");
    }

}

void CShipDave::Motion_Change(wstring_view svMotion)
{
    if (m_sCurrentMotion != svMotion)
    {
        m_sCurrentMotion = svMotion;
        m_iFrame = 0;
        m_fAccFrameDelta = 0.f;
    }
}

CShipDave* CShipDave::Create()
{
    CShipDave* pShipDave = new CShipDave;

    if (FAILED(pShipDave->Ready_GameObject()))
    {
        Safe_Release(pShipDave);
        MSG_BOX("pShipDave Create Failed");
        return nullptr;
    }

    return pShipDave;
}

void CShipDave::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}
