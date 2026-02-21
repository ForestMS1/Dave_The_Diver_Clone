#include "pch.h"
#include "CShipPhoneIDiverUpgrade.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CDInputMgr.h"

#include "CManagement.h"
#include "CShipPhoneIDiverUpgradeSuccess.h"
#include "CShipPhoneIDiverUpgradeBtn.h"

#include "CShipPhoneIDiverItemImg.h"

CShipPhoneIDiverUpgrade::CShipPhoneIDiverUpgrade(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CShipPhoneIDiverUpgrade::~CShipPhoneIDiverUpgrade()
{
}


HRESULT		CShipPhoneIDiverUpgrade::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_UI_IDiverUpgrade"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 284.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 260.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fViewZ = 0.3f;


    CShipPhoneIDiverUpgradeBtn* pUpBtn = CShipPhoneIDiverUpgradeBtn::Create(-0.8f, -2.45f);
    pUpBtn->Set_Parent(this);
    pUpBtn->Set_AssetName(L"Tex_Ship_IDiver_UpgradeBtn");

    CManagement::GetInstance()
        ->Get_Scene()
        ->Get_Layer(L"0_GameLogic_Layer")
        ->Add_GameObject(L"ShipPhoneIDiverUpgradeBtn", pUpBtn);



    CShipPhoneIDiverItemImg* pImg = CShipPhoneIDiverItemImg::Create(0.f, 2.f);
    pImg->Set_Parent(this);
    pImg->Set_AssetName(L"Tex_Ship_IDiver_Item_Sanso");
    pImg->Set_ViewZ(0.29f);
    CManagement::GetInstance()
        ->Get_Scene()
        ->Get_Layer(L"0_GameLogic_Layer")
        ->Add_GameObject(L"ShipPhoneIDiverUpgradeImg", pImg);
    return S_OK;
}

_int		CShipPhoneIDiverUpgrade::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    //m_bSuccessOpen
    auto pExists = CManagement::GetInstance()
        ->Get_Scene()
        ->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"ShipPhoneIDiverUpgradeSuccess");

    if (pExists)
    {
        m_bSuccessOpen = true;
    }
    else
    {
        m_bSuccessOpen = false;
    }

    if (!m_bSuccessOpen)
    {
        if (CDInputMgr::GetInstance()->Key_Down(DIK_C))
        {
            Set_DeadCascade();
        }
    }
    

    if (CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
    {
        if (!m_bSuccessOpen)
        {
            CShipPhoneIDiverUpgradeSuccess* pUpSucess = CShipPhoneIDiverUpgradeSuccess::Create(0.f, 0.f);
            pUpSucess->Set_Parent(this);

            CManagement::GetInstance()
                ->Get_Scene()
                ->Get_Layer(L"0_GameLogic_Layer")
                ->Add_GameObject(L"ShipPhoneIDiverUpgradeSuccess", pUpSucess);
        }
    }


    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CShipPhoneIDiverUpgrade::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipPhoneIDiverUpgrade::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();


    


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_UI_IDiverUpgrade"))
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



    // Title
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.3f;
        float fOffsetY = 2.7f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"제목", &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
        }
    }

    // LeftTop
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.2f;
        float fOffsetY = 1.1f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"Lv1", &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
        }
    }

    // LeftMiddle
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.2f;
        float fOffsetY = 0.8f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(L"asdf", &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
        }
    }

    // LeftBottom
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.2f;
        float fOffsetY = 0.5f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(L"11 f", &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
        }
    }


    // RgihtTop
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.7f;
        float fOffsetY = 1.1f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"Lv1", &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
        }
    }

    // RgihtMiddle
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.7f;
        float fOffsetY = 0.8f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(L"asdf", &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
        }
    }

    // RgihtBottom
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.7f;
        float fOffsetY = 0.5f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(L"11 f", &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
        }
    }


    // Desc
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.0f;
        float fOffsetY = -0.5f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(L"DESCdsafsdfasdf", &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f),(DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }


    // Money
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.2f;
        float fOffsetY = -1.8f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
           
            pDefFont->Render_Font(L"123", &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
        }
    }


}

HRESULT			CShipPhoneIDiverUpgrade::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipPhoneIDiverUpgrade* CShipPhoneIDiverUpgrade::Create(float fPosX, float fPosY)
{
    CShipPhoneIDiverUpgrade* pIDiverUpgrade = new CShipPhoneIDiverUpgrade{ fPosX , fPosY };

    if (FAILED(pIDiverUpgrade->Ready_GameObject()))
    {
        Safe_Release(pIDiverUpgrade);
        MSG_BOX("pIDiverUpgrade Create Failed");
        return nullptr;
    }

    return pIDiverUpgrade;
}

void CShipPhoneIDiverUpgrade::Free()
{
    CGameObject::Free();
}