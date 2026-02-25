#include "pch.h"
#include "CSelectedFrame.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CBluejongR.h"
#include "CYellowbackR.h"
#include "CYellowTangR.h"
#include "CDartR.h"
#include "CClownFishR.h"
#include "CSushiFrame.h"
#include "CSushiList.h"
#include "CAssetDefaultFont.h"
#include "CAssetMgr.h"
#include "CMenuFrame.h"
CSelectedFrame::CSelectedFrame()
    : CGameObject()
{
    created = false;

    m_sFishName = L"";
    m_sSushiName = L"";
    m_sQuantity = L"";
    m_sCost = L"";
    m_sQuality = L"";
    m_sPlate = L"";
}

CSelectedFrame::CSelectedFrame(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CSelectedFrame::~CSelectedFrame()
{
}

HRESULT CSelectedFrame::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;
    //숫자 폰트
    CAssetMgr::GetInstance()->LoadAsset(L"Font_SushiNumber");

    CGameObject* pGameObject = nullptr; 
    CGameObject* sushilist = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"List");
    list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
    m_sFishName = static_cast<CSushiList*>(sushilist)->m_sFishName;
    m_sCost = static_cast<CSushiList*>(sushilist)->m_sCost;
    m_sQuality = static_cast<CSushiList*>(sushilist)->m_sQuality;

 



    return S_OK;
}

_int CSelectedFrame::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    if (created) {
        
        m_sSushiName = m_sFishName + L" 초밥";
        m_sPlate = m_sPlate + L" / " + m_sPlate;
        if (m_sFishName == L"블루종") {
            pSushiGameObject = CBluejongR::Create();

            if (nullptr == pSushiGameObject)
                return E_FAIL;

            if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiConfirm", pSushiGameObject)))
                return E_FAIL;
        }
        else if (m_sFishName == L"노랑백") {
            pSushiGameObject = CYellowbackR::Create();

            if (nullptr == pSushiGameObject)
                return E_FAIL;

            if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiConfirm", pSushiGameObject)))
                return E_FAIL;
        }
        else if (m_sFishName == L"코반아지") {
            pSushiGameObject = CDartR::Create();

            if (nullptr == pSushiGameObject)
                return E_FAIL;

            if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiConfirm", pSushiGameObject)))
                return E_FAIL;
        }
        else if (m_sFishName == L"흰동가리") {
            pSushiGameObject = CClownFishR::Create();

            if (nullptr == pSushiGameObject)
                return E_FAIL;

            if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiConfirm", pSushiGameObject)))
                return E_FAIL;
        }
        else if (m_sFishName == L"노랑탕") {
            pSushiGameObject = CYellowTangR::Create();

            if (nullptr == pSushiGameObject)
                return E_FAIL;

            if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiConfirm", pSushiGameObject)))
                return E_FAIL;
        }
        created = false;
    }
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
        _vec3 curPos;
        float xOffset, yOffset;
        m_pTransformCom->Get_Info(INFO_POS, &curPos);
        xOffset = 1.2f;
        yOffset = 0.07f;
        curPos.x += xOffset;
        curPos.y -= yOffset;
        //가격
        CHelper::GetScreenPointFromWorld(&screen1, &curPos);
        //접시
        xOffset = 1.13f;
        curPos.x -= xOffset;
        CHelper::GetScreenPointFromWorld(&screen2, &curPos);
        //만족도
        xOffset = 0.5f;
        curPos.x -= xOffset;
        CHelper::GetScreenPointFromWorld(&screen3, &curPos);
        //초밥 이름
        xOffset = 0.2f;
        yOffset = 0.26f;
        curPos.x -= xOffset;
        curPos.y += yOffset;

        CHelper::GetScreenPointFromWorld(&screen4, &curPos);
        //초밥 사진
        if (pSushiGameObject != nullptr) {
            pSushiGameObject->Set_Render(true);
            CTransform* pTransform = static_cast<CTransform*>(pSushiGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
            pTransform->m_vScale = { 0.25f, 0.15f, 1.f };
            pTransform->m_vInfo[INFO_POS] = m_pTransformCom->m_vInfo[INFO_POS];
            //pTransform->m_vInfo[INFO_POS].z -= 0.1f;
            pTransform->m_vInfo[INFO_POS].y -=  0.05f; 
            CGameObject* menu = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"MenuFrame");
            if (static_cast<CMenuFrame*>(menu)->frameMoved == true) {
                pTransform->m_vInfo[INFO_POS].x -= 1.15f;

            }
            else {
                pTransform->m_vInfo[INFO_POS].x -= 1.12f;

            }

        }
    }
    else {
        if(pSushiGameObject != nullptr)
            pSushiGameObject->Set_Render(false);
    }
  
   


    return iExit;
}

void CSelectedFrame::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }

}

void CSelectedFrame::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pSelectTextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();


        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
        CAssetDefaultFont* pNameFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_SushiName");

        CAssetDefaultFont* pNumberFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_SushiNumber");

        //가격
        _vec2 vPos1 = { screen1.x, screen1.y };
        pNumberFont->Render_Font(m_sCost, &vPos1, D3DXCOLOR(0 / 255.f, 0 / 255.f, 0 / 255.f, 1.f));
        //접시
        _vec2 vPos2 = { screen2.x, screen2.y };
        pNumberFont->Render_Font(m_sPlate, &vPos2, D3DXCOLOR(0 / 255.f, 0 / 255.f, 0 / 255.f, 1.f));
        //만족도
        _vec2 vPos3 = { screen3.x, screen3.y };
        pNumberFont->Render_Font(m_sQuality, &vPos3, D3DXCOLOR(0 / 255.f, 0 / 255.f, 0 / 255.f, 1.f));
        //초밥 이름
        _vec2 vPos4 = { screen4.x, screen4.y };
        pNameFont->Render_Font(m_sSushiName, &vPos4, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    }

}

HRESULT CSelectedFrame::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐

    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_SelectFrameTex", L"Com_Texture", &m_pSelectTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 1.5f, 0.3f, 1.f };
    return S_OK;
}


CSelectedFrame* CSelectedFrame::Create()
{
    CSelectedFrame* selectFrame = new CSelectedFrame;

    if (FAILED(selectFrame->Ready_GameObject()))
    {
        Safe_Release(selectFrame);
        MSG_BOX("selectFrame Create Failed");
        return nullptr;
    }

    return selectFrame;
}

void CSelectedFrame::Free()
{
    CGameObject::Free();
}
