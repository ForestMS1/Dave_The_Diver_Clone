#include "CDaggerBoxUI.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CDaggerBoxUI::CDaggerBoxUI()
{
}

CDaggerBoxUI::CDaggerBoxUI(const CDaggerBoxUI& rhs)
    :CGameObject(rhs)
{
}

CDaggerBoxUI::~CDaggerBoxUI()
{
}

HRESULT CDaggerBoxUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 37.5f, 37.5f, 1.f };
    m_pTransformCom->Multiply_Scale(&vScale);

    D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_DaggerBox")->at(0))->Get_ImgInfo();
    imgInfo.Width;

    _float fWidth = imgInfo.Width;;
    _float fHeight = imgInfo.Height;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
    m_pTransformCom->Multiply_Scale(&vScale);

    m_pTransformCom->Set_Pos(280.f, -280.f, 10.f);

    return S_OK;
}

_int CDaggerBoxUI::Update_GameObject(const _float& fTimeDelta)
{
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    CGameObject::Update_GameObject(fTimeDelta);

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Compute_ViewZ(&vPos);
    return 0;
}

void CDaggerBoxUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CDaggerBoxUI::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_DaggerBox"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }

    m_pBufferCom->Render_Buffer();

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_DaggerBoxStroke"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    m_pBufferCom->Render_Buffer();
}

HRESULT CDaggerBoxUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CDaggerBoxUI* CDaggerBoxUI::Create()
{
    CDaggerBoxUI* pUI = new CDaggerBoxUI;
    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("DaggerBoxUI Create failed");
        return nullptr;
    }

    return pUI;
}

void CDaggerBoxUI::Free()
{
    CGameObject::Free();
}
