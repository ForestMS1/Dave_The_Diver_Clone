#include "CRKeyUI.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CRKeyUI::CRKeyUI(_float x, _float y, _float z)
    :m_vPos({ x,y,z })
{
}

CRKeyUI::CRKeyUI(const CRKeyUI& rhs)
    :CGameObject(rhs)
{
}

CRKeyUI::~CRKeyUI()
{
}

HRESULT CRKeyUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 15.f, 15.f, 1.f };
    m_pTransformCom->Set_Scale(&vScale);
    m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

    return S_OK;
}

_int CRKeyUI::Update_GameObject(const _float& fTimeDelta)
{
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    CGameObject::Update_GameObject(fTimeDelta);

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Compute_ViewZ(&vPos);
    return 0;
}

void CRKeyUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CRKeyUI::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_RKeyUI"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    m_pBufferCom->Render_Buffer();
}

HRESULT CRKeyUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CRKeyUI* CRKeyUI::Create(_float x, _float y, _float z)
{
    CRKeyUI* pUI = new CRKeyUI(x, y, z);
    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("CRKeyUI Create failed");
        return nullptr;
    }
    return pUI;
}

void CRKeyUI::Free()
{
    CGameObject::Free();
}
