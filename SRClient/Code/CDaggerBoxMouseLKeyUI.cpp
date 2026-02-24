#include "CDaggerBoxMouseLKeyUI.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CDaggerBoxMouseLKeyUI::CDaggerBoxMouseLKeyUI()
{
}

CDaggerBoxMouseLKeyUI::CDaggerBoxMouseLKeyUI(const CDaggerBoxMouseLKeyUI& rhs)
    :CGameObject(rhs)
{
}

CDaggerBoxMouseLKeyUI::~CDaggerBoxMouseLKeyUI()
{
}

HRESULT CDaggerBoxMouseLKeyUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    CDaggerBoxMouseLKeyUI::Refresh_Size();

    m_pTransformCom->Set_Pos(251.f, -252.f, 0.f);

    return S_OK;
}

_int CDaggerBoxMouseLKeyUI::Update_GameObject(const _float& fTimeDelta)
{
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    CGameObject::Update_GameObject(fTimeDelta);

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Compute_ViewZ(&vPos);
    return 0;
}

void CDaggerBoxMouseLKeyUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CDaggerBoxMouseLKeyUI::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_DaggerBoxMouseLKey"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    m_pBufferCom->Render_Buffer();
}

void CDaggerBoxMouseLKeyUI::Refresh_Size()
{
    _vec3 vScale = { 11.5f, 16.f, 1.f };
    m_pTransformCom->Set_Scale(&vScale);
}

HRESULT CDaggerBoxMouseLKeyUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CDaggerBoxMouseLKeyUI* CDaggerBoxMouseLKeyUI::Create()
{
    CDaggerBoxMouseLKeyUI* pUI = new CDaggerBoxMouseLKeyUI;
    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("CDaggerBoxMouseLKeyUI Create failed");
        return nullptr;
    }

    return pUI;
}

void CDaggerBoxMouseLKeyUI::Free()
{
    CGameObject::Free();
}
