#include "pch.h"
#include "CClownFishR.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
CClownFishR::CClownFishR()
    : CGameObject()
{
}

CClownFishR::CClownFishR(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CClownFishR::~CClownFishR()
{
}

HRESULT CClownFishR::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;



    return S_OK;
}

_int CClownFishR::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    return iExit;
}

void CClownFishR::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }

}

void CClownFishR::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

        //pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pSushiTextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

        //pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    }

}

HRESULT CClownFishR::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_ClownFishTex", L"Com_Texture", &m_pSushiTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    //m_pTransformCom->m_vScale = { 0.6f, 0.2f, 1.f };
    return S_OK;
}


CClownFishR* CClownFishR::Create()
{
    CClownFishR* clownFish = new CClownFishR;

    if (FAILED(clownFish->Ready_GameObject()))
    {
        Safe_Release(clownFish);
        MSG_BOX("clownFish Create Failed");
        return nullptr;
    }

    return clownFish;
}

void CClownFishR::Free()
{
    CGameObject::Free();
}
