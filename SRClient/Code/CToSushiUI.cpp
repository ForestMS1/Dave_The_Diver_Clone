#include "pch.h"
#include "CToSushiUI.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"

#include "CToSushiUIItem.h"


CToSushiUI::CToSushiUI(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CToSushiUI::~CToSushiUI()
{
}


HRESULT		CToSushiUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_ToSushiUI"))
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
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fDbgX = 0.f;
    m_fDbgY = 0.f;

    m_fViewZ = 0.9f;

    m_bOpenTween = true;
    m_tweenOpen = m_tweenOpen.from(-10.f).to(0.f).during(200);
    m_pTransformCom->Set_Pos(0.f, -10.f, 0.f);
    m_pTransformCom->Update_Component(0.f);

    if (auto pLayer = CManagement::GetInstance()
        ->Get_Scene()
        ->Get_Layer(L"0_GameLogic_Layer"))
    {
        {
            auto pToSushiItem = CToSushiUIItem::Create(-2.73f, 3.16f);
            pToSushiItem->Set_Parent(this);
            pLayer->Add_GameObject(L"pToSushiItem", pToSushiItem);
        }

        {
            auto pToSushiItem = CToSushiUIItem::Create(-2.73f, 2.f);
            pToSushiItem->Set_Parent(this);
            pLayer->Add_GameObject(L"pToSushiItem", pToSushiItem);
        }
    }

    return S_OK;
}

_int		CToSushiUI::Update_GameObject(const _float& fTimeDelta)
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

void		CToSushiUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CToSushiUI::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_ToSushiUI"))
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


     // 식재료
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 3.37f;
        float fOffsetY = 0.f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(L"획득한 식재료가 없습니다.", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

}

void CToSushiUI::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_DbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_DbgY", &m_fDbgY, 0.01);
}

HRESULT			CToSushiUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

}


CToSushiUI* CToSushiUI::Create(float fPosX, float fPosY)
{
    CToSushiUI* pIDiverUpgrade = new CToSushiUI{ fPosX , fPosY };

    if (FAILED(pIDiverUpgrade->Ready_GameObject()))
    {
        Safe_Release(pIDiverUpgrade);
        MSG_BOX("pIDiverUpgrade Create Failed");
        return nullptr;
    }

    return pIDiverUpgrade;
}

void CToSushiUI::Free()
{
    CGameObject::Free();
}