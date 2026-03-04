#include "pch.h"
#include "CDiveItemDescUI.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CGetItemUIStar.h"
#include "CGetItemUIImg.h"

CDiveItemDescUI::CDiveItemDescUI(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CDiveItemDescUI::~CDiveItemDescUI()
{
}


HRESULT		CDiveItemDescUI::Ready_GameObject()
{
    m_sTitle = L"Title";


    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_DiveItemDescUI"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {

            float fWidth = pTexture->Get_ImgInfo()->Width;
            float fHeight = pTexture->Get_ImgInfo()->Height;
            float fAspect = fWidth / fHeight;
            vScale = { fAspect, 1.f, 1.f };

            float fScale = 50.f;
            vScale.x *= fScale;
            vScale.y *= fScale;
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY , 0.f);
    m_pTransformCom->Set_Scale(&vScale);



    return S_OK;
}

HRESULT CDiveItemDescUI::Ready_AfterCreate()
{
    if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
    {
        //Stars
        //float fStarRefX = 0.5;
        //for (int i = 0; i < m_iStartCnt; ++i)
        //{
        //    auto pGetItemUIStart = CGetItemUIStar::Create(fStarRefX, -0.2f);
        //    pGetItemUIStart->Set_Parent(this);
        //    pLayer->Add_GameObject(L"GetItemUIStar", pGetItemUIStart);

        //    fStarRefX += 0.5f;
        //}

        //{
        //    auto pGetItemImg = CGetItemUIImg::Create(-83.2f, -23.f);
        //    pGetItemImg->Set_AssetName(m_sImgAssetName);
        //    pGetItemImg->Ready_After_Create();
        //    pGetItemImg->Set_Parent(this);
        //    pLayer->Add_GameObject(L"GetItemUIImg", pGetItemImg);
        //}
    }
    return S_OK;
}

_int		CDiveItemDescUI::Update_GameObject(const _float& fTimeDelta)
{
    //LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
    //_matrix matView, matInvView;
    //pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    //D3DXMatrixInverse(&matInvView, 0, &matView);

    //float x = matInvView.m[3][0];
    //float y = matInvView.m[3][1];
    //float val = -640.f - 200.f;
    //val = m_tween.step(int(fTimeDelta * 1000.f));
    //if (m_tween.progress() >= 1.0f) {
    //    Set_DeadCascade();
    //    return OBJ_DEAD;
    //}
    //float val = -640.f - 200.f;
    //if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
    //{
    //    if (auto pObj = pLayer->Get_GameObjects(L"GetItemUI"))
    //    {
    //        if (pObj->front() == this)
    //        {
    //            val = m_tween.step(int(fTimeDelta * 1000.f));
    //            if (m_tween.progress() >= 1.0f) {
    //                Set_DeadCascade();
    //                return OBJ_DEAD;
    //            }
    //        }
    //    }
    //}

    //m_pTransformCom->Set_Pos(val, m_fPosY, 0.f);

    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    if (m_bRender)
    {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    }
    return iExit;
}

void		CDiveItemDescUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CDiveItemDescUI::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_DiveItemDescUI"))
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
        float fOffsetX = 0.f;
        float fOffsetY = +30.f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(m_sTitle, &vPos, D3DXCOLOR(0.99f, 0.91f, 0.16f, 1.0f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }


    // DESC
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.f;
        float fOffsetY = 0.f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(m_sDesc, &vPos, D3DXCOLOR(0.67f, 0.81f, 0.94f, 1.0f), (DT_CENTER | DT_VCENTER | DT_NOCLIP));
        }
    }
}

HRESULT			CDiveItemDescUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CDiveItemDescUI* CDiveItemDescUI::Create(float fPosX, float fPosY)
{
    CDiveItemDescUI* pGetItemUI = new CDiveItemDescUI{ fPosX , fPosY };

    if (FAILED(pGetItemUI->Ready_GameObject()))
    {
        Safe_Release(pGetItemUI);
        MSG_BOX("pGetItemUI Create Failed");
        return nullptr;
    }

    return pGetItemUI;
}

void CDiveItemDescUI::Free()
{
    CGameObject::Free();
}