#include "pch.h"
#include "CWeightText.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CColliderMgr.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CO2UI.h"

CWeightText::CWeightText(float fPosX, float fPosY)
    : IObserver()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_sTxt({})
    , m_dwOpt(DT_NOCLIP)
    , m_color(D3DXCOLOR(1.f, 1.f, 1.f, 1.f))
    , m_sFont(L"Font_210YouthL_Size15")
{
}

CWeightText::~CWeightText()
{
}


HRESULT      CWeightText::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;
    m_sFont = L"Font_Snowstorm_Size20";
    return S_OK;
}

_int      CWeightText::Update_GameObject(const _float& fTimeDelta)
{
    wchar_t szBuffer[64] = {};
    swprintf(szBuffer, 64, L"%.1f/%.1fkg", m_fPlayerCurWeight, m_fPlayerMaxWeight);
    m_sTxt = szBuffer;

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    auto a = dynamic_cast<CO2UI*>(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer")->Get_GameObjectFirst(L"O2UI"));
    a->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &m_vPos);

    float fOffsetX = 90.f;
    float fOffsetY = -20.f;
    m_vPos.x += fOffsetX;
    m_vPos.y += fOffsetY;

    m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

    return iExit;
}

void      CWeightText::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void      CWeightText::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    _vec3 vScreenPos;
    CHelper::GetScreenPointFromWorld(&vScreenPos, &m_vPos);

    _vec2 vPos = { vScreenPos.x , vScreenPos.y };
    if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(m_sFont))
    {
        pDefFont->Render_Font(m_sTxt, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
    }

}

HRESULT         CWeightText::Ready_Component()
{

    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CWeightText* CWeightText::Create(float fPosX, float fPosY)
{
    CWeightText* pWeaponImg = new CWeightText{ fPosX , fPosY };

    if (FAILED(pWeaponImg->Ready_GameObject()))
    {
        Safe_Release(pWeaponImg);
        MSG_BOX("CWeightText Create Failed");
        return nullptr;
    }

    return pWeaponImg;
}

void CWeightText::Free()
{
    CGameObject::Free();
}

void CWeightText::OnNotify(const Event& e)
{
    switch (e.type)
    {
    case EVENTTYPE::CHANGE_WEIGTH:
        m_fPlayerCurWeight = e.fValue;
        m_fPlayerMaxWeight = e.fValue2;
        break;
    default:
        break;
    }
}
