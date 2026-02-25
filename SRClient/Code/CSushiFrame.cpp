#include "pch.h"
#include "CSushiFrame.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CHelper.h"
#include "CDInputMgr.h"
#include "CBluejongR.h"
#include "CYellowbackR.h"
#include "CYellowTangR.h"
#include "CDartR.h"
#include "CClownFishR.h"
#include "CAssetDefaultFont.h"
#include "CAssetMgr.h"
#include "CGameMemMgr.h"
CSushiFrame::CSushiFrame()
    : CGameObject()
{
    m_bSelected = false;
    m_bImageCreated = false;
    ConfirmOpened = false;
    fishName = L"";
    m_sQuanity = L"1";
}

CSushiFrame::CSushiFrame(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CSushiFrame::~CSushiFrame()
{
}

void CSushiFrame::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_fOffsetX", &m_fOffsetX, 0.1);
    ImGui::DragFloat("m_fOffsetY", &m_fOffsetY ,0.1);
}

HRESULT CSushiFrame::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };
    _vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];

    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Frame", this);
   
  
    CAssetMgr::GetInstance()->LoadAsset(L"Font_DefaultXX");
    CAssetMgr::GetInstance()->LoadAsset(L"Font_Level");
 
    //CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_DefaultXX");

    return S_OK;
}

_int CSushiFrame::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender && !ConfirmOpened && stoi(m_sQuanity) > 0) {
        CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Frame", m_pAABB);
    }
    else if (stoi(m_sQuanity) <= 0) {
        m_bSelected = false;
    }
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    }
    m_pAABB->Transform(m_pTransformCom->Get_World());

    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    if (!m_bImageCreated) {
        _vec3 curPos;
        m_pTransformCom->Get_Info(INFO_POS, &curPos);
        m_fOffsetX = 0.26;
        m_fOffsetY = 0.26;
        curPos.x -= m_fOffsetX;
        curPos.y += m_fOffsetY;
        CHelper::GetScreenPointFromWorld(&screen, &curPos);
        m_pTransformCom->Get_Info(INFO_POS, &curPos);
        m_fOffsetX = 0.2;
        m_fOffsetY = 0.15;
        curPos.x += m_fOffsetX;
        curPos.y -= m_fOffsetY;
        CHelper::GetScreenPointFromWorld(&screen2, &curPos);
        CGameObject* pGameObject = nullptr;
        if (fishName == L"블루종") {
            pGameObject = CBluejongR::Create();

            if (nullptr == pGameObject)
                return E_FAIL;

            if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushi", pGameObject)))
                return E_FAIL;
        }
        else if (fishName == L"노랑백") {
            pGameObject = CYellowbackR::Create();

            if (nullptr == pGameObject)
                return E_FAIL;

            if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushi", pGameObject)))
                return E_FAIL;
        }
        else if (fishName == L"코반아지") {
            pGameObject = CDartR::Create();

            if (nullptr == pGameObject)
                return E_FAIL;

            if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushi", pGameObject)))
                return E_FAIL;
        }
        else if (fishName == L"흰동가리") {
            pGameObject = CClownFishR::Create();

            if (nullptr == pGameObject)
                return E_FAIL;

            if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushi", pGameObject)))
                return E_FAIL;
        }
        else if (fishName == L"노랑탕") {
            pGameObject = CYellowTangR::Create();

            if (nullptr == pGameObject)
                return E_FAIL;

            if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushi", pGameObject)))
                return E_FAIL;
        }
        if (pGameObject != nullptr) {
            CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
            pTransform->m_vScale = { 0.2f, 0.12f, 1.f };
            pTransform->m_vInfo[INFO_POS] = m_pTransformCom->m_vInfo[INFO_POS];
            pTransform->m_vInfo[INFO_POS].z -= 0.001f;
            pTransform->m_vInfo[INFO_POS].x -= 0.01f;
            pTransform->m_vInfo[INFO_POS].y -= 0.05f;

            m_bImageCreated = true;
        }
          
    }





    return iExit;
}

void CSushiFrame::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
        // 레이와 충돌 체크
        _vec3 vRayPos, vRayDir;
        CHelper::GetMousePointRay(&vRayPos, &vRayDir);
        float fDist{};
        if (m_pAABB->Intersect(&vRayPos, &vRayDir, fDist))
        {
            // Some Logic
            if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
            {
                // 만약 충돌한다면 태그비교후 보이드포인터 들고와서 캐스팅한다음 조작한다.
                if (m_pAABB->Get_Tag() == L"AABB_Frame")
                {
                    list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
                    list<CGameObject*>::iterator iter = frame->begin();
                    for (iter; iter != frame->end(); iter++) {
                        static_cast<CSushiFrame*>((*iter))->m_bSelected = false;
                    }
                    m_bSelected = true;


                    // 스시 info 렌더
                }
            }
        }
    }

}

void CSushiFrame::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pFrameTextureCom->Set_Texture(0);
        //  m_pFishTextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();
        if (m_bSelected)
        {
            pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

            m_pSelectedTextureCom->Set_Texture(0);
            m_pBufferCom->Render_Buffer();
            pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

        }


        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


        CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_DefaultXX");
        CAssetDefaultFont* plevelFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Level");
        vector<CGameMemMgr::FISH*> fishes = CGameMemMgr::GetInstance()->getFishes();
        vector<CGameMemMgr::FISH*>::iterator iter = fishes.begin();
        for (iter; iter != fishes.end(); iter++) {
            if ((*iter)->name == fishName) {
                m_sQuanity = std::to_wstring((*iter)->quantity);
                m_sLevel = L"Lv." + std::to_wstring((*iter)->level);
           
            }
        }
        _vec2 vPos = { screen2.x , screen2.y  };
        if (stoi(m_sQuanity) <= 0) {
            pDefFont->Render_Font(m_sQuanity, &vPos, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
        }
        else {
            pDefFont->Render_Font(m_sQuanity, &vPos, D3DXCOLOR(1.f, 1.f, 0.87f, 1.f));
        }
        _vec2 vPos1 = { screen.x , screen.y };
       
        
        plevelFont->Render_Font(m_sLevel, &vPos1, D3DXCOLOR(1.f, 0.9f, 0.8f, 1.f));
    }
  
    
   
    //float scalex = CInfoMgr::GetInstance()->Get_ScaleFactor();
}

HRESULT CSushiFrame::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_SushiFrameTex", L"Com_Texture", &m_pFrameTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_SushiSelectedTex", L"Com_Texture1", &m_pSelectedTextureCom))))
        return E_FAIL;

      /*  if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_BluejongTex", L"Com_Texture", &m_pFishTextureCom))))
            return E_FAIL;*/
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
    

    return S_OK;
}


CSushiFrame* CSushiFrame::Create(wstring fishName)
{
    CSushiFrame* frame = new CSushiFrame;
    frame->fishName = fishName;
    if (FAILED(frame->Ready_GameObject()))
    {
        Safe_Release(frame);
        MSG_BOX("sushiFrame Create Failed");
        return nullptr;
    }

    return frame;
}

void CSushiFrame::Free()
{
    Safe_Release(m_pAABB);
    CGameObject::Free();
}
