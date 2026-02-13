#include "pch.h"
#include "CSushiDave.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
CSushiDave::CSushiDave()
    : CGameObject()
{
    curState = IDLE;
}

CSushiDave::CSushiDave(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CSushiDave::~CSushiDave()
{
}

HRESULT CSushiDave::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_fFrame = 0.f;


    return S_OK;
}

_int CSushiDave::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    switch (curState)
    {
    case IDLE:
        m_fFrame += 2.f * fTimeDelta;

        if (2.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    case SUSHI_IDLE:
        m_fFrame += 2.f * fTimeDelta;
        if (2.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    default:
        m_fFrame += 8.f * fTimeDelta;
        if (8.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    }



    return iExit;
}

void CSushiDave::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Compute_ViewZ(&vPos);
}

void CSushiDave::Render_GameObject()
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

HRESULT CSushiDave::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_BanchoIdleTexture", L"Com_Texture", &m_pIdleTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_CookTexture", L"Com_Texture", &m_pCookTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 1.f, 1.5f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 8.8f, -2.0f, 1.f };
    return S_OK;
}


CSushiDave* CSushiDave::Create()
{
    CSushiDave* pBackGround = new CSushiDave;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("Open Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CSushiDave::Free()
{
    CGameObject::Free();
}
