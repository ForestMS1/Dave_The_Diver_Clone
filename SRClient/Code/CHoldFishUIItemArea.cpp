#include "pch.h"
#include "CHoldFishUIItemArea.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CHoldFishUIImg.h"
#include "CColliderMgr.h"

CHoldFishUIItemArea::CHoldFishUIItemArea(_uint iIdx,float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_pAABB(nullptr)
    , m_iIdx(iIdx)
{
}

CHoldFishUIItemArea::~CHoldFishUIItemArea()
{
}

void CHoldFishUIItemArea::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_fDbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_fDbgY", &m_fDbgY, 0.01);
}


HRESULT		CHoldFishUIItemArea::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_HoldFishItemArea"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {

            float fWidth = pTexture->Get_ImgInfo()->Width;
            float fHeight = pTexture->Get_ImgInfo()->Height;
            float fAspect = fWidth / fHeight;
            vScale = { fAspect, 1.f, 1.f };

            float fScale = 0.069f;
            vScale.x *= fScale;
            vScale.y *= fScale;
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    // 왼쪽 물고기 이미지
    {
        if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
        {
            //pLayer

            auto pJacksalChock = CHoldFishUIImg::Create(-0.680f, 0.f);
            pJacksalChock->Set_Scale(0.0175f);
            pJacksalChock->Set_ViewZ(0.49f);
            pJacksalChock->Set_AssetName(L"Tex_FishUIJaksalChock");
            pJacksalChock->Set_Parent(this);
            pJacksalChock->Ready_After_Create();
            pLayer->Add_GameObject(L"HoldFishItemImg", pJacksalChock);
        }
    }

    // 엣지
    {
        if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
        {
            //pLayer
            auto pEdge = CHoldFishUIImg::Create(0.f, 0.f);
            pEdge->Set_Scale(0.071f);
            pEdge->Set_ViewZ(999.f);
            pEdge->Set_AssetName(L"Tex_HoldFishItemAreaEdge");
            pEdge->Set_Parent(this);
            pEdge->Ready_After_Create();
            pLayer->Add_GameObject(L"HoldFishItemAreaEdge", pEdge);

            m_pEdgeImg = pEdge;
        }
    }


    _vec3 vPos = { 0.f, 0.f, 0.f };
    _vec3 vExt = { 1.f, 1.f, 0.001f };

    m_pAABB = CAABB::Create(&vPos, &vExt, L"AABB_HoldFishItemArea", this);

    m_bEdgeVisible = false;

    return S_OK;
}

_int		CHoldFishUIItemArea::Update_GameObject(const _float& fTimeDelta)
{
    _vec3 vPos;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

    float fOffsetX = 0.f;
    float fOffsetY = 0.f;
    vPos.x += m_fPosX;
    vPos.y += m_fPosY;

    m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);

    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_HoldFishUIItems", m_pAABB);
    m_pAABB->Transform(m_pTransformCom->Get_World());

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CHoldFishUIItemArea::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);


}

void		CHoldFishUIItemArea::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_HoldFishItemArea"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }

    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);


    // Rank
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.610f;
        float fOffsetY = 0.055f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(L"RANK1", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        }
    }

    // Title
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.440f;
        float fOffsetY = 0.055f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"Title", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 0.5f));
        }
    }

    // MeatCnt
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.420f;
        float fOffsetY = -0.010f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"1", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 0.5f));
        }
    }

    // Weight
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.560f;
        float fOffsetY = -0.010f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"1.1Kg", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 0.5f));
        }
    }

}

HRESULT			CHoldFishUIItemArea::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}

void CHoldFishUIItemArea::Set_EdgeVisible(bool bVisible)
{
    if (m_pEdgeImg)
    {
        if (bVisible)
        {
            m_pEdgeImg->Set_ViewZ(0.48f);
            m_bEdgeVisible = true;
        }
        else
        {
            m_pEdgeImg->Set_ViewZ(999.f);
            m_bEdgeVisible = false;
        }
    }
}


CHoldFishUIItemArea* CHoldFishUIItemArea::Create(_uint iIdx, float fPosX, float fPosY)
{
    CHoldFishUIItemArea* pGetItemUI = new CHoldFishUIItemArea{ iIdx, fPosX , fPosY };

    if (FAILED(pGetItemUI->Ready_GameObject()))
    {
        Safe_Release(pGetItemUI);
        MSG_BOX("pGetItemUI Create Failed");
        return nullptr;
    }

    return pGetItemUI;
}

void CHoldFishUIItemArea::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}