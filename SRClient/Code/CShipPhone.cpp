#include "pch.h"
#include "CShipPhone.h"
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CColliderMgr.h"
#include "CManagement.h"
#include "CDInputMgr.h"
#include "CHelper.h"
#include "CLog.h"
#include "CShipPhoneIDiverBG.h"
#include "CShipPhoneWeaponBG.h"
#include "CStockMarket.h"

CShipPhone::CShipPhone()
    : CGameObject()
{
}

CShipPhone::CShipPhone(const CShipPhone& rhs)
    : CGameObject(rhs)
{
}
CShipPhone::~CShipPhone()
{
}


HRESULT		CShipPhone::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };

    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_PhoneBG"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 130.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 130.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Scale(&vScale);
    m_pTransformCom->Set_Pos(-5.f, -10.f, 0.f);


    m_tweenOpen = m_tweenOpen.from(-10.f, 0.f, 0.f).to(0.f, 10, -15.f).during(500);
    m_bOpenTween = true;

    m_tweenClose = m_tweenClose.from(0.f).to(-10.f).during(100);
    m_bCloseTween = false;

    m_bOnFocus = false;
    m_bOnUnFocus = true;

    //m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Dave", this);

    // PosX, PosY, RotX, RotY
    m_tweenFocus = m_tweenFocus.from(-5.f, 0.f, 10, -15.f).to(0.f, 0.f, 0.f, 0.f).during(300);
    m_bFocus = false;
    m_bFocusing = false;

    // PosX, PosY, RotX, RotY
    m_tweenUnFocus = m_tweenUnFocus.from(0.f, 0.f, 0.f, 0.f).to(-5.f, 0.f, 10, -15.f).during(300);
    //m_bUnFocus = true;
    m_bUnFocusing = false;

    return S_OK;
}

_int		CShipPhone::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    // 충돌체 그룹에 넣어줘야한다.
    //CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Ship", m_pAABB);
    //m_pAABB->Transform(m_pTransformCom->Get_World());


    {
        if (m_bOpenTween)
        {
            // PosY, RotX, RotY
            auto val = m_tweenOpen.step(int(fTimeDelta * 1000.f));
            _vec3 dir = { 0.f, 1.f, 0.f };
            _vec3 vPos;
            

            m_pTransformCom->Get_Info(INFO_POS, &vPos);
            m_pTransformCom->Set_Pos(vPos.x, val[0], vPos.z);

            _vec3 vRot = { val[1], val[2], 0.f};
            m_pTransformCom->Set_Rotation(&vRot);
            if (m_tweenOpen.progress() >= 1.0f) {
                m_bOpenTween = false;
            }
        }

        if (m_bCloseTween)
        {
            auto val = m_tweenClose.step(int(fTimeDelta * 1000.f));
            _vec3 dir = { 0.f, 1.f, 0.f };
            _vec3 vPos;
            m_pTransformCom->Get_Info(INFO_POS, &vPos);
            m_pTransformCom->Set_Pos(vPos.x, val, vPos.z);
            if (m_tweenClose.progress() >= 1.0f) {
                m_bOpenTween = false;

                Set_DeadCascade();
                //CGameObject* pGameObj = this;

                //CHelper::TreeLevelTraversal(pGameObj, [](CGameObject* pObj) { pObj->Set_Dead(); });
            }
        }

        if (m_bFocusing)
        {
            // PosX, PosY, RotX, RotY
            auto val = m_tweenFocus.step(int(fTimeDelta * 1000.f));
            _vec3 vPos;
            m_pTransformCom->Get_Info(INFO_POS, &vPos);
            m_pTransformCom->Set_Pos(val[0], val[1], vPos.z);
            _vec3 vRot = { val[2], val[3], 0.f };
            m_pTransformCom->Set_Rotation(&vRot);
            //CLog::Debug(L"val: 0: %f, 1: %f, 2: %f, 3: %f \n", val[0], val[1], val[2], val[3]);
            if (m_tweenFocus.progress() >= 1.0f) {
                m_bFocus = true;
                //m_bUnFocus = false;
                m_bFocusing = false;
                m_tweenFocus.seek(0.f);
            }

        }

        if (m_bUnFocusing)
        {
            auto val = m_tweenUnFocus.step(int(fTimeDelta * 1000.f));
            _vec3 vPos;
            m_pTransformCom->Get_Info(INFO_POS, &vPos);
            m_pTransformCom->Set_Pos(val[0], val[1], vPos.z);
            _vec3 vRot = { val[2], val[3], 0.f };
            m_pTransformCom->Set_Rotation(&vRot);
            if (m_tweenUnFocus.progress() >= 1.0f) {
                m_bFocus = false;
                //m_bUnFocus = true;
                m_bUnFocusing = false;
                m_tweenUnFocus.seek(0.f);
            }
        }
    }

    if (m_bFocus)
    {
        if (ImGui::Button("UnFocus"))
        {
            UnFocus_App();
        }

        if (!m_bOnFocus)
        {
            m_bOnFocus = true;
            m_bOnUnFocus = false;
            OnFocus_App();
        }

        OnFocusing_App();
    }
    else 
    {
        if (!m_bOnUnFocus)
        {
            m_bOnFocus = false;
            m_bOnUnFocus = true;
            OnUnFocus_App();
        }

        OnUnFocusing_App();
    }
    

   

    //if (CDInputMgr::GetInstance()->Key_Pressing(DIK_RIGHT))
    //{
    //    _vec3 dir = { 1.f, 0.f, 0.f };
    //    m_pTransformCom->Move_Pos(&dir, 1.f, fTimeDelta);
    //}
    //if (CDInputMgr::GetInstance()->Key_Pressing(DIK_LEFT))
    //{
    //    _vec3 dir = { -1.f, 0.f, 0.f };
    //    m_pTransformCom->Move_Pos(&dir, 1.f, fTimeDelta);
    //}
    //if (CDInputMgr::GetInstance()->Key_Pressing(DIK_UP))
    //{
    //    m_pTransformCom->Rotation(ROT_Y, 1.f);
    //}
    //if (CDInputMgr::GetInstance()->Key_Pressing(DIK_DOWN))
    //{
    //    m_pTransformCom->Rotation(ROT_Y, -1.f);
    //}

    return iExit;
}

void		CShipPhone::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipPhone::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_PhoneBG"))
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

void CShipPhone::Focus_App(wstring_view svFocusAppName)
{
    if (!m_bFocus)
    {
        m_bFocusing = true;
        m_sFocusedAppName = svFocusAppName;
    }
}

void CShipPhone::UnFocus_App()
{
    m_bUnFocusing = true;

    m_bFocus = false;
    m_bOnFocus = false;

}

HRESULT			CShipPhone::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}
void CShipPhone::OnFocus_App()
{
    if (m_sFocusedAppName == L"IDiver")
    {
        auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer");
        CShipPhoneIDiverBG* pShipPhoneIDiverBG = CShipPhoneIDiverBG::Create();
        pLayer->Add_GameObject(L"ShipPhoneIDiverBG", pShipPhoneIDiverBG);
    }
    else if (m_sFocusedAppName == L"Weapon")
    {
        auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer");
        CShipPhoneWeaponBG* pWeapon = CShipPhoneWeaponBG::Create();
        pLayer->Add_GameObject(L"ShipPhoneWeaponBG", pWeapon);
    }
    else if (m_sFocusedAppName == L"Stock")
    {
        auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer");
        CStockMarket* pStock = CStockMarket::Create();
        pLayer->Add_GameObject(L"StockMarket", pStock);
    }

}
void CShipPhone::OnFocusing_App()
{
    //if (CDInputMgr::GetInstance()->Key_Down(DIK_C))
    //{
    //    UnFocus_App();
    //}
}
void CShipPhone::OnUnFocus_App()
{
    if (auto pGameObj = CManagement::GetInstance()
        ->Get_Scene()
        ->Get_Layer(L"0_GameLogic_Layer")
        ->Get_GameObjectFirst(L"ShipPhoneIDiverBG"))
    {
        pGameObj->Set_DeadCascade();
    }
    else if (auto pGameObj = CManagement::GetInstance()
        ->Get_Scene()
        ->Get_Layer(L"0_GameLogic_Layer")
        ->Get_GameObjectFirst(L"ShipPhoneWeaponBG"))
    {
        pGameObj->Set_DeadCascade();
    }
    else if (auto pGameObj = CManagement::GetInstance()
        ->Get_Scene()
        ->Get_Layer(L"0_GameLogic_Layer")
        ->Get_GameObjectFirst(L"StockMarket"))
    {
        pGameObj->Set_DeadCascade();
    }
}
void CShipPhone::OnUnFocusing_App()
{

}
CShipPhone* CShipPhone::Create()
{
    CShipPhone* pShipDiverBox = new CShipPhone;

    if (FAILED(pShipDiverBox->Ready_GameObject()))
    {
        Safe_Release(pShipDiverBox);
        MSG_BOX("pShipDiverBox Create Failed");
        return nullptr;
    }

    return pShipDiverBox;
}

void CShipPhone::Free()
{
    CGameObject::Free();
    //Safe_Release(m_pAABB);
}
