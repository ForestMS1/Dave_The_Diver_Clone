#include "pch.h"
#include "CBanchoGood.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"
#include "CGoToSushiUI.h"
#include "CGameMemMgr.h"
CBanchoGood::CBanchoGood(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CBanchoGood::~CBanchoGood()
{
}


HRESULT		CBanchoGood::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Bancho_Good"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width;
            float fHeight = pTexture->Get_ImgInfo()->Height;
            float fAspect = fWidth / fHeight;
            vScale = { fAspect, 1.f, 1.f };

            float fScale = 0.f;
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


    m_tween = m_tween
        .from(0.f, m_fPosX).to(1.f, m_fPosX-1.f).during(300)
        .to(1.f, m_fPosX - 1.f).to(1.f, m_fPosX - 1.f).during(1000)
        .to(1.f, m_fPosX - 1.f).to(0.f, m_fPosX).during(300)
        .via(tweeny::easing::linear);

    return S_OK;
}

_int		CBanchoGood::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    auto val = m_tween.step(int(fTimeDelta * 1000.f));
    _vec3 vScale = { val[0] , val[0] , val[0] };
    m_pTransformCom->Set_Scale(&vScale);

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    m_pTransformCom->Set_Pos(val[1] * 1.21153843f, vPos.y, vPos.z);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CBanchoGood::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    if (m_tween.progress() >= 1.0f)
    {
        if (CGameMemMgr::GetInstance()->Get_ShipNight())
        {
            if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer"))
            {
                if (auto pObj = pLayer->Get_GameObjectFirst(L"GoToSushiUI"))
                {
                    pObj->Set_DeadCascade();
                }
                else
                {
                    CGoToSushiUI* pToSushi = CGoToSushiUI::Create(0.f, 0.f);
                    pLayer->Add_GameObject(L"GoToSushiUI", pToSushi);
                }
            }
        }
        Set_DeadCascade();
    }
}

void		CBanchoGood::Render_GameObject()
{

    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
   
    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Bancho_Good"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
        m_pBufferCom->Render_Buffer();
    }

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

}

void CBanchoGood::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_DbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_DbgY", &m_fDbgY, 0.01);
}

HRESULT			CBanchoGood::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

}


CBanchoGood* CBanchoGood::Create(float fPosX, float fPosY)
{
    CBanchoGood* pIDiverUpgrade = new CBanchoGood{ fPosX , fPosY };

    if (FAILED(pIDiverUpgrade->Ready_GameObject()))
    {
        Safe_Release(pIDiverUpgrade);
        MSG_BOX("pIDiverUpgrade Create Failed");
        return nullptr;
    }

    return pIDiverUpgrade;
}

void CBanchoGood::Free()
{
    CGameObject::Free();
}