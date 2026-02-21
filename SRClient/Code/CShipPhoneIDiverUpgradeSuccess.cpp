#include "pch.h"
#include "CShipPhoneIDiverUpgradeSuccess.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CDInputMgr.h"
#include "CShipPhoneIDiverItemImg.h"
#include "CManagement.h"

CShipPhoneIDiverUpgradeSuccess::CShipPhoneIDiverUpgradeSuccess(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CShipPhoneIDiverUpgradeSuccess::~CShipPhoneIDiverUpgradeSuccess()
{
}


HRESULT		CShipPhoneIDiverUpgradeSuccess::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_IDiver_UpgradeSuccess"))
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

    m_fViewZ = 0.25f;


    CShipPhoneIDiverItemImg* pImg = CShipPhoneIDiverItemImg::Create(0.f, 1.f);
    pImg->Set_Parent(this);
    pImg->Set_AssetName(L"Tex_Ship_IDiver_Item_Sanso");
    pImg->Set_ViewZ(0.24f);
    CManagement::GetInstance()
        ->Get_Scene()
        ->Get_Layer(L"0_GameLogic_Layer")
        ->Add_GameObject(L"ShipPhoneIDiverUpgradeImg", pImg);
    return S_OK;
}

_int		CShipPhoneIDiverUpgradeSuccess::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    if (CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
    {
        Set_DeadCascade();
    }


    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CShipPhoneIDiverUpgradeSuccess::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipPhoneIDiverUpgradeSuccess::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();





    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_IDiver_UpgradeSuccess"))
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
        float fOffsetX = -0.0f;
        float fOffsetY = 0.4f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(L"Á¦¸ðÄí", &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // TOP
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.0f;
        float fOffsetY = 0.0f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(L"LV2", &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // Bottom
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.0f;
        float fOffsetY = -0.4f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(L"±ÜÀû±ÛÀû", &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

    // Desc
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.0f;
        float fOffsetY = -1.0f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(L"DESCDESC", &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

}

HRESULT			CShipPhoneIDiverUpgradeSuccess::Ready_Component()
{
    // ¹öÆÛ
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipPhoneIDiverUpgradeSuccess* CShipPhoneIDiverUpgradeSuccess::Create(float fPosX, float fPosY)
{
    CShipPhoneIDiverUpgradeSuccess* pIDiverUpgrade = new CShipPhoneIDiverUpgradeSuccess{ fPosX , fPosY };

    if (FAILED(pIDiverUpgrade->Ready_GameObject()))
    {
        Safe_Release(pIDiverUpgrade);
        MSG_BOX("pIDiverUpgrade Create Failed");
        return nullptr;
    }

    return pIDiverUpgrade;
}

void CShipPhoneIDiverUpgradeSuccess::Free()
{
    CGameObject::Free();
}