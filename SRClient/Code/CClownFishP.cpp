#include "pch.h"
#include "CClownFishP.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
CClownFishP::CClownFishP()
    : CGameObject()
{
}

CClownFishP::CClownFishP(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CClownFishP::~CClownFishP()
{
}

HRESULT CClownFishP::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;
    m_pTransformCom->m_vScale = { 0.25f, 0.15f, 1.f };



    return S_OK;
}

_int CClownFishP::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    return iExit;
}

void CClownFishP::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }

}

void CClownFishP::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

        pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pFishTextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

        pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    }

}

HRESULT CClownFishP::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_ClownFishP", L"Com_Texture", &m_pFishTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    //m_pTransformCom->m_vScale = { 0.6f, 0.2f, 1.f };
    return S_OK;
}


CClownFishP* CClownFishP::Create()
{
    CClownFishP* bluejong = new CClownFishP;

    if (FAILED(bluejong->Ready_GameObject()))
    {
        Safe_Release(bluejong);
        MSG_BOX("Bluejong Create Failed");
        return nullptr;
    }

    return bluejong;
}

void CClownFishP::Free()
{
    CGameObject::Free();
}
