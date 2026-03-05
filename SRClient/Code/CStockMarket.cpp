#include "pch.h"
#include "CStockMarket.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"
#include "CShipPhone.h"
#include "CGameMemMgr.h"

CStockMarket::CStockMarket()
    : CGameObject()
{
    if (CGameMemMgr::GetInstance()->Get_DiveInfos().size() == 0) {
        CGameMemMgr::GetInstance()->SetStockMarketIndex(0);
        //pictureIndex = 0;
    }else if (CGameMemMgr::GetInstance()->Get_DiveInfos().size() == 1 ) {
        CGameMemMgr::GetInstance()->SetStockMarketIndex(5);
        //pictureIndex = 5;
    }else if (CGameMemMgr::GetInstance()->Get_DiveInfos().size() == 2) {
        CGameMemMgr::GetInstance()->SetStockMarketIndex(13);
       // pictureIndex = 13;
    }

    pictureOriginalName = L"Tex_stockMarket";
}

CStockMarket::~CStockMarket()
{
}


HRESULT		CStockMarket::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_stockMarket"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 284.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 260.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }



    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(0.f, 0.1f, 0.f);
    m_pTransformCom->Set_Scale(&vScale);
    return S_OK;
}

_int		CStockMarket::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    if (auto pPhone = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst<CShipPhone>(L"ShipPhone"))
    {
        if (CDInputMgr::GetInstance()->Key_Down(DIK_C))
        {
            pPhone->UnFocus_App();
        }
    }

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
  
    if (CGameMemMgr::GetInstance()->GetStockMarketIndex() > 0) {
        pictureName = pictureOriginalName;
        pictureName = pictureOriginalName + to_wstring(CGameMemMgr::GetInstance()->GetStockMarketIndex());
    }
    else if(CGameMemMgr::GetInstance()->GetStockMarketIndex() == 0){
        pictureName = pictureOriginalName;
    }
    if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB)) {
        CGameMemMgr::GetInstance()->AddStockMarketIndex();
        moneyChanged = false;
    }

    if (!moneyChanged) {
        if (CGameMemMgr::GetInstance()->GetStockMarketIndex() == 3) {
            CGameMemMgr::GetInstance()->Set_Money2(0);
            moneyChanged = true;
        }else if (CGameMemMgr::GetInstance()->GetStockMarketIndex() == 8) {
            CGameMemMgr::GetInstance()->Set_Money2(2850);
            moneyChanged = true;
        }else if (CGameMemMgr::GetInstance()->GetStockMarketIndex() == 11) {
            CGameMemMgr::GetInstance()->Set_Money2(-99999);
            moneyChanged = true;
        }
    }
    return iExit;
}

void		CStockMarket::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void		CStockMarket::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(pictureName))
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

HRESULT			CStockMarket::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CStockMarket* CStockMarket::Create()
{
    CStockMarket* pIDiverBG = new CStockMarket;

    if (FAILED(pIDiverBG->Ready_GameObject()))
    {
        Safe_Release(pIDiverBG);
        MSG_BOX("pIDiverBG Create Failed");
        return nullptr;
    }

    return pIDiverBG;
}

void CStockMarket::Free()
{
    CGameObject::Free();
}