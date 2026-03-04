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
#include "CSushiDave.h"
#include "CSoundMgr.h"



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

void CWasabi::Update_ImGui()
{
    CGameObject::Update_ImGui();
    ImGui::DragFloat("tempY", &gauge, 0.01f);
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
        CGameObject* wasabi = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"WasabiObject");

        if (gauge >= -0.3f) {
            gauge = -0.3f;
            good += fTimeDelta;
            if (!soundPlayed) {
                CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_WasabiGood", CSoundMgr::SFX, 1.0f);
                soundPlayed = true;
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
        pGraphicDev->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, 0);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_BrownFrame"))
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
        //matTmp.m[3][0] -= 1.f;
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();
        // 와사비 프레임
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_WasabiFrame"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        D3DXMatrixIdentity(&matTmp);
        matTmp.m[0][0] = 5.f;
        matTmp.m[1][1] = 3.9f;
        matTmp.m[3][1] += 0.1f;
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


        ////////////////////////////와사비 게이지 

        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_WasabiGaugeBar"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }
        matTmp = *m_pTransformCom->Get_World();
        matTmp.m[3][1] += 0.3f;
        matTmp.m[0][0] = 0.4f;
        matTmp.m[1][1] = 2.0f;
        matTmp.m[3][0] += 3.7f;


        //matTmp.m[1][1] = 0.1f;
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();


     





        pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
        pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
        pGraphicDev->SetRenderState(D3DRS_STENCILREF, 0x1);
        pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

        pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 1); // 알파가 1 이상인 것만 통과
        pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

        // 2. 색상과 깊이 기록은 끔 (틀만 잡기 위함)
        pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
        pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_WasabiGaugeStancil"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();

        pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);

        // 2. 스텐실 판정: 기록된 '1' 영역에만 그리기
        pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
        pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Orange"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }
      /*  _matrix newMat = *m_pTransformCom->Get_World();
        newMat.m[0][0] += 2.f;*/
        matTmp.m[3][1] = gauge;

        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();


        pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
        pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


      
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_WasabiIcon"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        matTmp = *m_pTransformCom->Get_World();
        matTmp.m[3][1] += 2.3f;
        matTmp.m[0][0] = 0.5f;
        matTmp.m[1][1] = 0.5f;
        matTmp.m[3][0] += 3.7f;


        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();

        if (gauge >= -0.3f) {
            //gauge = -0.3f;
            if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Good"))
            {
                if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
                {
                    pGraphicDev->SetTexture(0, pTexture->Get_Texture());
                }
            }

            matTmp = *m_pTransformCom->Get_World();
            matTmp.m[0][0] = 2.f + good * 0.1f;
            matTmp.m[1][1] = 2.f + good * 0.1f;
            matTmp.m[3][1] += 1.f;
            pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
            m_pBufferCom->Render_Buffer();
            if (good > 3.f) {
                CGameObject* dave = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic_Layer")->Get_GameObjectFirst(L"Dave");
                static_cast<CSushiDave*>(dave)->makingWasabi = false;
                m_bRender = false;
                good = 0.f;
                soundPlayed = false;
            }
        }
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
            CGameObject* wasabi = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"WasabiObject");
            frameMove = true;
            wasabiCreated = true;
            m_fScale += 0.3f;
            gauge += 3.74f * 0.2f;
            if (gauge >= -0.3f) {
                static_cast<CWasabiObject*>(wasabi)->gauge = 0.03f;
                gauge = -0.3f;
            }
            CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_Grind", CSoundMgr::SFX, 1.0f);

        }
    }
}
