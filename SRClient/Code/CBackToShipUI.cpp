#include "pch.h"
#include "CBackToShipUI.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"
#include "CTransition.h"

CBackToShipUI::CBackToShipUI(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CBackToShipUI::~CBackToShipUI()
{
}


HRESULT		CBackToShipUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_BackToShipUI"))
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
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, -8.3f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fDbgX = 0.f;
    m_fDbgY = 0.f;

    //m_fViewZ = -1.f;

    //m_fViewZ = -999999.f;
    return S_OK;
}

_int		CBackToShipUI::Update_GameObject(const _float& fTimeDelta)
{
    if (CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
    {
        CTransition::FadedTransition(CTransition::SCENE_DIVE, CTransition::SCENE_SHIP);
    }

    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
    _matrix matView, matInvView;
    pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixInverse(&matInvView, 0, &matView);

    float x = matInvView.m[3][0];
    float y = matInvView.m[3][1];


    //m_pTransformCom->Set_Pos(x + m_fPosX, y + m_fPosY, -8.3f);
    m_pTransformCom->Set_Pos(0, 0, -8.3f);
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);

   

    return iExit;
}

void		CBackToShipUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void		CBackToShipUI::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_BackToShipUI"))
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
}

void CBackToShipUI::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_DbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_DbgY", &m_fDbgY, 0.01);
}

HRESULT			CBackToShipUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

}


CBackToShipUI* CBackToShipUI::Create(float fPosX, float fPosY)
{
    CBackToShipUI* pIDiverUpgrade = new CBackToShipUI{ fPosX , fPosY };

    if (FAILED(pIDiverUpgrade->Ready_GameObject()))
    {
        Safe_Release(pIDiverUpgrade);
        MSG_BOX("pIDiverUpgrade Create Failed");
        return nullptr;
    }

    return pIDiverUpgrade;
}

void CBackToShipUI::Free()
{
    CGameObject::Free();
}