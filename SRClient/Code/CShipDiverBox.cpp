#include "pch.h"
#include "CShipDiverBox.h"
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CShipDiverBoxInventory.h"
#include "CManagement.h"

CShipDiverBox::CShipDiverBox()
    : CGameObject()
{
}

CShipDiverBox::CShipDiverBox(const CShipDiverBox& rhs)
    : CGameObject(rhs)
{
}
CShipDiverBox::~CShipDiverBox()
{
}


HRESULT		CShipDiverBox::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };
    _vec3 vScale = { 1.0f, 1.0f, 1.0f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_DiverBox"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 100.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 100.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }

    m_fViewZ = 1.1f;

    _vec3 vPos = { 0.0f, 0.0f, 0.0f };
    m_pTransformCom->Set_Scale(&vScale);

   
    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Dave", this);
    return S_OK;
}

_int		CShipDiverBox::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);


    if (ImGui::Button("open diver box inventory"))
    {
        auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer");
        CShipDiverBoxInventory* pShipDiverBoxInventory = CShipDiverBoxInventory::Create();
        if (nullptr == pShipDiverBoxInventory)
	        return E_FAIL;
        if (FAILED(pLayer->Add_GameObject(L"ShipDiverBoxInventory", pShipDiverBoxInventory)))
	        return E_FAIL;
    }
    

        //


    return iExit;
}

void		CShipDiverBox::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipDiverBox::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_DiverBox"))
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

    pGraphicDev->SetTexture(0, nullptr);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT			CShipDiverBox::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}
CShipDiverBox* CShipDiverBox::Create()
{
    CShipDiverBox* pShipDiverBox = new CShipDiverBox;

    if (FAILED(pShipDiverBox->Ready_GameObject()))
    {
        Safe_Release(pShipDiverBox);
        MSG_BOX("pShipDiverBox Create Failed");
        return nullptr;
    }

    return pShipDiverBox;
}

void CShipDiverBox::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}
