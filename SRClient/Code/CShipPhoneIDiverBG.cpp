#include "pch.h"
#include "CShipPhoneIDiverBG.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CShipPhoneIDiverItem.h"
#include "CManagement.h"

CShipPhoneIDiverBG::CShipPhoneIDiverBG()
	: CGameObject()
{
}

CShipPhoneIDiverBG::~CShipPhoneIDiverBG()
{
}


HRESULT		CShipPhoneIDiverBG::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_PhoneIDiverBG"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 284.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 260.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }

    {
        CShipPhoneIDiverItem* pShipItem = CShipPhoneIDiverItem::Create(0.f, 2.f);
        pShipItem->Set_Parent(this);
        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneIDIverItem", pShipItem);
    }

    {
        CShipPhoneIDiverItem* pShipItem = CShipPhoneIDiverItem::Create(0.f, 0.7f);
        pShipItem->Set_Parent(this);
        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneIDIverItem", pShipItem);
    }

    {
        CShipPhoneIDiverItem* pShipItem = CShipPhoneIDiverItem::Create(0.f, -0.6f);
        pShipItem->Set_Parent(this);
        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneIDIverItem", pShipItem);
    }

    {
        CShipPhoneIDiverItem* pShipItem = CShipPhoneIDiverItem::Create(0.f, -1.9f);
        pShipItem->Set_Parent(this);
        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneIDIverItem", pShipItem);
    }


    //CShipPhoneIDiverItem* pShipItem = CShipPhoneIDiverItem::Create();
    //if (nullptr == pShipPhoneIcon)
    //    return E_FAIL;
    //if (FAILED(pLayer->Add_GameObject(L"ShipPhoneIDIverItem", pShipItem)))
    //    return E_FAIL;


    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(0.f, 0.1f, 0.f);
    m_pTransformCom->Set_Scale(&vScale);
    return S_OK;    
}

_int		CShipPhoneIDiverBG::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CShipPhoneIDiverBG::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipPhoneIDiverBG::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_PhoneIDiverBG"))
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

HRESULT			CShipPhoneIDiverBG::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipPhoneIDiverBG* CShipPhoneIDiverBG::Create()
{
    CShipPhoneIDiverBG* pIDiverBG = new CShipPhoneIDiverBG;

    if (FAILED(pIDiverBG->Ready_GameObject()))
    {
        Safe_Release(pIDiverBG);
        MSG_BOX("pIDiverBG Create Failed");
        return nullptr;
    }

    return pIDiverBG;
}

void CShipPhoneIDiverBG::Free()
{
    CGameObject::Free();
}