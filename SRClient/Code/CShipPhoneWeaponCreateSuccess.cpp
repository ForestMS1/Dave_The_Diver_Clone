#include "pch.h"
#include "CShipPhoneWeaponCreateSuccess.h"
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
#include "CShipPhoneWeaponArea.h"
#include "CAssetDefaultFont.h"
#include "CShipPhoneWeaponBG.h"

CShipPhoneWeaponCreateSuccess::CShipPhoneWeaponCreateSuccess()
    : CGameObject()
{
}

CShipPhoneWeaponCreateSuccess::~CShipPhoneWeaponCreateSuccess()
{
}


HRESULT		CShipPhoneWeaponCreateSuccess::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_Weapon_UI_NewWeapon"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 296.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 308.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }


    m_pTransformCom->Set_Pos(0.f, 0.1f, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fViewZ = 0.45f;

    
    return S_OK;
}

HRESULT CShipPhoneWeaponCreateSuccess::Ready_AfterCreate()
{
    {
        CShipPhoneWeaponImg* pWeaponImg = CShipPhoneWeaponImg::Create(0.f, 0.8f);
        pWeaponImg->Set_Parent(this);
        pWeaponImg->Set_AssetName(m_sAssetName);
        pWeaponImg->Set_ViewZ(0.44f);
        pWeaponImg->Set_CustomScaleX(2.f);
        pWeaponImg->Set_CustomScaleY(2.f);

        pWeaponImg->Ready_AfterCreate();

        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneWeaponImg", pWeaponImg);
    }
    return S_OK;
}
_int		CShipPhoneWeaponCreateSuccess::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


   


    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CShipPhoneWeaponCreateSuccess::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);


}

void		CShipPhoneWeaponCreateSuccess::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();



    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_Weapon_UI_NewWeapon"))
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
        float fOffsetY = -0.3f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            if (CShipPhoneWeaponBG::iCurrentIdx == 0)
            {
                pDefFont->Render_Font(L"±¸½Ä¶óÀÌÇÃ", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
            }
            else if (CShipPhoneWeaponBG::iCurrentIdx == 1)
            {
                pDefFont->Render_Font(L"Æ®¸®ÇÃ ¾Ç¼¿", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
            }
            else if (CShipPhoneWeaponBG::iCurrentIdx == 2)
            {
                pDefFont->Render_Font(L"Äõµå·Î ¾Ç¼¿", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
            }
            else if (CShipPhoneWeaponBG::iCurrentIdx == 3)
            {
                pDefFont->Render_Font(L"ÆæÅ¸ ¾Ç¼¿", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
            }
        }
    }

}

HRESULT			CShipPhoneWeaponCreateSuccess::Ready_Component()
{
    // ¹öÆÛ
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipPhoneWeaponCreateSuccess* CShipPhoneWeaponCreateSuccess::Create()
{
    CShipPhoneWeaponCreateSuccess* pIDiverBG = new CShipPhoneWeaponCreateSuccess;

    if (FAILED(pIDiverBG->Ready_GameObject()))
    {
        Safe_Release(pIDiverBG);
        MSG_BOX("pIDiverBG Create Failed");
        return nullptr;
    }

    return pIDiverBG;
}

void CShipPhoneWeaponCreateSuccess::Free()
{
    CGameObject::Free();

}