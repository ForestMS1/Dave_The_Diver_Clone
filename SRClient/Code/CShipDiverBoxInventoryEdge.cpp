#include "pch.h"
#include "CShipDiverBoxInventoryEdge.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"

CShipDiverBoxInventoryEdge::CShipDiverBoxInventoryEdge(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CShipDiverBoxInventoryEdge::~CShipDiverBoxInventoryEdge()
{
}


HRESULT		CShipDiverBoxInventoryEdge::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_InventoryBoxEdge"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
           // 92 88;
            auto a = pTexture->Get_ImgInfo()->Width;
            auto b = pTexture->Get_ImgInfo()->Height * 2;
            float fWidth = float(pTexture->Get_ImgInfo()->Width) / 190.f;
            float fHeight = float(pTexture->Get_ImgInfo()->Height) / 190.f;
            m_fFirstScaleX = fWidth;
            m_fFirstScaleY = fHeight;
            vScale = { fWidth, fHeight, 1.f };
        }
    }
    m_fCustomScaleX = 1.f;
    m_fCustomScaleY = 1.f;
    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(0.f, -10.f, 0.f);
    m_pTransformCom->Set_Scale(&vScale);


    m_fViewZ = 0.47f;
    return S_OK;
}

_int		CShipDiverBoxInventoryEdge::Update_GameObject(const _float& fTimeDelta)
{
    _vec3 vPos;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

    float fOffsetX = m_fPosX;
    float fOffsetY = m_fPosY;
    vPos.x += fOffsetX;
    vPos.y += fOffsetY;

    m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
    _vec3 vScale = { m_fFirstScaleX * m_fCustomScaleX, m_fFirstScaleY * m_fCustomScaleY, 0.f };
    m_pTransformCom->Set_Scale(&vScale);
    
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
    return iExit;
}

void		CShipDiverBoxInventoryEdge::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipDiverBoxInventoryEdge::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_InventoryBoxEdge"))
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

HRESULT			CShipDiverBoxInventoryEdge::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipDiverBoxInventoryEdge* CShipDiverBoxInventoryEdge::Create(float fPosX, float fPosY)
{
    CShipDiverBoxInventoryEdge* pIDiverUpgrade = new CShipDiverBoxInventoryEdge{ fPosX , fPosY };

    if (FAILED(pIDiverUpgrade->Ready_GameObject()))
    {
        Safe_Release(pIDiverUpgrade);
        MSG_BOX("pIDiverUpgrade Create Failed");
        return nullptr;
    }

    return pIDiverUpgrade;
}

void CShipDiverBoxInventoryEdge::Free()
{
    CGameObject::Free();
}