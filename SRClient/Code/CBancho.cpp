#include "pch.h"
#include "CBancho.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
CBancho::CBancho()
    : CGameObject() 
{
    curState = IDLE;
}

CBancho::CBancho(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CBancho::~CBancho()
{
}

HRESULT CBancho::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_fFrame = 0.f;


    return S_OK;
}

_int CBancho::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    switch (curState)
    {
    case IDLE:
        m_fFrame += 5.f * fTimeDelta;

        if (5.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    case COOK:
        m_fFrame += 3.f * fTimeDelta;
        if (3.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    }

   
    return iExit;
}

void CBancho::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Compute_ViewZ(&vPos);
}

void CBancho::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    switch (curState)
    {
    case IDLE:
        m_pIdleTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case COOK:
        m_pCookTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    }

    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBancho::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_BanchoIdleTexture", L"Com_Texture", &m_pIdleTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_CookTexture", L"Com_Texture1", &m_pCookTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 1.f, 1.5f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 8.8f, -2.0f, 1.f };
    return S_OK;
}


CBancho* CBancho::Create()
{
    CBancho* pBackGround = new CBancho;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("Open Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CBancho::Free()
{
    CGameObject::Free();

}
