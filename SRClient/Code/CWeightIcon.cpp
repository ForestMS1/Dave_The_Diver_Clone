#include "CWeightIcon.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CO2UI.h"
#include "CManagement.h"
CWeightIcon::CWeightIcon()
{
}

CWeightIcon::CWeightIcon(const CWeightIcon& rhs)
    :CGameObject(rhs)
{
}

CWeightIcon::~CWeightIcon()
{
}

HRESULT CWeightIcon::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 12.5f, 12.5f, 0.f };
    m_pTransformCom->Set_Scale(&vScale);

    return S_OK;
}

_int CWeightIcon::Update_GameObject(const _float& fTimeDelta)
{
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    CGameObject::Update_GameObject(fTimeDelta);

    auto a = dynamic_cast<CO2UI*>(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer")->Get_GameObjectFirst(L"O2UI"));
    a->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &m_vPos);

    float fOffsetX = 20.f;
    float fOffsetY = -20.f;
    float fOffsetZ = -1.f;
    m_vPos.x += fOffsetX;
    m_vPos.y += fOffsetY;
    m_vPos.z += fOffsetZ;

    m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);


    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Compute_ViewZ(&vPos);
    return 0;
}

void CWeightIcon::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CWeightIcon::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_WeightIcon"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }

    m_pBufferCom->Render_Buffer();
}

HRESULT CWeightIcon::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CWeightIcon* CWeightIcon::Create()
{
    CWeightIcon* pUI = new CWeightIcon;
    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("CWeightIcon Create failed");
        return nullptr;
    }

    return pUI;
}

void CWeightIcon::Free()
{
    CGameObject::Free();
}
