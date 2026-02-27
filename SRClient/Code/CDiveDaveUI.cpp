#include "CDiveDaveUI.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
CDiveDaveUI::CDiveDaveUI()
{
}

CDiveDaveUI::CDiveDaveUI(const CDiveDaveUI& rhs)
    :CGameObject(rhs)
{
}

CDiveDaveUI::~CDiveDaveUI()
{
}

HRESULT CDiveDaveUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 200.f, 200.f, 0.f };
    m_pTransformCom->Set_Scale(&vScale);
    return S_OK;
}

_int CDiveDaveUI::Update_GameObject(const _float& fTimeDelta)
{
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    CGameObject::Update_GameObject(fTimeDelta);
    return 0;
}

void CDiveDaveUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CDiveDaveUI::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();
}

HRESULT CDiveDaveUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_HarpoonTexture", L"Com_Texture", &m_pTextureCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CDiveDaveUI* CDiveDaveUI::Create()
{
    CDiveDaveUI* pUI = new CDiveDaveUI;
    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("UI Create failed");
        return nullptr;
    }

    return pUI;
}

void CDiveDaveUI::Free()
{
    CGameObject::Free();
}
