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

#include "CHoldFishUIImg.h"
#include "CGameMemMgr.h"

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
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_HoldFishUIMask"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width;
            float fHeight = pTexture->Get_ImgInfo()->Height;
            float fAspect = fWidth / fHeight;
            vScale = { fAspect, 1.f, 1.f };

            vScale.x *= 0.1f;
            vScale.y *= 0.1f;
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, -8.3f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fDbgX = 0.f;
    m_fDbgY = 0.f;
    m_fOffsetY = 0.f;


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
        //{
        //    auto pArea = CHoldFishUIItemArea::Create(0, 0.f, refY);
        //    pArea->Set_Parent(this);
        //    pLayer->Add_GameObject(L"HoldFishUIItemArea", pArea);
        //}
        //refY -= 0.15f;
        //{
        //    auto pArea = CHoldFishUIItemArea::Create(1, 0.f, refY);
        //    pArea->Set_Parent(this);
        //    pLayer->Add_GameObject(L"HoldFishUIItemArea", pArea);
        //}
        //refY -= 0.15f;
        //{
        //    auto pArea = CHoldFishUIItemArea::Create(2, 0.f, refY);
        //    pArea->Set_Parent(this);
        //    pLayer->Add_GameObject(L"HoldFishUIItemArea", pArea);
        //}
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

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);




    //if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_HoldFishUIMask"))
    //{
    //    if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
    //    {
    //        pGraphicDev->SetTexture(0, pTexture->Get_Texture());
    //    }
    //}
    //pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    //m_pBufferCom->Render_Buffer();


    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

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
}