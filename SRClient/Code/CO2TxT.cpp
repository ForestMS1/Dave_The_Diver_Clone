#include "pch.h"
#include "CO2TxT.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CColliderMgr.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CO2UI.h"

CO2TxT::CO2TxT(float fPosX, float fPosY)
    : IObserver()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_sTxt({})
    , m_dwOpt(DT_NOCLIP)
    , m_color(D3DXCOLOR(1.f, 1.f, 1.f, 1.f))
    , m_sFont(L"Font_210YouthL_Size15")
{
}

CO2TxT::~CO2TxT()
{
}


HRESULT      CO2TxT::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;
    m_sFont = L"Font_Snowstorm_Size30";

    m_sTxt = L"O2";

    m_bRender = true;
    return S_OK;
}

_int      CO2TxT::Update_GameObject(const _float& fTimeDelta)
{
    /*m_sTxt = to_wstring(m_iPlayerDepth);*/
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    if (m_bEndFrame)
    {
        auto a = dynamic_cast<CO2UI*>(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer")->Get_GameObjectFirst(L"O2UI"));
        a->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &m_vPos);

        float fOffsetX = -65.f;
        float fOffsetY = 20.f;
        m_vPos.x += fOffsetX;
        m_vPos.y += fOffsetY;

        m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
    }

    return iExit;
}

void      CO2TxT::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void      CO2TxT::Render_GameObject()
{
    if (!m_bRender)
        return;
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    _vec3 vScreenPos;
    CHelper::GetScreenPointFromWorld(&vScreenPos, &m_vPos);

    _vec2 vPos = { vScreenPos.x , vScreenPos.y };
    if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(m_sFont))
    {
        pDefFont->Render_Font(m_sTxt, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
    }

    m_bEndFrame = true;

}

HRESULT         CO2TxT::Ready_Component()
{

    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CO2TxT* CO2TxT::Create(float fPosX, float fPosY)
{
    CO2TxT* pWeaponImg = new CO2TxT{ fPosX , fPosY };

    if (FAILED(pWeaponImg->Ready_GameObject()))
    {
        Safe_Release(pWeaponImg);
        MSG_BOX("CO2TxT Create Failed");
        return nullptr;
    }

    return pWeaponImg;
}

void CO2TxT::Free()
{
    CGameObject::Free();
}

void CO2TxT::OnNotify(const Event& e)
{
    switch (e.type)
    {
    case EVENTTYPE::CHANGE_HP:
        m_iPlayerHp = e.value;
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
