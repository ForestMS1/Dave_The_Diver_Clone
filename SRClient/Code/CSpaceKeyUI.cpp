#include "CSpaceKeyUI.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CSpaceKeyUI::CSpaceKeyUI()
{
}

CSpaceKeyUI::CSpaceKeyUI(const CSpaceKeyUI& rhs)
    :IObserver(rhs)
{
}

CSpaceKeyUI::~CSpaceKeyUI()
{
}

HRESULT CSpaceKeyUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 50.f, 50.f, 1.f };
    m_pTransformCom->Multiply_Scale(&vScale);

    D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_SpaceKeyUI")->at(0))->Get_ImgInfo();
    imgInfo.Width;

    _float fWidth = imgInfo.Width;;
    _float fHeight = imgInfo.Height;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
    vScale *= 0.25f;
    m_pTransformCom->Multiply_Scale(&vScale);

    if (!m_bFlip)
        m_pTransformCom->Set_Pos(-105.f, 80.f, 10.f);
    else
        m_pTransformCom->Set_Pos(105.f, 80.f, 10.f);


    return S_OK;
}

_int CSpaceKeyUI::Update_GameObject(const _float& fTimeDelta)
{
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    CGameObject::Update_GameObject(fTimeDelta);

    if (!m_bFlip)
    {
        m_pTransformCom->Set_Pos(-105.f, 80.f, 10.f);
    }
    else
    {
        m_pTransformCom->Set_Pos(105.f, 80.f, 10.f);
    }

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Compute_ViewZ(&vPos);
    return 0;
}

void CSpaceKeyUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CSpaceKeyUI::Render_GameObject()
{
    if (!m_bRender)
        return;
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_SpaceKeyUI"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    m_pBufferCom->Render_Buffer();
}

HRESULT CSpaceKeyUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CSpaceKeyUI* CSpaceKeyUI::Create()
{
    CSpaceKeyUI* pUI = new CSpaceKeyUI;
    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("CSpaceKeyUI Create failed");
        return nullptr;
    }
    return pUI;
}

void CSpaceKeyUI::Free()
{
    CGameObject::Free();
}

void CSpaceKeyUI::OnNotify(const Event& e)
{
    switch (e.type)
    {
    case EVENTTYPE::ATTACK_START:
        m_bFlip = e.value;
        m_bRender = true;
        break;
    case EVENTTYPE::ATTACK_END:
        m_bRender = false;
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
