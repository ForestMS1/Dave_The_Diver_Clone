#include "CGaugeBarUI.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CGaugeBarUI::CGaugeBarUI()
{
}

CGaugeBarUI::CGaugeBarUI(const CGaugeBarUI& rhs)
    :IObserver(rhs)
{
}

CGaugeBarUI::~CGaugeBarUI()
{
}

HRESULT CGaugeBarUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 50.f, 50.f, 1.f };
    m_pTransformCom->Multiply_Scale(&vScale);

    D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_GaugeBar")->at(0))->Get_ImgInfo();
    imgInfo.Width;

    _float fWidth = imgInfo.Width;;
    _float fHeight = imgInfo.Height;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
    m_pTransformCom->Multiply_Scale(&vScale);

    if (!m_bFlip)
        m_pTransformCom->Set_Pos(-95.f, 0.f, 10.f);
    else
        m_pTransformCom->Set_Pos(95.f, 0.f, 10.f);

    return S_OK;
}

_int CGaugeBarUI::Update_GameObject(const _float& fTimeDelta)
{
    m_pDynamicBufferCom->Update_Gauge(m_fRatio);
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    CGameObject::Update_GameObject(fTimeDelta);


    if (!m_bFlip)
    {
        m_pTransformCom->m_vAngle.y = 0.f;
        m_pTransformCom->Set_Pos(-95.f, 0.f, 10.f);
    }
    else
    {
        m_pTransformCom->m_vAngle.y = -180.f;
        m_pTransformCom->Set_Pos(95.f, 0.f, 10.f);
    }

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Compute_ViewZ(&vPos);
    return 0;
}

void CGaugeBarUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CGaugeBarUI::Render_GameObject()
{
    if (!m_bRender)
        return;

    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_GaugeBar"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }

    m_pBufferCom->Render_Buffer();


    _matrix matWorld;
    matWorld = *m_pTransformCom->Get_World();
    matWorld._43 -= 1.f;
    pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
    pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    //-------------------------------------------------------------------------------------
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_GaugeColor"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    
    m_pDynamicBufferCom->Render_Buffer();
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CGaugeBarUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    if (FAILED((AddComponent<Engine::CRcDynamicTex, ID_STATIC>(L"Proto_RcDynamicTex", L"Com_DynamicBuffer", &m_pDynamicBufferCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CGaugeBarUI* CGaugeBarUI::Create()
{
    CGaugeBarUI* pUI = new CGaugeBarUI;
    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("GaugeBarUI Create failed");
        return nullptr;
    }

    return pUI;
}

void CGaugeBarUI::Free()
{
    CGameObject::Free();
}

void CGaugeBarUI::OnNotify(const Event& e)
{
    switch (e.type)
    {
    case EVENTTYPE::ATTACK_GAUGE_CHANGE:
        m_fRatio = e.fValue;
        break;
    case EVENTTYPE::ATTACK_START:
        m_bFlip = e.value;
        m_bRender = true;
        break;
    case EVENTTYPE::ATTACK_END:
        m_fRatio = 0.f;
        m_bRender = false;
        break;
    default:
        break;
    }
}
