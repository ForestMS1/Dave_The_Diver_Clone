#include "pch.h"
#include "CO2Text.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CColliderMgr.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CO2UI.h"

CO2Text::CO2Text(float fPosX, float fPosY)
    : IObserver()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_sTxt({})
    , m_dwOpt(DT_NOCLIP)
    , m_color(D3DXCOLOR(1.f, 1.f, 1.f, 1.f))
    , m_sFont(L"Font_210YouthL_Size15")
{
}

CO2Text::~CO2Text()
{
}


HRESULT      CO2Text::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;
    m_sFont = L"Font_Snowstorm";

    m_bRender = true;
    return S_OK;
}

_int      CO2Text::Update_GameObject(const _float& fTimeDelta)
{
    m_sTxt = to_wstring(m_iPlayerHp);
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    auto a = dynamic_cast<CO2UI*>(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer")->Get_GameObjectFirst(L"O2UI"));
    a->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &m_vPos);

    float fOffsetX = -65.f;
    float fOffsetY = -20.f;
    m_vPos.x += fOffsetX;
    m_vPos.y += fOffsetY;

    m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

    return iExit;
}

void      CO2Text::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void      CO2Text::Render_GameObject()
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

}

HRESULT         CO2Text::Ready_Component()
{

    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CO2Text* CO2Text::Create(float fPosX, float fPosY)
{
    CO2Text* pWeaponImg = new CO2Text{ fPosX , fPosY };

    if (FAILED(pWeaponImg->Ready_GameObject()))
    {
        Safe_Release(pWeaponImg);
        MSG_BOX("CO2Text Create Failed");
        return nullptr;
    }

    return pWeaponImg;
}

void CO2Text::Free()
{
    CGameObject::Free();
}

void CO2Text::OnNotify(const Event& e)
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
