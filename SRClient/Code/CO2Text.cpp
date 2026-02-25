#include "pch.h"
#include "CO2Text.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CColliderMgr.h"
#include "CAssetDefaultFont.h"

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


HRESULT		CO2Text::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;
    CAssetMgr::GetInstance()->AddAsset(L"Font_210YouthL_Size50", CAssetDefaultFont::Create(L"../Bin/Resource/Font/210YouthL.ttf", L"210 ¸Ç¹ßÀÇÃ»Ãá L", 0, 50, FW_HEAVY));
    CAssetMgr::GetInstance()->LoadAsset(L"Font_210YouthL_Size50");
    m_sFont = L"Font_210YouthL_Size50";
    m_pTransformCom->Set_Pos(98.f, 780.f, 0.1f);
    return S_OK;
}

_int		CO2Text::Update_GameObject(const _float& fTimeDelta)
{
    if(m_iPlayerHp >= 100)
        m_pTransformCom->Set_Pos(85.f, 780.f, 0.1f);
    else
        m_pTransformCom->Set_Pos(98.f, 780.f, 0.1f);
    m_sTxt = to_wstring(m_iPlayerHp);
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    return iExit;
}

void		CO2Text::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CO2Text::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    _vec3 vInfoPos;
    m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);

    _vec2 vPos = { vInfoPos.x , vInfoPos.y };
    if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(m_sFont))
    {
        //pDefFont->Render_Font(m_sTxt, &vPos, m_color, m_dwOpt);
        pDefFont->Render_Font(m_sTxt, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    }
}

HRESULT			CO2Text::Ready_Component()
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
    default:
        break;
    }
}
