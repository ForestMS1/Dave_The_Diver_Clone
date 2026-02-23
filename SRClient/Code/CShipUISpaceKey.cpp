#include "pch.h"
#include "CShipUISpaceKey.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"

CShipUISpaceKey::CShipUISpaceKey(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CShipUISpaceKey::~CShipUISpaceKey()
{
}


HRESULT		CShipUISpaceKey::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_UI_SpaceKey"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 400.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 400.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }

    m_bAddRender = false;

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);
    return S_OK;
}

_int		CShipUISpaceKey::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    if (m_bAddRender)
    {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    }

    return iExit;
}

void		CShipUISpaceKey::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipUISpaceKey::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_UI_SpaceKey"))
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

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT			CShipUISpaceKey::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipUISpaceKey* CShipUISpaceKey::Create(float fPosX, float fPosY)
{
    CShipUISpaceKey* pShipSpaceKey = new CShipUISpaceKey{ fPosX , fPosY };

    if (FAILED(pShipSpaceKey->Ready_GameObject()))
    {
        Safe_Release(pShipSpaceKey);
        MSG_BOX("pShipSpaceKey Create Failed");
        return nullptr;
    }

    return pShipSpaceKey;
}

void CShipUISpaceKey::Free()
{
    CGameObject::Free();
}