#include "pch.h"
#include "CFishFrame.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CBluejongP.h"
#include "CClownFishP.h"
#include "CDartP.h"
#include "CYellowbackP.h"
#include "CYellowTangP.h"
#include "CSushiFrame.h"
#include "CAssetDefaultFont.h"
#include "CAssetMgr.h"
#include "CDepthP.h"
#include "CHeadP.h"
CFishFrame::CFishFrame()
    : CGameObject()
{
}

CFishFrame::CFishFrame(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CFishFrame::~CFishFrame()
{
}

HRESULT CFishFrame::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;
    CAssetMgr::GetInstance()->AddAsset(L"Font_FishName", CAssetDefaultFont::Create(L"¸»±º °íµñ", 0, 25, FW_BOLD));
    CAssetMgr::GetInstance()->AddAsset(L"Font_FishQuantity", CAssetDefaultFont::Create(L"¿µÇâ", 0, 26, FW_BOLD));
    CAssetMgr::GetInstance()->LoadAsset(L"Font_FishName");
    CAssetMgr::GetInstance()->LoadAsset(L"Font_FishQuantity");
    _vec3 curPos = { 2.35f,-0.08f,-4.6f };
    CHelper::GetScreenPointFromWorld(&screen, &curPos);

    _vec3 quantity = { 4.8f,-0.18f,-4.6f };
    CHelper::GetScreenPointFromWorld(&screen1, &quantity);

    CGameObject* pGameObject = nullptr;

    pGameObject = CBluejongP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishPic", pGameObject)))
        return E_FAIL;
    pictures.push_back(pGameObject);

    pGameObject = CYellowbackP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishPic", pGameObject)))
        return E_FAIL;

    pictures.push_back(pGameObject);

    pGameObject = CDartP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishPic", pGameObject)))
        return E_FAIL;

    pictures.push_back(pGameObject);

    pGameObject = CClownFishP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishPic", pGameObject)))
        return E_FAIL;
    pictures.push_back(pGameObject);

    pGameObject = CYellowTangP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishPic", pGameObject)))
        return E_FAIL;
    pictures.push_back(pGameObject);
    pGameObject = CHeadP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishPic", pGameObject)))
        return E_FAIL;
    CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
    pTransform->m_vScale.y = 0.25f;
    pictures.push_back(pGameObject);

    pGameObject = CDepthP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Depth", pGameObject)))
        return E_FAIL;

    list<CGameObject*>* picture = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"fishPic");
    list<CGameObject*>::iterator iter = picture->begin();
    for (iter; iter != picture->end(); iter++) {
        CTransform* pTransform = static_cast<CTransform*>((*iter)->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = m_pTransformCom->m_vInfo[INFO_POS];
        pTransform->m_vInfo[INFO_POS] = { 2.f,-0.3f,-4.6f };
    }
    return S_OK;
}

_int CFishFrame::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    }
    else {
        for (auto picture : pictures) {
            picture->Set_Render(false);
            CGameObject* depth = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Depth");
            depth->Set_Render(false);
        }
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    return iExit;
}

void CFishFrame::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }

}

void CFishFrame::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

        pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pFishframeTextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

        pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


        list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
        list<CGameObject*>::iterator iter = frame->begin();
        for (iter; iter != frame->end(); iter++) {
            if (static_cast<CSushiFrame*>(*iter)->m_bSelected == true) {
                CGameObject* depth = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Depth");
                depth->Set_Render(true);
                m_sFishName = static_cast<CSushiFrame*>(*iter)->fishName;
                if (m_sFishName == L"¹ÝÃÝ") {
                    m_sFishName = L"???";
                }
                m_sFishQuantity = static_cast<CSushiFrame*>(*iter)->m_sQuanity + L"/1";
                vector<CGameObject*>::iterator iter3 = pictures.begin();
                for (iter3; iter3 != pictures.end(); iter3++) {
                    (*iter3)->Set_Render(false);
                }
                if (m_sFishName == L"ºí·çÁ¾") {
                    pictures[0]->Set_Render(true);
                }
                else if (m_sFishName == L"³ë¶û¹é") {
                    pictures[1]->Set_Render(true);
                }
                else if (m_sFishName == L"ÄÚ¹Ý¾ÆÁö") {
                    pictures[2]->Set_Render(true);
                }
                else if (m_sFishName == L"Èòµ¿°¡¸®") {
                    pictures[3]->Set_Render(true);
                }
                else if (m_sFishName == L"³ë¶ûÅÁ") {
                    pictures[4]->Set_Render(true);
                }
                else if (m_sFishName == L"???") {
                    pictures[5]->Set_Render(true);
                    
                }
            }
        }
        CAssetDefaultFont* pInfoFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_FishName");
        CAssetDefaultFont* pQuantity = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_FishQuantity");
        _vec2 vPos = { screen.x, screen.y};
        pInfoFont->Render_Font(m_sFishName, &vPos, D3DXCOLOR(200/255.f, 177/255.f, 154/255.f, 1.f));
        _vec2 vPos1 = { screen1.x, screen1.y };
       
        pQuantity->Render_Font(m_sFishQuantity, &vPos1, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    }

}

HRESULT CFishFrame::Ready_Component()
{
    // ¹öÆÛ
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // ÅØ½ºÃÄ
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Fishcell", L"Com_Texture", &m_pFishframeTextureCom))))
        return E_FAIL;
    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    //m_pTransformCom->m_vScale = { 0.6f, 0.2f, 1.f };
    return S_OK;
}


CFishFrame* CFishFrame::Create()
{
    CFishFrame* frame = new CFishFrame;

    if (FAILED(frame->Ready_GameObject()))
    {
        Safe_Release(frame);
        MSG_BOX("Fishframe Create Failed");
        return nullptr;
    }

    return frame;
}

void CFishFrame::Free()
{
    CGameObject::Free();
}
