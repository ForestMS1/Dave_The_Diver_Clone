#include "pch.h"
#include "CShipDiverBoxInventory.h"
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CShipDiverBoxDave.h"
#include "CManagement.h"

CShipDiverBoxInventory::CShipDiverBoxInventory()
    : CGameObject()
{
}

CShipDiverBoxInventory::CShipDiverBoxInventory(const CShipDiverBoxInventory& rhs)
    : CGameObject(rhs)
{
}
CShipDiverBoxInventory::~CShipDiverBoxInventory()
{
}


HRESULT		CShipDiverBoxInventory::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };

    _vec3 vPos = { 00.0f, 0.0f, 0.0f };
    _vec3 v = { 5.f , 5.f, 5.f };
    m_pTransformCom->Set_Scale(&v);
    m_pTransformCom->Set_Pos(0.f, -10.f, 0.f);
    //m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Dave", this);

    m_fViewZ = 0.5f;


    m_bOpen = false;

    m_bOpenTween = true;
    m_tweenOpen = m_tweenOpen.from(-10.f).to(0.f).during(200);

    m_bCloseTween = false;
    m_tweenClose = m_tweenClose.from(0.f).to(-10.f).during(200);


    //CShipDiverBoxDave
    CShipDiverBoxDave* pDave = CShipDiverBoxDave::Create();
    pDave->Set_Parent(this);
    CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxDave", pDave);

    return S_OK;
}

_int		CShipDiverBoxInventory::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    if (CDInputMgr::GetInstance()->Key_Down(DIK_C))
    {
        m_bCloseTween = true;
    }

    if (m_bOpenTween)
    {
        // PosY, RotX, RotY
        auto val = m_tweenOpen.step(int(fTimeDelta * 1000.f));
        _vec3 dir = { 0.f, 1.f, 0.f };
        _vec3 vPos;

        m_pTransformCom->Get_Info(INFO_POS, &vPos);
        m_pTransformCom->Set_Pos(vPos.x, val, vPos.z);

        if (m_tweenOpen.progress() >= 1.0f) {
            m_bOpenTween = false;
            m_bOpen = true;
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

            Set_DeadCascade();
        }
    }

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CShipDiverBoxInventory::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipDiverBoxInventory::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_DiverBoxInvenTmp"))
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

HRESULT			CShipDiverBoxInventory::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}
CShipDiverBoxInventory* CShipDiverBoxInventory::Create()
{
    CShipDiverBoxInventory* pShipDiverBox = new CShipDiverBoxInventory;

    if (FAILED(pShipDiverBox->Ready_GameObject()))
    {
        Safe_Release(pShipDiverBox);
        MSG_BOX("pShipDiverBox Create Failed");
        return nullptr;
    }

    return pShipDiverBox;
}

void CShipDiverBoxInventory::Free()
{
    CGameObject::Free();
    //Safe_Release(m_pAABB);
}
