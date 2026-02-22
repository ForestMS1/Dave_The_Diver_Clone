#include "pch.h"
#include "CShipPhoneWeaponArea.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"
#include "CShipPhone.h"
#include "CGameMemMgr.h"
#include "CShipPhoneWeaponImg.h"
#include "CAssetDefaultFont.h"
#include "CShipPhoneWeaponBG.h"

CShipPhoneWeaponArea::CShipPhoneWeaponArea(_uint iIdx, float fPosX, float fPosY)
    : CGameObject()
    , m_iIdx(iIdx)
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CShipPhoneWeaponArea::~CShipPhoneWeaponArea()
{
}


HRESULT		CShipPhoneWeaponArea::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };

    m_pTransformCom->Set_Pos(0.f, 0.1f, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    //m_fViewZ = 0.4;

    return S_OK;
}

void CShipPhoneWeaponArea::Ready_AfterCreate()
{
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    CShipPhoneWeaponImg* pImg = CShipPhoneWeaponImg::Create(m_fPosX, m_fPosY);
    pImg->Set_Parent(this);
    pImg->Set_AssetName(L"Tex_Ship_Weapon_UI_Area");

    _vec3 vExt = { 1.f, 1.f, 0.01f };
    _vec3 vPos = { 0.f, 0.f, 0.f };
    pImg->Set_Aabb(CAABB::Create(&vPos, &vExt, L"AABB_WeaponArea_" + to_wstring(m_iIdx), this));
    pImg->Set_ColliderGroup(L"Coll_Weapon");

    pImg->Ready_AfterCreate();

    {
        CShipPhoneWeaponImg* pWeaponImg = CShipPhoneWeaponImg::Create(0.f, 0.3f);
        pWeaponImg->Set_Parent(pImg);
        pWeaponImg->Set_AssetName(L"Tex_Ship_Weapon_GukikRifle");

        pWeaponImg->Ready_AfterCreate();

        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneWeaponImg", pWeaponImg);
    }


    {
        CShipPhoneWeaponImg* pWeaponImg = CShipPhoneWeaponImg::Create(0.f, -0.4f);
        pWeaponImg->Set_Parent(pImg);
        pWeaponImg->Set_AssetName(L"Tex_Ship_Weapon_UI_CreateBtn");

        pWeaponImg->Ready_AfterCreate();

        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneWeaponImg", pWeaponImg);
    }

    CManagement::GetInstance()
        ->Get_Scene()
        ->Get_Layer(L"0_GameLogic_Layer")
        ->Add_GameObject(L"ShipPhoneWeaponArea", pImg);
}

_int		CShipPhoneWeaponArea::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);





    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);



    return iExit;
}

void		CShipPhoneWeaponArea::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);


}

void		CShipPhoneWeaponArea::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();



    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_Weapon_BG"))
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


    
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -0.0f;
        float fOffsetY = -0.1f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            if (m_iIdx == 0)
            {
                pDefFont->Render_Font(L"±¸½Ä¶óÀÌÇÃ", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
            }
            else if (m_iIdx == 1)
            {
                pDefFont->Render_Font(L"¹º°¡ÃÑ", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
            }
            
        }
    }

}

HRESULT			CShipPhoneWeaponArea::Ready_Component()
{
    // ¹öÆÛ
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipPhoneWeaponArea* CShipPhoneWeaponArea::Create(_uint iIdx, float fPosX, float fPosY)
{
    CShipPhoneWeaponArea* pIDiverBG = new CShipPhoneWeaponArea{ iIdx, fPosX, fPosY };

    if (FAILED(pIDiverBG->Ready_GameObject()))
    {
        Safe_Release(pIDiverBG);
        MSG_BOX("pIDiverBG Create Failed");
        return nullptr;
    }

    return pIDiverBG;
}

void CShipPhoneWeaponArea::Free()
{
    CGameObject::Free();

}