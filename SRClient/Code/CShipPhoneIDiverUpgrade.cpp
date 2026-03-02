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
#include "CGameMemMgr.h"
#include "IDiverInfo.h"

CShipPhoneIDiverUpgrade::CShipPhoneIDiverUpgrade(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_sAssetName({})
    , m_sTitle({})
    , m_sLeftTop({})
    , m_sLeftMiddle({})
    , m_sLeftBottom({})
    , m_sRightTop({})
    , m_sRightMiddle({})
    , m_sRightBottom({})
    , m_sDesc({})
{
}

CShipPhoneIDiverUpgrade::~CShipPhoneIDiverUpgrade()
{
}

void CShipPhoneIDiverUpgrade::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_fDbgX", &m_fDbgX, 0.01f);
    ImGui::DragFloat("m_fDbgY", &m_fDbgY, 0.01f);
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



    return S_OK;
}

void CShipPhoneIDiverUpgrade::Ready_AfterCreate()
{
    CShipPhoneIDiverUpgradeBtn* pUpBtn = CShipPhoneIDiverUpgradeBtn::Create(-0.8f, -2.45f);
    pUpBtn->Set_Parent(this);
    pUpBtn->Set_AssetName(L"Tex_Ship_IDiver_UpgradeBtnAlpha");

    m_pUpBtn = pUpBtn;

    CManagement::GetInstance()
        ->Get_Scene()
        ->Get_Layer(L"0_GameLogic_Layer")
        ->Add_GameObject(L"ShipPhoneIDiverUpgradeBtn", pUpBtn);



    CShipPhoneIDiverItemImg* pImg = CShipPhoneIDiverItemImg::Create(0.f, 1.75f);
    pImg->Set_Parent(this);
    pImg->Set_AssetName(m_sAssetName);
    pImg->Set_ViewZ(0.29f);
    CManagement::GetInstance()
        ->Get_Scene()
        ->Get_Layer(L"0_GameLogic_Layer")
        ->Add_GameObject(L"ShipPhoneIDiverUpgradeImg", pImg);
}

_int		CShipPhoneIDiverUpgrade::Update_GameObject(const _float& fTimeDelta)
{


    _uint iCurrLevel = CGameMemMgr::GetInstance()->Get_IDiverCurrentLevel()->at(m_sAssetName);

    if (IDiver::Get_Info(m_sAssetName, iCurrLevel).sLevel == L"MAX" || IDiver::Get_Info(m_sAssetName, iCurrLevel).sLevel == L"최종병기작살")
    {
        m_pUpBtn->Set_AssetName(L"Tex_Ship_IDiver_UpgradeBtnAlpha");
    }
    else
    {
        if (CGameMemMgr::GetInstance()->Get_Money() > IDiver::Get_Info(m_sAssetName, iCurrLevel + 1).iCost)
        {
            m_pUpBtn->Set_AssetName(L"Tex_Ship_IDiver_UpgradeBtn");
        }
        else
        {
            m_pUpBtn->Set_AssetName(L"Tex_Ship_IDiver_UpgradeBtnAlpha");
        }
    }
    
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
            _uint iCurrLevel = CGameMemMgr::GetInstance()->Get_IDiverCurrentLevel()->at(m_sAssetName);
            if (CGameMemMgr::GetInstance()->Get_Money() > IDiver::Get_Info(m_sAssetName, iCurrLevel + 1).iCost)
            {
                CGameMemMgr::GetInstance()->Set_Money2(CGameMemMgr::GetInstance()->Get_Money() - IDiver::Get_Info(m_sAssetName, iCurrLevel+1).iCost);

                if (
                    !(IDiver::Get_Info(m_sAssetName, iCurrLevel).sLevel == L"MAX"
                        || IDiver::Get_Info(m_sAssetName, iCurrLevel).sLevel == L"최종병기작살"))
                {
                    CGameMemMgr::GetInstance()->Set_IDiverCurrentLevel(m_sAssetName, iCurrLevel + 1);
                    //iCurrLevel = CGameMemMgr::GetInstance()->Get_IDiverCurrentLevel()->at(m_sAssetName);

                    CShipPhoneIDiverUpgradeSuccess* pUpSucess = CShipPhoneIDiverUpgradeSuccess::Create(0.f, 0.f);
                    pUpSucess->Set_Parent(m_pParentGameObject);

                    pUpSucess->Set_Title(L"공기통");
                    pUpSucess->Set_Top(L"LV 1");
                    pUpSucess->Set_Bottom(L"공기량 111");
                    pUpSucess->Set_Desc(L"더 오래 잠수 할수 있습니다.");
                    pUpSucess->Set_AssetName(m_sAssetName);

                    pUpSucess->Ready_AfterCreate();

                    CManagement::GetInstance()
                        ->Get_Scene()
                        ->Get_Layer(L"0_GameLogic_Layer")
                        ->Add_GameObject(L"ShipPhoneIDiverUpgradeSuccess", pUpSucess);

                    Set_DeadCascade();
                    return OBJ_DEAD;
                }
            }
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


    _uint iCurrLevel = CGameMemMgr::GetInstance()->Get_IDiverCurrentLevel()->at(m_sAssetName);

    ;
    // Title
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.f;
        float fOffsetY = 2.53f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel).sTitle, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // LeftTop
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.93f;
        float fOffsetY = 0.96f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel).sLevel, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // LeftMiddle
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.93f;
        float fOffsetY = 0.64f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel).sUnitName, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // LeftBottom
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.93f;
        float fOffsetY = 0.36f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel).sUnit, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }


    // RgihtTop
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.93f;
        float fOffsetY = 0.96f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel + 1).sLevel, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // RgihtMiddle
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.93f;
        float fOffsetY = 0.64f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel + 1).sUnitName, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // RgihtBottom
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.93f;
        float fOffsetY = 0.36f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel + 1).sUnit, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
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
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel).sUpgradeDesc, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f),(DT_CENTER | DT_VCENTER  | DT_NOCLIP));
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
            D3DXCOLOR col;
            if (CGameMemMgr::GetInstance()->Get_Money() > IDiver::Get_Info(m_sAssetName, iCurrLevel).iCost)
            {
                col = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
            }
            else
            {
                col = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
            }
            pDefFont->Render_Font(to_wstring(IDiver::Get_Info(m_sAssetName, iCurrLevel + 1).iCost), &vPos, col);
            //pDefFont->Render_Font(L"123", &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
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