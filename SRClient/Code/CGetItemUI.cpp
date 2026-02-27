#include "pch.h"
#include "CGetItemUI.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CGetItemUIStar.h"
#include "CGetItemUIImg.h"

CGetItemUI::CGetItemUI(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CGetItemUI::~CGetItemUI()
{
}


HRESULT		CGetItemUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_GetItemUI"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            
            float fWidth = pTexture->Get_ImgInfo()->Width;
            float fHeight = pTexture->Get_ImgInfo()->Height;
            float fAspect = fWidth / fHeight;
            vScale = { fAspect, 1.f, 1.f };
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(-13.f, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    m_tween = m_tween.from(-13.f).to(m_fPosX).during(500).to(m_fPosX).during(500).to(-13.f).during(500);



    if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
    {
        //Stars
        {
            auto pGetItemUIStart = CGetItemUIStar::Create(0.5f, -0.2f);
            pGetItemUIStart->Set_Parent(this);
            pLayer->Add_GameObject(L"GetItemUIStar", pGetItemUIStart);
        }
        {
            auto pGetItemUIStart = CGetItemUIStar::Create(1.f, -0.2f);
            pGetItemUIStart->Set_Parent(this);
            pLayer->Add_GameObject(L"GetItemUIStar", pGetItemUIStart);
        }

        {
            auto pGetItemImg = CGetItemUIImg::Create(-1.6f, -0.5f);
            pGetItemImg->Set_AssetName(L"Tex_PhoneBG");
            pGetItemImg->Ready_After_Create();
            pGetItemImg->Set_Parent(this);
            pLayer->Add_GameObject(L"GetItemUIImg", pGetItemImg);
        }
    }
    
    return S_OK;
}

_int		CGetItemUI::Update_GameObject(const _float& fTimeDelta)
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
    _matrix matView, matInvView;
    pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixInverse(&matInvView, 0, &matView);

    float x = matInvView.m[3][0];
    float y = matInvView.m[3][1];
    float val = -13.f;

    if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
    {
        if (auto pObj = pLayer->Get_GameObjects(L"GetItemUI"))
        {
            if (pObj->front() == this)
            {
                val = m_tween.step(int(fTimeDelta * 1000.f));
               if (m_tween.progress() >= 1.0f) {
                   Set_DeadCascade();
                   return OBJ_DEAD;
               }
            }
        }
    }

    m_pTransformCom->Set_Pos(x + val, y + m_fPosY, 0.f);

    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CGetItemUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CGetItemUI::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_GetItemUI"))
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

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


    // TITLE
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -2.0f;
        float fOffsetY = +0.6f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"TITLE", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        }
    }

    // RANK
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.9f;
        float fOffsetY = -0.1f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"RANK 1", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 0.5f));
        }
    }

    // WEIGHT
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.3f;
        float fOffsetY = -0.6f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"WEIGHT", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 0.5f));
        }
    }

}

HRESULT			CGetItemUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CGetItemUI* CGetItemUI::Create(float fPosX, float fPosY)
{
    CGetItemUI* pGetItemUI = new CGetItemUI{ fPosX , fPosY };

    if (FAILED(pGetItemUI->Ready_GameObject()))
    {
        Safe_Release(pGetItemUI);
        MSG_BOX("pGetItemUI Create Failed");
        return nullptr;
    }

    return pGetItemUI;
}

void CGetItemUI::Free()
{
    CGameObject::Free();
}