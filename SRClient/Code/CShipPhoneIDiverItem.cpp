#include "pch.h"
#include "CShipPhoneIDiverItem.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CShipPhoneIDiverItemImg.h"
#include "CAssetDefaultFont.h"
#include "CColliderMgr.h"
#include "CGameMemMgr.h"
#include "IDiverInfo.h"

CShipPhoneIDiverItem::CShipPhoneIDiverItem(_int iIdx, float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_iIdx(iIdx)
{
}

CShipPhoneIDiverItem::~CShipPhoneIDiverItem()
{
}

void CShipPhoneIDiverItem::Ready_AfterCreate()

{
    CShipPhoneIDiverItemImg* pItemImg = CShipPhoneIDiverItemImg::Create(-1.5f, 0.15f);
    pItemImg->Set_Parent(this);
    pItemImg->Set_AssetName(m_sAssetName);
    CManagement::GetInstance()
        ->Get_Scene()
        ->Get_Layer(L"0_GameLogic_Layer")
        ->Add_GameObject(L"ShipPhoneIDIverItemImg", pItemImg);
}


HRESULT		CShipPhoneIDiverItem::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_UI_IDiverItem"))
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



    float refX = 0.92f / 2.f;
    float refY = 0.88f / 2.f;
    _vec3 vExtents = { 1.f, 1.f, 0.01f };
    _vec3 vPos = { 0.0f, 0.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_IDiverItem", this);

    return S_OK;
}

_int		CShipPhoneIDiverItem::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_IDiver", m_pAABB);
    m_pAABB->Transform(m_pTransformCom->Get_World());

    return iExit;
}

void		CShipPhoneIDiverItem::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipPhoneIDiverItem::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_UI_IDiverItem"))
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

    // Title
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.f;
        float fOffsetY = 0.5f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel).sTitle, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
        }
    }


    // LEFT TOP
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.f;
        float fOffsetY = 0.11f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel).sLevel, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        }
    }

    // LEFT MIDDLE
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.f;
        float fOffsetY = -0.08;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel).sUnitName, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        }
    }


    // LEFT BOTTOM
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.f;
        float fOffsetY = -0.3f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel).sUnit, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        }
    }


    // RIGHT TOP
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.7f;
        float fOffsetY = 0.11f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel + 1).sLevel, &vPos, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
        }
    }

    // RIGHT MIDDLE
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.7f;
        float fOffsetY = -0.08;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel+1).sUnitName, &vPos, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
        }
    }


    // RIGHT BOTTOM
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.7f;
        float fOffsetY = -0.3f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(IDiver::Get_Info(m_sAssetName, iCurrLevel+1).sUnit, &vPos, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
        }
    }

    // Money
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -1.5f;
        float fOffsetY = -0.34f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size10"))
        {
            D3DXCOLOR col;
            //if (m_bMoneyLack)
            //{
            //    col = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
            //}
            //else
            //{
            //    col = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
            //}
            if (IDiver::Get_Info(m_sAssetName, iCurrLevel).sLevel == L"MAX"|| IDiver::Get_Info(m_sAssetName, iCurrLevel).sLevel == L"최종병기작살")
            {
                pDefFont->Render_Font(L"MAX", &vPos, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
            }
            else
            {
                if (CGameMemMgr::GetInstance()->Get_Money() > IDiver::Get_Info(m_sAssetName, iCurrLevel + 1).iCost)
                {
                    col = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
                }
                else
                {
                    col = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
                }
                pDefFont->Render_Font(to_wstring(IDiver::Get_Info(m_sAssetName, iCurrLevel + 1).iCost), &vPos, col);
            }
            
        }
    }

}

HRESULT			CShipPhoneIDiverItem::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipPhoneIDiverItem* CShipPhoneIDiverItem::Create(_int iIdx, float fPosX, float fPosY)
{
    CShipPhoneIDiverItem* pIDiverItem = new CShipPhoneIDiverItem{ iIdx, fPosX , fPosY };

    if (FAILED(pIDiverItem->Ready_GameObject()))
    {
        Safe_Release(pIDiverItem);
        MSG_BOX("pIDiverItem Create Failed");
        return nullptr;
    }



    return pIDiverItem;
}

void CShipPhoneIDiverItem::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}