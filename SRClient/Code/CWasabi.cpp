#include "pch.h"
#include "CWasabi.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CDInputMgr.h"
#include "CMenuBubble.h"
#include "CChair.h"
#include "CAABB.h"
#include "CColliderMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CWasabiObject.h"



CWasabi::CWasabi()
    : CGameObject()
{
    frameMove = false;
    wasabiCreated = false;
    m_fScale = 0.6f;
}

CWasabi::CWasabi(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CWasabi::~CWasabi()
{
}

HRESULT CWasabi::Ready_GameObject()
{
  
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_fFrame = 0.f;

    return S_OK;
}

_int CWasabi::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
        Key_Input();
        if (frameMove) {
            m_fFrame += 8.f * fTimeDelta;
            if (8.f < m_fFrame) {
                m_fFrame = 0.f;
                frameMove = false;
            }
        }

    }


    return iExit;
}

void CWasabi::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

  
}

void CWasabi::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

        // 와사비 프레임
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_WasabiFrame"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        matTmp.m[0][0] = 5.f;
        matTmp.m[1][1] = 4.f;
        matTmp.m[2][2] = 1.f;
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();

        if (wasabiCreated) {
            if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Wasabi"))
            {
                if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
                {
                    pGraphicDev->SetTexture(0, pTexture->Get_Texture());
                }
            }
            D3DXMatrixIdentity(&matTmp);

            matTmp.m[0][0] = m_fScale;
            matTmp.m[1][1] = m_fScale;
            matTmp.m[2][2] = 1.f;
            matTmp.m[3][0] += 0.3f;
            matTmp.m[3][1] += 1.f;

            pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
            m_pBufferCom->Render_Buffer();
        }
      


        m_pWasabiTextureCom->Set_Texture((_uint)m_fFrame);
        _matrix world = *m_pTransformCom->Get_World();
        world.m[3][1] -= 0.2f;
        pGraphicDev->SetTransform(D3DTS_WORLD, &world);

        m_pBufferCom->Render_Buffer();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    }
}



HRESULT CWasabi::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_WasabiAnim", L"Com_Texture", &m_pWasabiTextureCom))))
        return E_FAIL;



    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 3.f, 3.f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 0.3f, -0.5f, 0.f };
    m_pTransformCom->Rotation(ROT_Z, 180.f);
    return S_OK;
}

CWasabi* CWasabi::Create()
{
    CWasabi* pBackGround = new CWasabi;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("Wasabi Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CWasabi::Free()
{
    CGameObject::Free();

}

void CWasabi::Key_Input()
{
    if (!frameMove) {
        if (CDInputMgr::GetInstance()->Key_Down(DIKEYBOARD_K)) {
            frameMove = true;
            wasabiCreated = true;
            m_fScale += 0.2f;
            CGameObject* wasabi = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"WasabiObject");
            static_cast<CWasabiObject*>(wasabi)->gauge += 0.93 * 0.20f;
            if (static_cast<CWasabiObject*>(wasabi)->gauge > 0.93f) {
                static_cast<CWasabiObject*>(wasabi)->gauge = 0.93f;
            }
        }
    }
}
