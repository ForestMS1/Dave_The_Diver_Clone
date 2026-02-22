#include "pch.h"
#include "CShipDiverBoxInventory.h"
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CRenderer.h"

CShipDiverBoxInventory::CShipDiverBoxInventory()
    : CGameObject()
{
}

CShipDiverBoxInventory::CShipDiverBoxInventory(const CShipDiverBoxInventory& rhs)
    : CGameObject(rhs)
{
}
CShipDiverBoxInventory::~CShipDiverBoxInventory()
{
}


HRESULT		CShipDiverBoxInventory::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };

    _vec3 vPos = { 00.0f, 0.0f, 0.0f };
    _vec3 v = { 2.f , 2.f, 2.f };
    m_pTransformCom->Set_Scale(&v);
    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Dave", this);
    return S_OK;
}

_int		CShipDiverBoxInventory::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CShipDiverBoxInventory::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipDiverBoxInventory::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_DiverBoxInvenTmp"))
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
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT			CShipDiverBoxInventory::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}
CShipDiverBoxInventory* CShipDiverBoxInventory::Create()
{
    CShipDiverBoxInventory* pShipDiverBox = new CShipDiverBoxInventory;

    if (FAILED(pShipDiverBox->Ready_GameObject()))
    {
        Safe_Release(pShipDiverBox);
        MSG_BOX("pShipDiverBox Create Failed");
        return nullptr;
    }

    return pShipDiverBox;
}

void CShipDiverBoxInventory::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}
