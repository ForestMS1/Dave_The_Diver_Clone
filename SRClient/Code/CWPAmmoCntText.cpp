#include "pch.h"
#include "CWPAmmoCntText.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CColliderMgr.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CWPBoxUI.h"
#include "CDiveDaveGun.h"

CWPAmmoCntText::CWPAmmoCntText(float fPosX, float fPosY)
    : IObserver()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_sTxt({})
    , m_dwOpt(DT_NOCLIP)
    , m_color(D3DXCOLOR(0.f, 0.f, 0.f, 1.f))
    , m_sFont(L"Font_210YouthL_Size15")
{
}

CWPAmmoCntText::~CWPAmmoCntText()
{
}


HRESULT      CWPAmmoCntText::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;
    m_sFont = L"Font_210YouthL_Size15";
    m_bRender = true;
    return S_OK;
}

_int      CWPAmmoCntText::Update_GameObject(const _float& fTimeDelta)
{
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    auto a = dynamic_cast<CWPBoxUI*>(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer")->Get_GameObjectFirst(L"WPBoxUI2"));
    a->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &m_vPos);

    auto gun = static_cast<CDiveDaveGun*>(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"DiveDaveGun"));
    m_iAmmoCnt = gun->Get_AmmoCnt();
    m_sTxt = to_wstring(m_iAmmoCnt);
    

    float fOffsetX = 27.5f;
    float fOffsetY = -12.5f;
    m_vPos.x += fOffsetX;
    m_vPos.y += fOffsetY;

    m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

    return iExit;
}

void      CWPAmmoCntText::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void      CWPAmmoCntText::Render_GameObject()
{
    if (!m_bRender)
        return;
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    _vec3 vScreenPos;
    CHelper::GetScreenPointFromWorld(&vScreenPos, &m_vPos);

    _vec2 vPos = { vScreenPos.x , vScreenPos.y };
    if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(m_sFont))
    {
        pDefFont->Render_Font(m_sTxt, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
    }

}

HRESULT         CWPAmmoCntText::Ready_Component()
{

    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CWPAmmoCntText* CWPAmmoCntText::Create(float fPosX, float fPosY)
{
    CWPAmmoCntText* pWeaponImg = new CWPAmmoCntText{ fPosX , fPosY };

    if (FAILED(pWeaponImg->Ready_GameObject()))
    {
        Safe_Release(pWeaponImg);
        MSG_BOX("CWPAmmoCntText Create Failed");
        return nullptr;
    }

    return pWeaponImg;
}

void CWPAmmoCntText::Free()
{
    CGameObject::Free();
}

void CWPAmmoCntText::OnNotify(const Event& e)
{
    switch (e.type)
    {
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
