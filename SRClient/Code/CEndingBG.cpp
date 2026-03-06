#include "pch.h"
#include "CEndingBG.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CDInputMgr.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
CEndingBG::CEndingBG()
    : CGameObject()
{
}

CEndingBG::CEndingBG(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CEndingBG::~CEndingBG()
{
}

HRESULT CEndingBG::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    return S_OK;
}

_int CEndingBG::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

  
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_SUSHIBGALPHA, this);

    return iExit;
}

void CEndingBG::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Compute_ViewZ(&vPos);
}

void CEndingBG::Render_GameObject()
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

HRESULT CEndingBG::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_EndingBG", L"Com_Texture", &m_pTextureCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
    // 21  12 
//pos 10
    m_pTransformCom->m_vScale = { 3.2f,1.5f,1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 0.f,1.7f,0.f };
    return S_OK;
}


CEndingBG* CEndingBG::Create()
{
    CEndingBG* pBackGround = new CEndingBG;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("pBackGround Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CEndingBG::Free()
{
    CGameObject::Free();
}
