#include "pch.h"
#include "CSushiList.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
CSushiList::CSushiList()
    : CGameObject()
{
    render = false;
}

CSushiList::CSushiList(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CSushiList::~CSushiList()
{
}

HRESULT CSushiList::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;



    return S_OK;
}

_int CSushiList::Update_GameObject(const _float& fTimeDelta)
{
    if (render) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    return iExit;
}

void CSushiList::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (render) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }

}

void CSushiList::Render_GameObject()
{
    if (render) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pListextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    }

}

HRESULT CSushiList::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_SushiListTex", L"Com_Texture", &m_pListextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 2.0f, 3.0f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 3.5f,0.f,-4.5f };
    return S_OK;
}


CSushiList* CSushiList::Create()
{
    CSushiList* pBackGround = new CSushiList;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("List Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CSushiList::Free()
{
    CGameObject::Free();
}
