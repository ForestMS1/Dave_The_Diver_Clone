#include "pch.h"
#include "CShipPhoneIcon.h"
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CColliderMgr.h"
#include "CShipPhone.h"
#include "CShipPhoneApp.h"
#include "CManagement.h"
#include "CHelper.h"
#include "CDInputMgr.h"


CShipPhoneIcon::CShipPhoneIcon()
    : CGameObject()
{
}

CShipPhoneIcon::CShipPhoneIcon(const CShipPhoneIcon& rhs)
    : CGameObject(rhs)
{
}
CShipPhoneIcon::~CShipPhoneIcon()
{
}


HRESULT		CShipPhoneIcon::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vExtents = { 1.0f, 1.0f, 0.1f };

    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_PhoneIcon"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 200.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 200.f;
            vScale = {fWidth, fHeight, 1.f};
        }
    }

    _vec3 vPos = { 0.0f, 0.0f, 0.0f };
    m_pTransformCom->Set_Scale(&vScale);
    m_pTransformCom->Set_Pos(-6.f, -4.f, 0.f);
    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_PhoneIcon", this);

    m_bPhoneOpen = false;
    return S_OK;
}

_int		CShipPhoneIcon::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    // 충돌체 그룹에 넣어줘야한다.
    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_PhoneIcon", m_pAABB);
    m_pAABB->Transform(m_pTransformCom->Get_World());

    if (m_bPhoneOpen)
    {
        if (!CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"ShipPhone"))
        {
            m_bPhoneOpen = false;
        }
    }
    

    return iExit;
}

void		CShipPhoneIcon::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    // 레이와 충돌 체크
    _vec3 vRayPos, vRayDir;
    CHelper::GetMousePointRay(&vRayPos, &vRayDir);
    float fDist{};
    if (m_pAABB->Intersect(&vRayPos, &vRayDir, fDist))
    {
        // Some Logic
        if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
        {
            // 만약 충돌한다면 태그비교후 보이드포인터 들고와서 캐스팅한다음 조작한다.
            if (m_pAABB->Get_Tag() == L"AABB_PhoneIcon")
            {
                if (m_bPhoneOpen)
                {
                    reinterpret_cast<CShipPhoneIcon*>(m_pAABB->Get_VoidPtr())->Close_Phone();
                }
                else
                {
                    reinterpret_cast<CShipPhoneIcon*>(m_pAABB->Get_VoidPtr())->Open_Phone();
                }
            }
        }
    }
}

void		CShipPhoneIcon::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_PhoneIcon"))
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

HRESULT			CShipPhoneIcon::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}
HRESULT CShipPhoneIcon::Open_Phone()
{
    if (!m_bPhoneOpen)
    {
        m_bPhoneOpen = true;
        auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer");

        CShipPhone* pShipPhone = CShipPhone::Create();
        if (nullptr == pShipPhone)
            return E_FAIL;
        if (FAILED(pLayer->Add_GameObject(L"ShipPhone", pShipPhone)))
            return E_FAIL;

        int xCnt = 4;
        for (int i = 0; i < 22; ++i)
        {
            float fPosX = +(i % 4) - 1.5f;
            float fPosY = -(i / 4) + 2.5f;
            CShipPhoneApp* pShipPhoneApp = CShipPhoneApp::Create(i, fPosX, fPosY);
            pShipPhoneApp->Set_Parent(pShipPhone);
            if (nullptr == pShipPhoneApp)
                return E_FAIL;
            if (FAILED(pLayer->Add_GameObject(L"ShipPhoneApp", pShipPhoneApp)))
                return E_FAIL;
        }

    }
   
    return S_OK;
}
HRESULT CShipPhoneIcon::Close_Phone()
{
    if (m_bPhoneOpen)
    {
        auto pPhoneObj = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"ShipPhone");
        if (auto pPhone = dynamic_cast<CShipPhone*>(pPhoneObj))
        {
            pPhone->Set_CloseTween(true);
        }
    }
    
    return S_OK;
}
CShipPhoneIcon* CShipPhoneIcon::Create()
{
    CShipPhoneIcon* pShipDiverBox = new CShipPhoneIcon;

    if (FAILED(pShipDiverBox->Ready_GameObject()))
    {
        Safe_Release(pShipDiverBox);
        MSG_BOX("pShipDiverBox Create Failed");
        return nullptr;
    }

    return pShipDiverBox;
}

void CShipPhoneIcon::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}
