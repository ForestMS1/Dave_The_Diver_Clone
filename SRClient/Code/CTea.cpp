#include "pch.h"
#include "CTea.h"
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
#include "CSushiDave.h"
#include "CCustomer1.h"
#include "CSoundMgr.h"
#include "CTimerMgr.h"



CTea::CTea()
    : CGameObject()
{
    frameMove = false;
    wasabiCreated = false;
    m_fScale = 0.6f;
    customer = nullptr;
}

CTea::CTea(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CTea::~CTea()
{
}

void CTea::Update_ImGui()
{
    CGameObject::Update_ImGui();
    ImGui::DragFloat("teagauge", &gauge, 0.01f);
    ImGui::DragFloat("angle", &angle, 0.01f);
    ImGui::DragFloat("popgaugeX", &potGaugeX, 0.01f);
    ImGui::DragFloat("popgaugeY", &potGaugeY, 0.01f);
    ImGui::DragFloat("potAngle", &potAngle, 0.01f);
}
HRESULT CTea::Ready_GameObject()
{

    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_fFrame = 0.f;

    return S_OK;
}

_int CTea::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
        Key_Input();

        if (gauge >= -0.3f) {
            gauge = -0.3f;
        }
    }
    if (finished) {
        resetTime += fTimeDelta;
        if (resetTime > 2.f) {
            Reset_Value();
            finished = false;
            m_bRender = false;
           
        }
        static_cast<CCustomer1*>(customer)->gotTea = true;
        if (!soundPlayed) {
            CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_Perfect", CSoundMgr::SFX, 1.0f);
            soundPlayed = true;
        }
    }




    return iExit;
}

void CTea::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);


}

void CTea::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        // 차 프레임
        pGraphicDev->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, 0);
     /*   pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
            D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);*/
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_CupFrame"))
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
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();

        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_CupBack"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        D3DXMatrixIdentity(&matTmp);
        matTmp.m[0][0] = 2.f;
        matTmp.m[1][1] = 2.f;
        matTmp.m[3][1] -= 1.f;

        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();


        // 차 따르는 거

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


        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_TeaLiquidStancil"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        D3DXMatrixIdentity(&matTmp);
        matTmp.m[0][0] = 5.f;
        matTmp.m[1][1] = 4.f;
        matTmp.m[3][0] = 0.f;

        //matTmp.m[3][1] = -0.28f;
        matTmp.m[3][1] = potGaugeY;
        matTmp.m[3][0] = potGaugeX;

        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();

        pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);

        // 2. 스텐실 판정: 기록된 '1' 영역에만 그리기
        pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
        pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);



        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_TeaLiquid"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

      
        D3DXMATRIX matRot, matScale,matTrans;
        D3DXMatrixIdentity(&matTmp);
        D3DXMatrixIdentity(&matScale);
        D3DXMatrixIdentity(&matTrans);
        D3DXMatrixIdentity(&matRot);
        D3DXMatrixScaling(&matScale, 4.f, 4.f, 1.f);
        D3DXMatrixRotationZ(&matRot, angle);
        D3DXMatrixTranslation(&matTrans, 2.2f, -0.28f, 0.f);
        matTmp = matScale * matRot * matTrans;
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();
        pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
        pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);





        pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
        pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
        pGraphicDev->SetRenderState(D3DRS_STENCILREF, 0x2);
        pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

        pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 1); // 알파가 1 이상인 것만 통과
        pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

        // 2. 색상과 깊이 기록은 끔 (틀만 잡기 위함)
        pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
        pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
       
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_CupBack"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        D3DXMatrixIdentity(&matTmp);
        matTmp.m[0][0] = 2.f;
        matTmp.m[1][1] = 2.f;
        matTmp.m[3][1] -= 0.78f;

        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();

        pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);

        // 2. 스텐실 판정: 기록된 '1' 영역에만 그리기
        pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
        pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Green"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }
 
        matTmp.m[3][1] = gauge;
    

        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();


        pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
        pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);



        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_CupFront"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        D3DXMatrixIdentity(&matTmp);
        matTmp.m[0][0] = 2.f;
        matTmp.m[1][1] = 2.f;
        matTmp.m[3][1] -= 1.f;
    
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();




        if (finished) {
            if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Perfect"))
            {
                if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
                {
                    pGraphicDev->SetTexture(0, pTexture->Get_Texture());
                }
            }

            D3DXMatrixIdentity(&matTmp);
            matTmp.m[0][0] = 3.f;
            matTmp.m[1][1] = 3.f;
            matTmp.m[3][1]  = 0.f;

            pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
            m_pBufferCom->Render_Buffer();
        }
        ////////////////////////////////////////////////////
        //////////////////////주전자///////////////////////


        pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
        pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
        pGraphicDev->SetRenderState(D3DRS_STENCILREF, 0x3);
        pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

        pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 1); // 알파가 1 이상인 것만 통과
        pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

        // 2. 색상과 깊이 기록은 끔 (틀만 잡기 위함)
        pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
        pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_CupFrame"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        D3DXMatrixIdentity(&matTmp);
        matTmp.m[0][0] = 5.f;
        matTmp.m[1][1] = 4.f;
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();


        pGraphicDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);

        // 2. 스텐실 판정: 기록된 '1' 영역에만 그리기
        pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
        pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Pot"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        D3DXMatrixIdentity(&matTmp);
        D3DXMatrixIdentity(&matRot);
        D3DXMatrixIdentity(&matScale);
        D3DXMatrixIdentity(&matTrans);
        D3DXMatrixScaling(&matScale, 2.f, 2.f, 1.f);
        D3DXMatrixRotationZ(&matRot, potAngle);
        D3DXMatrixTranslation(&matTrans, 2.75f, 2.93f, 0.f);
        matTmp = matScale * matRot * matTrans;
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();


        pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
        pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);



        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_FKey"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        D3DXMatrixIdentity(&matTmp);
        matTmp.m[0][0] = 0.5f;
        matTmp.m[1][1] = 0.5f;
        matTmp.m[3][1] = 2.93f;

        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        m_pBufferCom->Render_Buffer();


        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    }
}



HRESULT CTea::Ready_Component()
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

CTea* CTea::Create()
{
    CTea* pBackGround = new CTea;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("Wasabi Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CTea::Free()
{
    //Safe_Release(customer);
    CGameObject::Free();

}

void CTea::Key_Input()
{
    if (!frameMove) {
        if (CDInputMgr::GetInstance()->Key_Pressing(DIKEYBOARD_F)) {
            soundPlayedTime += CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_FPS60");
            if (soundPlayedTime > 2.3f) {
                CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_TeaPouring", CSoundMgr::SFX, 1.0f);
                soundPlayedTime = 0.f;
            }
            angle += 0.05f;
            if (potAngle < 0.52f) {
                potAngle += 0.3f;
            }
            else {
                potAngle = 0.52f;
            }
            if (potGaugeY < 1.62f) {
                gauge += 0.01f;
            }
            else {
                potGaugeY -= 0.05f;

            }

            if (gauge >= -1.82) {
                gauge = -1.82;
                potGaugeY -= 0.2f;
            }
            if (potGaugeY < -6.62f) {
                finished = true;
                //주문한 손님의 gotTea를 true로 바꿈
                // 2초뒤 RENDER를 FALSE로 바꿈
            }
        }
    }
}

void CTea::Reset_Value()
{
    gauge = -4.43f;
    potGaugeX = 0.81f;
    potGaugeY = 6.33f;
    angle = 0.f;
    potAngle = 0.f;
    resetTime = 0.f;
    soundPlayed = false;
}
