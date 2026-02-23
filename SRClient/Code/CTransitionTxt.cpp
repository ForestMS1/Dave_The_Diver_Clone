#include "pch.h"
#include "CTransitionTxt.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CColliderMgr.h"
#include "CAssetDefaultFont.h"

CTransitionTxt::CTransitionTxt(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_sTxt({})
    , m_dwOpt(DT_NOCLIP)
    , m_color(D3DXCOLOR(1.f, 1.f, 1.f, 1.f))
    , m_sFont(L"Font_210YouthL_Size15")
{
}

CTransitionTxt::~CTransitionTxt()
{
}


HRESULT		CTransitionTxt::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    return S_OK;
}

void CTransitionTxt::Ready_AfterCreate()
{
    _vec3 vPos;
    GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

    float fOffsetX = m_fPosX;
    float fOffsetY = m_fPosY;
    vPos.x += fOffsetX;
    vPos.y += fOffsetY;

    m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);

    m_pTransformCom->Update_Component(0.f);
}

_int		CTransitionTxt::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);




    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CTransitionTxt::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CTransitionTxt::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();



    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    // Desc
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.0f;
        float fOffsetY = -0.0f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(m_sFont))
        {
            pDefFont->Render_Font(m_sTxt, &vPos, m_color, m_dwOpt);
        }
    }


    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
}

HRESULT			CTransitionTxt::Ready_Component()
{

    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CTransitionTxt* CTransitionTxt::Create(float fPosX, float fPosY)
{
    CTransitionTxt* pWeaponImg = new CTransitionTxt{ fPosX , fPosY };

    if (FAILED(pWeaponImg->Ready_GameObject()))
    {
        Safe_Release(pWeaponImg);
        MSG_BOX("pWeaponImg Create Failed");
        return nullptr;
    }

    return pWeaponImg;
}

void CTransitionTxt::Free()
{
    CGameObject::Free();
}