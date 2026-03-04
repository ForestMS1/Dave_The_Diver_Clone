#include "CO2StrokeUI.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CO2StrokeUI::CO2StrokeUI()
{
}

CO2StrokeUI::CO2StrokeUI(const CO2StrokeUI& rhs)
    :IObserver(rhs)
{
}

CO2StrokeUI::~CO2StrokeUI()
{
}

HRESULT CO2StrokeUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 59.f, 59.f, 5.f };

    m_pTransformCom->Set_Scale(&vScale);
    m_pTransformCom->Set_Pos(-536.f, -265.f, 5.f);

    m_bRender = true;
    return S_OK;
}

_int CO2StrokeUI::Update_GameObject(const _float& fTimeDelta)
{
    if (m_fPlayerHpRatio - m_fRatio > 0.1f)
        m_fRatio += fTimeDelta * 5.f;
    else if (m_fRatio - m_fPlayerHpRatio > 0.1f)
        m_fRatio -= fTimeDelta * 5.f;
    m_pDynamicBufferCom->Update_Gauge(m_fRatio);
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    CGameObject::Update_GameObject(fTimeDelta);

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Compute_ViewZ(&vPos);
    return 0;
}

void CO2StrokeUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CO2StrokeUI::Render_GameObject()
{
    if (!m_bRender)
        return;
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_O2Stroke"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }

    m_pBufferCom->Render_Buffer();
    //--------------------------------------------------------------------------------------------------------
    _matrix matWorld;
    matWorld = *m_pTransformCom->Get_World();
    matWorld._43 += 1.f;
    pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
    pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    //-------------------------------------------------------------------------------------
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_O2StrokeFull"))
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
}

HRESULT CO2StrokeUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    m_pDynamicBufferCom = Engine::CRcDynamicTex::Create();
    m_mapComponent[ID_STATIC].insert({ L"Com_DynamicBuffer", m_pDynamicBufferCom });

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CO2StrokeUI* CO2StrokeUI::Create()
{
    CO2StrokeUI* pUI = new CO2StrokeUI;
    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("DaggerBoxUI Create failed");
        return nullptr;
    }

    return pUI;
}

void CO2StrokeUI::Free()
{
    CGameObject::Free();
}

void CO2StrokeUI::OnNotify(const Event& e)
{
    switch (e.type)
    {
    case EVENTTYPE::CHANGE_HP:
        m_fPlayerHpRatio = e.fValue;
        if (m_fPlayerHpRatio > m_fRatio)
            m_IsRestore = true;
        else
            m_IsRestore = false;
        break;
    case EVENTTYPE::BOSS_INTRO_START:
        m_bRender = false;
        break;
    case EVENTTYPE::BOSS_INTRO_END:
        m_bRender = true;
        break;
    default:
        break;
    }
}
