#include "pch.h"
#include "CToSushiUIItem.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"
#include "CDiveResultUIImg.h"

CToSushiUIItem::CToSushiUIItem(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CToSushiUIItem::~CToSushiUIItem()
{
}


HRESULT		CToSushiUIItem::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_ToSushiUIItem"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width;
            float fHeight = pTexture->Get_ImgInfo()->Height;
            float fAspect = fWidth / fHeight;
            vScale = { fAspect, 1.f, 1.f };

            float fScale = 0.55f;
            vScale.x *= fScale;
            vScale.y *= fScale;
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fDbgX = 0.f;
    m_fDbgY = 0.f;

    m_fViewZ = 0.9f;


    return S_OK;
}

HRESULT CToSushiUIItem::Ready_AfterCreate()
{
    {
        if (auto pLayer = CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer"))
        {
            // Fish
            auto pImg = CDiveResultUIImg::Create(-2.49f, -0.05f);
            pImg->Set_Scale(0.49f);
            pImg->Set_ViewZ(0.49f);
            pImg->Set_AssetName(m_sFishImgAssetName);
            pImg->Set_Parent(this);
            pImg->Ready_After_Create();
            pLayer->Add_GameObject(L"ToSushiFishImg", pImg);
        }
    }

    {
        if (auto pLayer = CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer"))
        {
            // Sushi
            auto pImg = CDiveResultUIImg::Create(-1.57f, -0.2f);
            pImg->Set_Scale(0.233f);
            pImg->Set_ViewZ(0.49f);
            pImg->Set_AssetName(m_sSushiImgAssetName);
            pImg->Set_Parent(this);
            pImg->Ready_After_Create();
            pLayer->Add_GameObject(L"ToSushiSushiImg", pImg);
        }
    }
    return S_OK;
}

_int		CToSushiUIItem::Update_GameObject(const _float& fTimeDelta)
{
    _vec3 vPos;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

    float fOffsetX = 0.f;
    float fOffsetY = 0.f;
    vPos.x += m_fPosX;
    vPos.y += m_fPosY;

    m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CToSushiUIItem::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CToSushiUIItem::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_ToSushiUIItem"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


    // RANK
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.89f;
        float fOffsetY = 0.44f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(L"RANK 1", &vPos, D3DXCOLOR(0.764f, 0.937f, 1.0f, 1.0f));
        }
    }


    // Title
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.78f;
        float fOffsetY = 0.44f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(m_sTitle, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        }
    }

    // CNT
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 2.62f;
        float fOffsetY = -0.12f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(m_sCnt, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        }
    }


    // LV
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.14f;
        float fOffsetY = 0.03f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(m_sLv, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        }
    }

    // Money
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.89f;
        float fOffsetY = -0.23f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(m_sMoney, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        }
    }
}

void CToSushiUIItem::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_DbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_DbgY", &m_fDbgY, 0.01);
}

HRESULT			CToSushiUIItem::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

}


CToSushiUIItem* CToSushiUIItem::Create(float fPosX, float fPosY)
{
    CToSushiUIItem* pToSushi = new CToSushiUIItem{ fPosX , fPosY };

    if (FAILED(pToSushi->Ready_GameObject()))
    {
        Safe_Release(pToSushi);
        MSG_BOX("pToSushi Create Failed");
        return nullptr;
    }

    return pToSushi;
}

void CToSushiUIItem::Free()
{
    CGameObject::Free();
}