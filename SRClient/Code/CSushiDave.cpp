#include "pch.h"
#include "CSushiDave.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CDInputMgr.h"
#include "CAABB.h"
#include "CColliderMgr.h"
#include "CGameMemMgr.h"
#include "CBancho.h"
#include "CCustomer1.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CWasabi.h"
#include "CWasabiObject.h"


CSushiDave::CSushiDave()
    : CGameObject()
{
    curState = IDLE;
    holdingSushi = false;
    curDir = LEFT;
    prevDir = LEFT;
    m_sSushiName = L"";
}

CSushiDave::CSushiDave(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CSushiDave::~CSushiDave()
{
}

HRESULT CSushiDave::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_fFrame = 0.f;
    _vec3 vExtents = { 0.3f, 1.0f, 0.1f };
    _vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Dave", this);


    return S_OK;
}

_int CSushiDave::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Dave", m_pAABB);
    _matrix vPos = *m_pTransformCom->Get_World();
    vPos.m[3][2] = -2.99f;
    //vPos.m[3][0] -= 0.3f;
    m_pAABB->Transform(&vPos);
    //m_pAABB->Transform(m_pTransformCom->Get_World());

    switch (curState)
    {
    case IDLE:
        m_fFrame += 2.f * fTimeDelta;
        if (2.f < m_fFrame)
            m_fFrame = 0.f;

        break;
    case SUSHI_IDLE:
        m_fFrame += 1.f * fTimeDelta;
        if (1.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    default:
        m_fFrame += 8.f * fTimeDelta;
        if (8.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    }

 

    return iExit;
}

void CSushiDave::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Compute_ViewZ(&vPos);
    Key_Input(fTimeDelta);
    if (curDir != prevDir) {
        m_pTransformCom->Rotation(ROT_Y, 180.f);
    }
    prevDir = curDir;

    list<CCollider*>* bancho = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_Bancho");
    list<CCollider*>* coliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_Customer");
    list<CCollider*>* wasabi = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_Wasabi");

    if (bancho != nullptr) {
        if (m_pAABB->Intersect(bancho->front()))
        {
            // Some Logic
            CGameObject* bancho = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"Bancho");

            if (static_cast<CBancho*>(bancho)->m_fGauge >= 0) {
                if (CDInputMgr::GetInstance()->Key_Down(DIKEYBOARD_SPACE))
                {
                    holdingSushi = true;
                    m_sSushiName = CGameMemMgr::GetInstance()->getCookingMenu().front()->name;
                    if (m_sSushiName == L"블루종") {
                        m_sTexName = L"Tex_Bluejong";
                    }
                    else if (m_sSushiName == L"노랑탕") {
                        m_sTexName = L"Tex_YellowTang";
                    }
                    else if (m_sSushiName == L"코반아지") {
                        m_sTexName = L"Tex_Dart";
                    }
                    else if (m_sSushiName == L"노랑백") {
                        m_sTexName = L"Tex_YellowBack";
                    }
                    else if (m_sSushiName == L"흰동가리") {
                        m_sTexName = L"Tex_ClownFish";
                    }
                    CGameMemMgr::GetInstance()->deleteCookingMenu();
                    static_cast<CBancho*>(bancho)->m_fGauge = -1;
                    static_cast<CBancho*>(bancho)->wasabiUse = false;

                }
            }
        }
    }
    if (m_pAABB->Intersect(wasabi->front()))
    {
        if (CDInputMgr::GetInstance()->Key_Down(DIKEYBOARD_SPACE)) {
            CGameObject* wasabi1 = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Wasabi");
            static_cast<CWasabi*>(wasabi1)->Set_Render(true);
        }

    }
    if (coliders != nullptr) {
        list<CGameObject*>* customers = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"Customer");
        for (auto customer : *customers) {
            if (m_pAABB->Intersect(static_cast<CCustomer1*>(customer)->Get_AABB())) {
                if (CDInputMgr::GetInstance()->Key_Down(DIKEYBOARD_SPACE))
                {
                    if (static_cast<CCustomer1*>(customer)->MenuBubble != nullptr) {
                        static_cast<CCustomer1*>(customer)->gotSushi = true;
                        static_cast<CCustomer1*>(customer)->sushiHanded = m_sSushiName;
                        holdingSushi = false;
                    }

                }
            }

        }

    }
  
}

void CSushiDave::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    switch (curState)
    {
    case IDLE:
        m_pIdleTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case WALK:
        m_pWalkTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case RUN:
        m_pRunTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case TIRED:
        m_pTiredTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case SUSHI_IDLE:
        m_pSushiIdleTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case SUSHI_WALK:
        m_pSushiWalkTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case SUSHI_RUN:
        m_pSushiRunTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case SUSHI_TIRED:
        m_pSushiTiredTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    }

    m_pBufferCom->Render_Buffer();
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    if (holdingSushi) {
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_SushiBox2"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        _matrix scaleMat = *m_pTransformCom->Get_World();
        scaleMat.m[0][0] = 0.3f;
        scaleMat.m[1][1] = 0.3f;
        scaleMat.m[3][1] += 1.f;

        pGraphicDev->SetTransform(D3DTS_WORLD, &scaleMat);
        m_pBufferCom->Render_Buffer();

        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sTexName))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        scaleMat.m[0][0] = 0.15f;
        scaleMat.m[1][1] = 0.15f;
        pGraphicDev->SetTransform(D3DTS_WORLD, &scaleMat);
        m_pBufferCom->Render_Buffer();
    }
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CSushiDave::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 스시 없을때
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveIdleTexture", L"Com_Texture", &m_pIdleTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveWalkTexture", L"Com_Texture1", &m_pWalkTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveRunTexture", L"Com_Texture2", &m_pRunTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveTiredTexture", L"Com_Texture3", &m_pTiredTextureCom))))
        return E_FAIL;

    //스시 들고 있을때
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveSushiIdleTex", L"Com_Texture4", &m_pSushiIdleTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveSushiWalkTexture", L"Com_Texture5", &m_pSushiWalkTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveSushiRunTexture", L"Com_Texture6", &m_pSushiRunTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveSushiTiredTexture", L"Com_Texture7", &m_pSushiTiredTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 0.7f, 1.f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 5.5f, -1.25f, -2.89f };
    return S_OK;
}

void CSushiDave::Key_Input(const _float& fTimeDelta)
{
    bool bMove = false;
  
    if (!holdingSushi) {
        if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_A) && m_pTransformCom->m_vInfo[INFO_POS].x > -4.2f)
        {
            _vec3 left = { -1,0,0 };
            curState = WALK;
            curDir = LEFT;
            m_pTransformCom->Move_Pos(&left, 2.f, fTimeDelta);
            bMove = true;
        }

        if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_D) && m_pTransformCom->m_vInfo[INFO_POS].x < 6.5f)
        {
            _vec3 right = { 1,0,0 };
            curState = WALK;
            curDir = RIGHT;
            m_pTransformCom->Move_Pos(&right, 2.f, fTimeDelta);
            bMove = true;
        }
        if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LSHIFT))
        {
            curState = RUN;
            if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_A) && m_pTransformCom->m_vInfo[INFO_POS].x > -4.2f)
            {
                _vec3 left = { -1,0,0 };
                curDir = LEFT;
                m_pTransformCom->Move_Pos(&left, 2.5f, fTimeDelta);
                bMove = true;
            }
            if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_D) && m_pTransformCom->m_vInfo[INFO_POS].x < 6.5f)
            {
                _vec3 right = { 1,0,0 };
                curDir = RIGHT;
                m_pTransformCom->Move_Pos(&right, 2.5f, fTimeDelta);
                bMove = true;
            }
        }
    }
    else {
        if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_A) && m_pTransformCom->m_vInfo[INFO_POS].x > -4.2f)
        {
            _vec3 left = { -1,0,0 };
            curState = SUSHI_WALK;
            curDir = LEFT;
            m_pTransformCom->Move_Pos(&left, 2.f, fTimeDelta);
            bMove = true;
        }

        if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_D) && m_pTransformCom->m_vInfo[INFO_POS].x < 6.5f)
        {
            _vec3 right = { 1,0,0 };
            curState = SUSHI_WALK;
            curDir = RIGHT;
            m_pTransformCom->Move_Pos(&right, 2.f, fTimeDelta);
            bMove = true;
        }
        if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LSHIFT))
        {
            curState = SUSHI_RUN;
            if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_A) && m_pTransformCom->m_vInfo[INFO_POS].x > -4.2f)
            {
                _vec3 left = { -1,0,0 };
                curDir = LEFT;
                m_pTransformCom->Move_Pos(&left, 2.5f, fTimeDelta);
                bMove = true;
            }
            if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_D) && m_pTransformCom->m_vInfo[INFO_POS].x < 6.5f)
            {
                _vec3 right = { 1,0,0 };
                curDir = RIGHT;
                m_pTransformCom->Move_Pos(&right, 2.5f, fTimeDelta);
                bMove = true;
            }
        }
    }
   
    if (!bMove) {
        if (holdingSushi) {
            curState = SUSHI_IDLE;

        }
        else {
            curState = IDLE;

        }
        m_fFrame = 0;
    }
}
CSushiDave* CSushiDave::Create()
{
    CSushiDave* pBackGround = new CSushiDave;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("Dave Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CSushiDave::Free()
{
    Safe_Release(m_pAABB);

    CGameObject::Free();

}
