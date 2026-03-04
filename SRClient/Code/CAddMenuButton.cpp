#include "pch.h"
#include "CAddMenuButton.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
CAddMenuButton::CAddMenuButton()
    : CGameObject()
{
    m_bSelected = false;

}

CAddMenuButton::CAddMenuButton(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CAddMenuButton::~CAddMenuButton()
{
}

HRESULT CAddMenuButton::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

  

    return S_OK;
}

_int CAddMenuButton::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    }



    return iExit;
}

void CAddMenuButton::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }
 
}

void CAddMenuButton::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pAddTextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();
        if (m_bSelected) {
            m_pSelectTextureCom->Set_Texture(0);
            m_pBufferCom->Render_Buffer();
        }
     
      

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    }
   
}

HRESULT CAddMenuButton::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_AddMenuTex", L"Com_Texture", &m_pAddTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_SelectTex", L"Com_Texture1", &m_pSelectTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 1.5f, 0.3f, 1.f };
    return S_OK;
}


CAddMenuButton* CAddMenuButton::Create()
{
    CAddMenuButton* pBackGround = new CAddMenuButton;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("AddButton Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CAddMenuButton::Free()
{
    CGameObject::Free();

}
