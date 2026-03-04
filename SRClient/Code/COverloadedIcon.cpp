#include "COverloadedIcon.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CDiveDave.h"
#include "CManagement.h"
COverloadedIcon::COverloadedIcon()
{
}

COverloadedIcon::COverloadedIcon(const COverloadedIcon& rhs)
    :CGameObject(rhs)
{
}

COverloadedIcon::~COverloadedIcon()
{
}

HRESULT COverloadedIcon::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"UI_Overloaded")->at(0))->Get_ImgInfo();

    _float fWidth = imgInfo.Width;
    _float fHeight = imgInfo.Height;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
    vScale *= 0.25f;
    m_pTransformCom->Multiply_Scale(&vScale);

    //_vec3 vScale = { 12.5f, 12.5f, 0.f };
    //m_pTransformCom->Set_Scale(&vScale);

    return S_OK;
}

_int COverloadedIcon::Update_GameObject(const _float& fTimeDelta)
{
    auto a = dynamic_cast<CDiveDave*>(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"DiveDave"));
    if (!a->Is_Overloaded())
        return 0;

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    CGameObject::Update_GameObject(fTimeDelta);

    Add_Frame(fTimeDelta, 5.f, 2);
    a->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &m_vPos);

    float fOffsetX = 0.f;
    float fOffsetY = 0.8f;
    m_vPos.x += fOffsetX;
    m_vPos.y += fOffsetY;

    m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);


    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Compute_ViewZ(&vPos);
    return 0;
}

void COverloadedIcon::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void COverloadedIcon::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"UI_Overloaded"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at((_uint)m_fFrame)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }

    m_pBufferCom->Render_Buffer();
}

void COverloadedIcon::Add_Frame(const _float& fTimeDelta, const _float& fSpeed, _uint size)
{
    m_fFrame += fSpeed * fTimeDelta;
    if (m_fFrame > size)
        m_fFrame = 0.f;
}

HRESULT COverloadedIcon::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

COverloadedIcon* COverloadedIcon::Create()
{
    COverloadedIcon* pUI = new COverloadedIcon;
    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("COverloadedIcon Create failed");
        return nullptr;
    }

    return pUI;
}

void COverloadedIcon::Free()
{
    CGameObject::Free();
}
