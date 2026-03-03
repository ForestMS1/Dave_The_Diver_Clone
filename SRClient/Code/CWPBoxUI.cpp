#include "CWPBoxUI.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CWPBoxUI::CWPBoxUI(_bool isSub)
    : m_bIsGunSlot(isSub)
    , m_bIsFront(isSub)
{
}

CWPBoxUI::CWPBoxUI(const CWPBoxUI& rhs)
    :IObserver(rhs)
{
}

CWPBoxUI::~CWPBoxUI()
{
}

HRESULT CWPBoxUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 37.5f, 37.5f, 1.f };
    m_pTransformCom->Multiply_Scale(&vScale);

    D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_WPMainBox")->at(0))->Get_ImgInfo();
    imgInfo.Width;

    _float fWidth = imgInfo.Width;;
    _float fHeight = imgInfo.Height;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
    m_pTransformCom->Multiply_Scale(&vScale);

    if (!m_bIsGunSlot)
        m_pTransformCom->Set_Pos(510.f, -280.f, 10.f);
    else
        m_pTransformCom->Set_Pos(530.f, -295.f, 20.f);


    return S_OK;
}

_int CWPBoxUI::Update_GameObject(const _float& fTimeDelta)
{
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    CGameObject::Update_GameObject(fTimeDelta);

    Move_Slot(fTimeDelta);

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Compute_ViewZ(&vPos);
    return 0;
}

void CWPBoxUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CWPBoxUI::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_WPMainBox"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }

    m_pBufferCom->Render_Buffer();

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_WPMainBoxStroke"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    m_pBufferCom->Render_Buffer();

    Draw_Item(pGraphicDev);
}

HRESULT CWPBoxUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CWPBoxUI* CWPBoxUI::Create(_bool isSub)
{
    CWPBoxUI* pUI = new CWPBoxUI(isSub);
    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("CWPBoxUI Create failed");
        return nullptr;
    }

    return pUI;
}

void CWPBoxUI::Free()
{
    CGameObject::Free();
}

void CWPBoxUI::Move_Slot(const _float& fTimeDelta)
{
    if (!m_bIsChanging)
        return;

    _vec3 vTargetPos;
    if (m_bIsFront)
        vTargetPos = { 510.f, -280.f, 10.f };
    else
        vTargetPos = { 530.f, -295.f, 20.f };

    _vec3 vCurPos, vDir;
    m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
    vDir = vTargetPos - vCurPos;
    if (D3DXVec3Length(&vDir) < 0.01f)
    {
        m_bIsChanging = false;
        m_bIsFront = !m_bIsFront;
        return;
    }
    m_pTransformCom->Move_Pos(&vDir, 10.f, fTimeDelta);
}
void CWPBoxUI::Draw_Item(LPDIRECT3DDEVICE9 pGraphicDev)
{
    if (!m_wsTargetItemTexName.empty())
    {
        // ===== 아이템 전용 스케일 =====
        D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTargetItemTexName)->at(0))->Get_ImgInfo();

        float fTargetSize = 1.f;   // 원하는 UI 내부 아이템 크기
        float fMax = max(imgInfo.Width, imgInfo.Height);

        float fScaleX = imgInfo.Width / fMax;
        float fScaleY = imgInfo.Height / fMax;

        D3DXMATRIX matScale, matWorld;

        D3DXMatrixScaling(&matScale,
            fScaleX * fTargetSize,
            fScaleY * fTargetSize,
            1.f);

        matWorld = matScale * (*m_pTransformCom->Get_World());

        pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_wsTargetItemTexName))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        m_pBufferCom->Render_Buffer();

        // 원래 월드행렬 복구
        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    }
}

void CWPBoxUI::OnNotify(const Event& e)
{
    switch (e.type)
    {
    case EVENTTYPE::GET_WEAPON:
        if(e.value == 1 && !m_bIsGunSlot)
            m_wsTargetItemTexName = e.ItemTextureName;
        if (e.value == 2 && m_bIsGunSlot)
            m_wsTargetItemTexName = e.ItemTextureName;
        break;
    case EVENTTYPE::WEAPONSLOT_CHANGE:
        m_bIsChanging = true;
        //m_bIsGunSlot = !m_bIsGunSlot;
        break;

    default:
        break;
    }
}
