#include "pch.h"
#include "CHood.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
CHood::CHood()
    : CGameObject()
{
}

CHood::CHood(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CHood::~CHood()
{
}

HRESULT CHood::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;



    return S_OK;
}

_int CHood::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CHood::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Compute_ViewZ(&vPos);
}

void CHood::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CHood::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_HoodTex", L"Com_Texture", &m_pTextureCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 1.4f, 2.f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 6.1f, 2.5f, -2.7f };
    return S_OK;
}


CHood* CHood::Create()
{
    CHood* pBackGround = new CHood;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("Hood Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CHood::Free()
{
    CGameObject::Free();
}
