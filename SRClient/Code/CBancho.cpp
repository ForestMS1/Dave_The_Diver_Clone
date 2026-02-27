#include "pch.h"
#include "CBancho.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CGameMemMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CDInputMgr.h"
#include "CColliderMgr.h"
#include "CWasabiObject.h"
CBancho::CBancho()
    : CGameObject() 
{
    curState = IDLE;
    m_sFishName = L"";
    m_sTexName = L"";
    wasabiUse = false;
}

CBancho::CBancho(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CBancho::~CBancho()
{
}

HRESULT CBancho::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_fFrame = 0.f;
    _vec3 vExtents = { 0.3f, 1.0f, 3.1f };
    _vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Bancho", this);
    return S_OK;

}
void CBancho::Update_ImGui()
{
    CGameObject::Update_ImGui();
    ImGui::DragFloat("tempY", &m_fGauge, 0.01f);
}
_int CBancho::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
 
    
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Bancho", m_pAABB);
    _matrix temp = *m_pTransformCom->Get_World();
    temp.m[3][0] = 5.2f;
    temp.m[3][2] = -1.5f;
    m_pAABB->Transform(&temp);

    switch (curState)
    {
    case IDLE:
        m_fFrame += 5.f * fTimeDelta;
        break;
    case COOK:
        m_fFrame += 5.f * fTimeDelta;
        break;
    }
    CGameObject* wasabi = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"WasabiObject");

    if (static_cast<CWasabiObject*>(wasabi)->gauge > -0.9f) {
        if (CGameMemMgr::GetInstance()->getCookingMenu().size() != 0) {
            // 요리중으로 변경되면 현재 상태를 COOK 전 상태를 
        
            m_sFishName = CGameMemMgr::GetInstance()->getCookingMenu().front()->name;
            curState = COOK;
            if (3.f < m_fFrame)
                m_fFrame = 0.f;
            if (m_sFishName == L"블루종") {
                m_sTexName = L"Tex_Bluejong";
            }
            else if (m_sFishName == L"노랑탕") {
                m_sTexName = L"Tex_YellowTang";

            }
            else if (m_sFishName == L"코반아지") {
                m_sTexName = L"Tex_Dart";

            }
            else if (m_sFishName == L"노랑백") {
                m_sTexName = L"Tex_YellowBack";

            }
            else if (m_sFishName == L"흰동가리") {
                m_sTexName = L"Tex_ClownFish";
            }
        }
        else {
            curState = IDLE;
            if (5.f < m_fFrame)
                m_fFrame = 0.f;
        }
    }
    

    return iExit;
}

void CBancho::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Compute_ViewZ(&vPos);


    if (curState == COOK) {
        m_fGauge += fTimeDelta * 0.15f;
    }
    //if (m_pAABB->Intersect())
    //{
    //    // Some Logic
    //    if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
    //    {
    //        // 만약 충돌한다면 태그비교후 보이드포인터 들고와서 캐스팅한다음 조작한다.
    //        if (m_pAABB->Get_Tag() == L"AABB_Bancho")
    //        {

    //        }
    //    }
    //}
}

void CBancho::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    switch (curState)
    {
    case IDLE:
        m_pIdleTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case COOK:
        m_pCookTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    }
    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    if (curState == COOK) {
        if (m_fGauge <= 0) {
            if (!wasabiUse) {
                CGameObject* wasabi = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"WasabiObject");
                static_cast<CWasabiObject*>(wasabi)->gauge -= 0.93 * 0.25;
                wasabiUse = true;
            }
            if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_CookingBox"))
            {
                if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
                {
                    pGraphicDev->SetTexture(0, pTexture->Get_Texture());
                }
            }
        }
        else {
            if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_CookingComplete"))
            {
                if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
                {
                    pGraphicDev->SetTexture(0, pTexture->Get_Texture());
                }
                m_fGauge = 0;
            }
        }
        _matrix bancho = *m_pTransformCom->Get_World();
        bancho.m[0][0] = 1.2f;
        bancho.m[1][1] = 0.2f;
        bancho.m[3][1] += 2.0f; 

        _matrix sushi = bancho;


        pGraphicDev->SetTransform(D3DTS_WORLD, &bancho);

        m_pBufferCom->Render_Buffer();
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_CookingGauge"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        bancho.m[0][0] = 0.55f;
        bancho.m[1][1] = 0.05f;

       pGraphicDev->SetTransform(D3DTS_WORLD, &bancho);
       m_pBufferCom->Render_Buffer();

        pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
        pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
        pGraphicDev->SetRenderState(D3DRS_STENCILREF, 0x1);
        pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

        pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 140); // 알파가 1 이상인 것만 통과
        pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

        // 2. 색상과 깊이 기록은 끔 (틀만 잡기 위함)
        pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
        pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

        m_pBufferCom->Render_Buffer();

        pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);

        // 2. 스텐실 판정: 기록된 '1' 영역에만 그리기
        pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
        pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

 
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Yellow"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }
        _matrix newMat = *m_pTransformCom->Get_World();
         bancho.m[3][0] += m_fGauge;
        pGraphicDev->SetTransform(D3DTS_WORLD, &bancho);
        m_pBufferCom->Render_Buffer();


        pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
        pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sTexName))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        sushi.m[0][0] = 0.15f;
        sushi.m[1][1] = 0.15f;
        sushi.m[3][0] += 0.63f;
        pGraphicDev->SetTransform(D3DTS_WORLD, &sushi);
        m_pBufferCom->Render_Buffer();
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_CookingIcon"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        sushi.m[0][0] = 0.13f;
        sushi.m[1][1] = 0.13f;
        sushi.m[3][0] -= 1.25f;
        pGraphicDev->SetTransform(D3DTS_WORLD, &sushi);
        m_pBufferCom->Render_Buffer();

    }
}

HRESULT CBancho::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_BanchoIdleTexture", L"Com_Texture", &m_pIdleTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_CookTexture", L"Com_Texture1", &m_pCookTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 1.f, 1.5f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 8.f, -1.9f, 1.f };
    return S_OK;
}


CBancho* CBancho::Create()
{
    CBancho* pBackGround = new CBancho;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("Open Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CBancho::Free()
{
    Safe_Release(m_pAABB);

    CGameObject::Free();

}
