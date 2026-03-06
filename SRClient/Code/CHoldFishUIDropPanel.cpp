#include "pch.h"
#include "CHoldFishUIDropPanel.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CHoldFishUIImg.h"
#include "CColliderMgr.h"
#include "CSoundMgr.h"

CHoldFishUIDropPanel::CHoldFishUIDropPanel(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CHoldFishUIDropPanel::~CHoldFishUIDropPanel()
{
}

void CHoldFishUIDropPanel::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_fDbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_fDbgY", &m_fDbgY, 0.01);
}


HRESULT		CHoldFishUIDropPanel::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_HoldFishUIDropPanel"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {

            float fWidth = pTexture->Get_ImgInfo()->Width;
            float fHeight = pTexture->Get_ImgInfo()->Height;
            float fAspect = fWidth / fHeight;
            vScale = { fAspect, 1.f, 1.f };

            float fScale = 0.15f;
            vScale.x *= fScale;
            vScale.y *= fScale;
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
    m_pTransformCom->Set_Scale(&vScale);
    m_fViewZ = 0.399;

    CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_Ship_ui_button_click", CSoundMgr::SFX, 1.f);

    return S_OK;
}

_int		CHoldFishUIDropPanel::Update_GameObject(const _float& fTimeDelta)
{
    _vec3 vPos;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

    float fOffsetX = 0.f;
    float fOffsetY = 0.f;
    vPos.x += m_fPosX;
    vPos.y += m_fPosY;

    m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);

    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA_AFTER_ORTHO_UI, this);

    return iExit;
}

void		CHoldFishUIDropPanel::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CHoldFishUIDropPanel::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_HoldFishUIDropPanel"))
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



    // Title
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.f;
        float fOffsetY = 0.06;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            
            pDefFont->Render_Font(m_sTitle + L"을 버리시겠습니까?", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }


}

HRESULT			CHoldFishUIDropPanel::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}



CHoldFishUIDropPanel* CHoldFishUIDropPanel::Create(float fPosX, float fPosY)
{
    CHoldFishUIDropPanel* pGetItemUI = new CHoldFishUIDropPanel{fPosX , fPosY };

    if (FAILED(pGetItemUI->Ready_GameObject()))
    {
        Safe_Release(pGetItemUI);
        MSG_BOX("pGetItemUI Create Failed");
        return nullptr;
    }

    return pGetItemUI;
}

void CHoldFishUIDropPanel::Free()
{
    CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_Ship_ui_button_click", CSoundMgr::SFX, 1.f);
    CGameObject::Free();
}