#include "pch.h"
#include "CShipUIMoney.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CGameMemMgr.h"

CShipUIMoney::CShipUIMoney(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CShipUIMoney::~CShipUIMoney()
{
}

void CShipUIMoney::Update_ImGui()
{
    ImGui::DragFloat("fontXOffset", &m_fFontOffsetX, 0.1);
    ImGui::DragFloat("fontYOffset", &m_fFontOffsetY, 0.1);
}


HRESULT		CShipUIMoney::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_UI_Money"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 180.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 180.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fFontOffsetX = -0.7f;
    m_fFontOffsetY = 0.1f;
    return S_OK;
}

_int		CShipUIMoney::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CShipUIMoney::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipUIMoney::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_UI_Money"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }

    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    _vec3 vInfoPos;
    m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);

    vInfoPos.x += m_fFontOffsetX;
    vInfoPos.y += m_fFontOffsetY;

    _vec3 vScreenPos;
    CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);


    _vec2 vPos = { vScreenPos.x , vScreenPos.y};
    if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
    {
        
        pDefFont->Render_Font(CHelper::ToCommaWString(CGameMemMgr::GetInstance()->Get_Money()), &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    }
   

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT			CShipUIMoney::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipUIMoney* CShipUIMoney::Create(float fPosX, float fPosY)
{
    CShipUIMoney* pIDiverUpgrade = new CShipUIMoney{ fPosX , fPosY };

    if (FAILED(pIDiverUpgrade->Ready_GameObject()))
    {
        Safe_Release(pIDiverUpgrade);
        MSG_BOX("pIDiverUpgrade Create Failed");
        return nullptr;
    }

    return pIDiverUpgrade;
}

void CShipUIMoney::Free()
{
    CGameObject::Free();
}