#include "pch.h"
#include "CDiveResultUI.h"
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


CDiveResultUI::CDiveResultUI(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CDiveResultUI::~CDiveResultUI()
{
}


HRESULT		CDiveResultUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_DiveResultUI"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width;
            float fHeight = pTexture->Get_ImgInfo()->Height;
            float fAspect = fWidth / fHeight;
            vScale = { fAspect, 1.f, 1.f };

            float fScale = 5.f;
            vScale.x *= fScale;
            vScale.y *= fScale;
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    //m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fDbgX = 0.f;
    m_fDbgY = 0.f;

    m_fViewZ = 0.9f;

    m_bOpenTween = true;
    m_tweenOpen = m_tweenOpen.from(-10.f).to(0.f).during(200);
    m_pTransformCom->Set_Pos(0.f, -10.f, 0.f);
    m_pTransformCom->Update_Component(0.f);



    {
        if (auto pLayer = CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer"))
        {
            auto pImg = CDiveResultUIImg::Create(-2.4f, 0.14f);
            pImg->Set_Scale(0.1);
            pImg->Set_ViewZ(0.49f);
            pImg->Set_AssetName(L"Tex_FishUIJaksalChock");
            pImg->Set_Parent(this);
            pImg->Ready_After_Create();
            pLayer->Add_GameObject(L"BiggestFishImg", pImg);
        }
    }


    {
        if (auto pLayer = CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer"))
        {
            auto pImg = CDiveResultUIImg::Create(-2.55f, -1.16f);
            pImg->Set_Scale(0.1);
            pImg->Set_ViewZ(0.49f);
            pImg->Set_AssetName(L"Tex_FishUIJaksalChock");
            pImg->Set_Parent(this);
            pImg->Ready_After_Create();
            pLayer->Add_GameObject(L"TestImg1", pImg);
        }
    }

    return S_OK;
}

_int		CDiveResultUI::Update_GameObject(const _float& fTimeDelta)
{
    if (CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
    {
        Set_DeadCascade();
    }

    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    if (m_bOpenTween)
    {
        // PosY, RotX, RotY
        auto val = m_tweenOpen.step(int(fTimeDelta * 1000.f));
        _vec3 dir = { 0.f, 1.f, 0.f };
        _vec3 vPos;

        m_pTransformCom->Get_Info(INFO_POS, &vPos);
        m_pTransformCom->Set_Pos(vPos.x, val, vPos.z);

        if (m_tweenOpen.progress() >= 1.0f) {
            m_bOpenTween = false;
            m_bOpen = true;
        }
    }

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CDiveResultUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CDiveResultUI::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_DiveResultUI"))
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


    // Dive NO
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.23f;
        float fOffsetY = 3.850f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"DIVE NO", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // DATE
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 1.83f;
        float fOffsetY = 3.83f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"DATE", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // TEMP
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.21f;
        float fOffsetY = 3.13f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"TEMP", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // TIME
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.21f;
        float fOffsetY = 2.38f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"TIME", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // DEPTH
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 1.72f;
        float fOffsetY = 2.36f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"DEPTH", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // CAUGHT
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.21f;
        float fOffsetY = 1.47f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"CAUGHT", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // OBTAINED
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 1.76f;
        float fOffsetY = 1.47f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"OBTAINED", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // BIGGEST FISH
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.23f;
        float fOffsetY = 0.27f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"BIGGEST", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // 30CM
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.46f;
        float fOffsetY = -0.09f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"30CM", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }
}

void CDiveResultUI::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_DbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_DbgY", &m_fDbgY, 0.01);
}

HRESULT			CDiveResultUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

}


CDiveResultUI* CDiveResultUI::Create(float fPosX, float fPosY)
{
    CDiveResultUI* pIDiverUpgrade = new CDiveResultUI{ fPosX , fPosY };

    if (FAILED(pIDiverUpgrade->Ready_GameObject()))
    {
        Safe_Release(pIDiverUpgrade);
        MSG_BOX("pIDiverUpgrade Create Failed");
        return nullptr;
    }

    return pIDiverUpgrade;
}

void CDiveResultUI::Free()
{
    CGameObject::Free();
}