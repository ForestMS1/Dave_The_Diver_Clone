#include "pch.h"
#include "CHoldFishUIItemGroup.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CHoldFishUIItemArea.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"
#include"CHoldFishUIDropPanel.h"
#include "CHelper.h"
#include "CHoldFishUIImg.h"
#include "CGameMemMgr.h"
#include "CInfoMgr.h"

CHoldFishUIItemGroup::CHoldFishUIItemGroup(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CHoldFishUIItemGroup::~CHoldFishUIItemGroup()
{
}


HRESULT		CHoldFishUIItemGroup::Ready_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
    
    D3DXCreateTexture(pGraphicDev, CInfoMgr::GetInstance()->Get_WINCX(), CInfoMgr::GetInstance()->Get_WINCY(), 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pRenderTargetTex);
    m_pRenderTargetTex->GetSurfaceLevel(0, &m_pRenderTargetSurf);

    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    //if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_HoldFishUIMask"))
    //{
    //    if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
    //    {
    //        float fWidth = pTexture->Get_ImgInfo()->Width;
    //        float fHeight = pTexture->Get_ImgInfo()->Height;
    //        float fAspect = fWidth / fHeight;
    //        vScale = { fAspect, 1.f, 1.f };

    //        vScale.x *= 0.1f;
    //        vScale.y *= 0.1f;
    //    }
    //}

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, -8.3f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fDbgX = 0.f;
    m_fDbgY = 0.f;
    m_fOffsetY = 0.f;

    m_fViewZ = 0.48f;

    if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
    {
        float refY = 0.5f;

        for (auto& caughtFishe : CGameMemMgr::GetInstance()->Get_DiveInfos().back().Get_Fishes())
        {
            auto pArea = CHoldFishUIItemArea::Create(0, 0.f, refY);
            pArea->Set_Parent(this);
            pArea->Set_Thumbnail(caughtFishe.sThumbNailAssetName);
            pArea->Set_Title(caughtFishe.sFishName);
            pArea->Set_Star(caughtFishe.iStar);
            pArea->Set_MeatCnt(caughtFishe.iMeatCnt);
            pArea->Set_Weight(caughtFishe.fWeight);
            pArea->Set_Rank(caughtFishe.iRank);
            pArea->Ready_AfterCreate();
            pLayer->Add_GameObject(L"HoldFishUIItemArea", pArea);
            refY -= 0.15f;
        }
    }

    return S_OK;
}

_int		CHoldFishUIItemGroup::Update_GameObject(const _float& fTimeDelta)
{


    _vec3 vPos;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

    float fOffsetX = 0.f;
    float fOffsetY = 0.f;
    vPos.x += m_fPosX + m_fDbgX;
    vPos.y += m_fPosY + m_fDbgY + m_fOffsetY;

    m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);

    if (!CDInputMgr::GetInstance()->Mouse_Pressing(DIM_LB))
    {
        int fishsize = CGameMemMgr::GetInstance()->Get_DiveInfos().back().Get_Fishes().size();
        auto oversize = fishsize - 10;
        float maxY = 0.f;
        if (oversize > 0)
        {
            maxY = oversize * 0.15f;
        }
        float minY = -0.f; 

        if (m_fOffsetY <= maxY && m_fOffsetY >= minY)
        {

        }
        else
        {
            float fTarget = m_fOffsetY;

            if (m_fOffsetY > maxY)
                fTarget = maxY;
            else if (m_fOffsetY < minY)
                fTarget = minY;

            float stiffness = 25.f;
            float damping = 8.f;

            float force = (fTarget - m_fOffsetY) * stiffness;
            m_fVelocityY += force * fTimeDelta;

            m_fVelocityY *= (1.f - damping * fTimeDelta);
            m_fOffsetY += m_fVelocityY * fTimeDelta;

            if (fabs(m_fOffsetY - fTarget) < 0.0001f && fabs(m_fVelocityY) < 0.0001f)
            {
                m_fOffsetY = fTarget;
                m_fVelocityY = 0.f;
            }
        }

        
    }

    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA_AFTER_ORTHO_UI, this);

    return iExit;
}

void		CHoldFishUIItemGroup::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
    {
        _vec3 vRayPos, vRayDir;
        CHelper::GetMousePointRay(&vRayPos, &vRayDir);

        if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_HoldFishUIItems"))
        {
            CHoldFishUIItemArea* pIntersected = nullptr;

            for (auto& pCollider : *pColliders)
            {
                if (pCollider->Get_Tag() == L"AABB_HoldFishItemArea")
                {
                    float fDist;
                    if (pCollider->Intersect(&vRayPos, &vRayDir, fDist))
                    {
                        pIntersected = reinterpret_cast<CHoldFishUIItemArea*>(pCollider->Get_VoidPtr());
                        break;
                    }
                }
            }

            if (pIntersected)
            {
                for (auto& pCollider : *pColliders)
                {
                    reinterpret_cast<CHoldFishUIItemArea*>(pCollider->Get_VoidPtr())->Set_EdgeVisible(false);
                }

                pIntersected->Set_EdgeVisible(true);
            }
        }
    }

    if (CDInputMgr::GetInstance()->Mouse_Pressing(DIM_LB))
    {
        _long dwMouseMove(0);
        if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_Y))
        {
            //CLog::Debug(L"mouseYMOVE %i \n", dwMouseMove);
            //_vec3 vUp = { 0.f, 1.f, 0.f };
            //m_pTransformCom->Move_Pos(&vUp, 1.f, fTimeDelta);

            m_fOffsetY += (float(dwMouseMove) * 0.01f);
        }
    }
}

void		CHoldFishUIItemGroup::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    // 렌더 타겟 설정
    LPDIRECT3DSURFACE9 pOldSurface = nullptr;
    pGraphicDev->GetRenderTarget(0, &pOldSurface);
    pGraphicDev->SetRenderTarget(0, m_pRenderTargetSurf);

    // 자식들이 퍼스펙티브이므로 Z버퍼도 함께 밀어줘야 깊이 판정이 정상 작동합니다.
    pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

    // 자식들을 렌더루푸에서 돌리지 않고 여기서 돌림
    CGameObject* pObj = this;
    CHelper::TreeLevelTraversal(pObj, [=](CGameObject* pGO) {
        if (pGO != pObj)
        {
            if (const auto& pArea = dynamic_cast<CHoldFishUIItemArea*>(pGO->Get_Parent()))
            {
                if (pGO->Get_Tag() == L"HoldFishUIEdge")
                {
                    if (pArea->Get_EdgeVisible())
                    {
                        pGO->Render_GameObject();
                    }
                }
                else
                {
                     pGO->Render_GameObject();
                }
            }
            else
            {
                pGO->Render_GameObject();
            }
        }
        });

    // 원래 메인 화면으로 복구
    pGraphicDev->SetRenderTarget(0, pOldSurface);

    // 스텐실 마스크
    pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
    pGraphicDev->SetRenderState(D3DRS_STENCILREF, 0x1);

    // 마스크 객체 자체는 화면에 안 그려지게 설정
    pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0);

    // 마스크 렌더링 
    auto pMask = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer")->Get_GameObjectFirst(L"HoldFishUIMask");
    if (pMask) pMask->Render_GameObject();

    // 마스킹된 도화지 붙이기
    pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000f);
    pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL); // 스텐실 값이 1인 곳에만 그림
    pGraphicDev->SetRenderState(D3DRS_STENCILREF, 0x1);

    
    _matrix matView, matProj;
    pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

    // 이미 새로운 렌더타겟데 애들이 그려질때 위치는 조정되어있으므로 여기서는 항등행렬로 세팅해준다.
    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity(&matIdentity);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matIdentity);
    pGraphicDev->SetTransform(D3DTS_VIEW, &matIdentity);
    pGraphicDev->SetTransform(D3DTS_PROJECTION, &matIdentity);

    pGraphicDev->SetTexture(0, m_pRenderTargetTex);
    m_pBufferCom->Render_Buffer();


    // 상태 원복
    pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
    pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

    if (pOldSurface) pOldSurface->Release();

}

void CHoldFishUIItemGroup::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_DbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_DbgY", &m_fDbgY, 0.01);
}

HRESULT			CHoldFishUIItemGroup::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

}


CHoldFishUIItemGroup* CHoldFishUIItemGroup::Create(float fPosX, float fPosY)
{
    CHoldFishUIItemGroup* pIDiverUpgrade = new CHoldFishUIItemGroup{ fPosX , fPosY };

    if (FAILED(pIDiverUpgrade->Ready_GameObject()))
    {
        Safe_Release(pIDiverUpgrade);
        MSG_BOX("pIDiverUpgrade Create Failed");
        return nullptr;
    }

    return pIDiverUpgrade;
}

void CHoldFishUIItemGroup::Free()
{
    CGameObject::Free();
    Safe_Release(m_pRenderTargetTex);
    Safe_Release(m_pRenderTargetSurf);
}