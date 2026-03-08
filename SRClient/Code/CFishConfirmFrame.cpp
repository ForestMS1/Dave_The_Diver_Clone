#include "pch.h"
#include "CFishConfirmFrame.h"
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
#include "CBluejongR.h"
#include "CClownFishR.h"
#include "CDartR.h"
#include "CYellowbackR.h"
#include "CYellowTangR.h"
#include "CSushiFrame.h"
#include "CAssetDefaultFont.h"
#include "CAssetMgr.h"
#include "CDepthP.h"
#include "COkayButton.h"
#include "CIncreaseButton.h"
#include "CLessButton.h"
#include "CMaxButton.h"
#include "CCloseButton.h"
#include "CBanchoR.h"
#include "CHeadP.h"
CFishConfirmFrame::CFishConfirmFrame()
    : CGameObject()
{
    m_sFishName = L"";
    m_sSushiName = L"";
    m_sFishQuantity = L"";
    m_sQuantitySelected = L"";
    quantitySelected = 1;
}

CFishConfirmFrame::CFishConfirmFrame(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CFishConfirmFrame::~CFishConfirmFrame()
{
}

HRESULT CFishConfirmFrame::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;
    //CAssetMgr::GetInstance()->AddAsset(L"Font_FishName", CAssetDefaultFont::Create(L"¸»±º °íµñ", 0, 23, FW_BOLD));
    //CAssetMgr::GetInstance()->AddAsset(L"Font_FishQuantity", CAssetDefaultFont::Create(L"¿µÇâ", 0, 26, FW_BOLD));
    CAssetMgr::GetInstance()->LoadAsset(L"Font_FishName");
    CAssetMgr::GetInstance()->LoadAsset(L"Font_FishQuantity");
    m_pTransformCom->m_vInfo[INFO_POS] = { 0.f,0.f,-5.1f };
    m_pTransformCom->m_vScale = { 1.5f,2.0f,1.0f };
    _vec3 FishName = { -1.68f,2.95f,0.f };
    CHelper::GetScreenPointFromWorld(&screen, &FishName);

    _vec3 quantity = { 1.9f,2.76f,0.f };
    CHelper::GetScreenPointFromWorld(&screen1, &quantity);

    _vec3 sushiName = { -0.65f,-2.f,0.f };

    CHelper::GetScreenPointFromWorld(&screen2, &sushiName);

    _vec3 selected1 = { -0.1f,0.4f,0.f };

    CHelper::GetScreenPointFromWorld(&screen3, &selected1);
    _vec3 selected2 = { 0.43f,-1.35f,0.f };

    CHelper::GetScreenPointFromWorld(&screen4, &selected2);
    CGameObject* pGameObject = nullptr;

    pGameObject = CBluejongP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishConfirmPic", pGameObject)))
        return E_FAIL;
    fishPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);
    pGameObject = CYellowbackP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishConfirmPic", pGameObject)))
        return E_FAIL;

    fishPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CDartP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishConfirmPic", pGameObject)))
        return E_FAIL;

    fishPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);


    

    pGameObject = CClownFishP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishConfirmPic", pGameObject)))
        return E_FAIL;
    fishPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CYellowTangP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishConfirmPic", pGameObject)))
        return E_FAIL;
    fishPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CHeadP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishConfirmPic", pGameObject)))
        return E_FAIL;

    fishPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CBluejongR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiConfirmPic", pGameObject)))
        return E_FAIL;
    sushiPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);
    pGameObject = CYellowbackR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiConfirmPic", pGameObject)))
        return E_FAIL;

    sushiPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CDartR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiConfirmPic", pGameObject)))
        return E_FAIL;

    sushiPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CClownFishR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiConfirmPic", pGameObject)))
        return E_FAIL;
    sushiPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CYellowTangR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiConfirmPic", pGameObject)))
        return E_FAIL;
    sushiPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CBanchoR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiConfirmPic", pGameObject)))
        return E_FAIL;
    sushiPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CDepthP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Depth_1", pGameObject)))
        return E_FAIL;
    {
        CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { -0.51f,0.92f,-6.1f };
        pTransform->m_vScale = { 0.17f,0.07f,0.01f };
    }
    frameGameObjects.push_back(pGameObject);

    pGameObject = COkayButton::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Okay_1", pGameObject)))
        return E_FAIL;
    static_cast<COkayButton*>(pGameObject)->Set_Okay(L"Add");
    {
        CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { -0.5f,-1.28f,-6.f };
        pTransform->m_vScale = { 0.45f,0.13f,0.01f };
    }
    frameGameObjects.push_back(pGameObject);


    pGameObject = CCloseButton::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Close_1", pGameObject)))
        return E_FAIL;
    static_cast<CCloseButton*>(pGameObject)->Set_Frame(L"FishConfirmFrame");
    {
        CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { 0.5f,-1.28f,-6.f };
        pTransform->m_vScale = { 0.45f,0.13f,0.01f };
    }
    frameGameObjects.push_back(pGameObject);

    pGameObject = CLessButton::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Less_1", pGameObject)))
        return E_FAIL;
    //static_cast<COkayButton*>(pGameObject)->Set_Okay(L"Add");
    {
        CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { -0.55f,0.1f,-6.1f };
        pTransform->m_vScale = { 0.07f,0.07f,0.01f };
    }
    frameGameObjects.push_back(pGameObject);

    pGameObject = CIncreaseButton::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Increase_1", pGameObject)))
        return E_FAIL;
    {
        CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { 0.55f,0.1f,-6.1f };
        pTransform->m_vScale = { 0.07f,0.07f,0.01f };
    }
    frameGameObjects.push_back(pGameObject);
   
    pGameObject = CMaxButton::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Max_1", pGameObject)))
        return E_FAIL;
    {
        CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { 0.f,0.3f,-6.1f };
        pTransform->m_vScale = { 0.38f,0.07f,0.01f };
    }
    frameGameObjects.push_back(pGameObject);

    list<CGameObject*>* picture = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"fishConfirmPic");
    list<CGameObject*>::iterator iter1 = picture->begin();
    for (iter1; iter1 != picture->end(); iter1++) {
        CTransform* pTransform = static_cast<CTransform*>((*iter1)->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { -0.89f,0.99f,-6.1f };
        pTransform->m_vScale = { 0.15f,0.15f,1.0f };
    }
    list<CGameObject*>* sushi = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"sushiConfirmPic");
    list<CGameObject*>::iterator iter2 = sushi->begin();
    for (iter2; iter2 != sushi->end(); iter2++) {
        CTransform* pTransform = static_cast<CTransform*>((*iter2)->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { 0.f,-0.41f,-6.1f };
        pTransform->m_vScale = { 0.2f, 0.1f, 1.f };
    }
    return S_OK;
}

_int CFishConfirmFrame::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
        m_sQuantitySelected = to_wstring(quantitySelected);
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    return iExit;
}

void CFishConfirmFrame::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }

}

void CFishConfirmFrame::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

       // pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pFishConfirmframeTextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

        //pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


        list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
        list<CGameObject*>::iterator iter = frame->begin();
        for (iter; iter != frame->end(); iter++) {
            if (static_cast<CSushiFrame*>(*iter)->m_bSelected == true) {
                CGameObject* depth = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Depth_1");
                depth->Set_Render(true);
             
                m_sFishName = static_cast<CSushiFrame*>(*iter)->fishName;
                if (m_sFishName == L"¹ÝÃÝ") {
                    m_sFishName = L"???";
                }
                m_sSushiName = m_sFishName + L" ÃÊ¹ä";
                m_sFishQuantity = static_cast<CSushiFrame*>(*iter)->m_sQuanity + L"/1";
                //m_sQuantitySelected = 
                vector<CGameObject*>::iterator iter3 = fishPictures.begin();
                for (iter3; iter3 != fishPictures.end(); iter3++) {
                    (*iter3)->Set_Render(false);
                }
                vector<CGameObject*>::iterator iter4 = sushiPictures.begin();
                for (iter4; iter4 != sushiPictures.end(); iter4++) {
                    (*iter4)->Set_Render(false);
                }
                if (m_sFishName == L"ºí·çÁ¾") {
                    fishPictures[0]->Set_Render(true);
                    sushiPictures[0]->Set_Render(true);
                }
                else if (m_sFishName == L"³ë¶û¹é") {
                    fishPictures[1]->Set_Render(true);
                    sushiPictures[1]->Set_Render(true);
                }
                else if (m_sFishName == L"ÄÚ¹Ý¾ÆÁö") {
                    fishPictures[2]->Set_Render(true);
                    sushiPictures[2]->Set_Render(true);
                }
                else if (m_sFishName == L"Èòµ¿°¡¸®") {
                    fishPictures[3]->Set_Render(true);
                    sushiPictures[3]->Set_Render(true);
                }
                else if (m_sFishName == L"³ë¶ûÅÁ") {
                    fishPictures[4]->Set_Render(true);
                    sushiPictures[4]->Set_Render(true);
                }
                else if (m_sFishName == L"???") {
                    fishPictures[5]->Set_Render(true);
                    sushiPictures[5]->Set_Render(true);
                    CTransform* pTransform = static_cast<CTransform*>(sushiPictures[5]->Get_Component(ID_DYNAMIC, L"Com_Transform"));
                    pTransform->m_vScale = { 0.2f, 0.13f, 1.f };

                }
            }
        }


        CAssetDefaultFont* pSushiNameFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_SushiName");

        CAssetDefaultFont* pFishNameFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_FishName");
        CAssetDefaultFont* pQuantityFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_FishQuantity");
        _vec2 vPos = { screen.x, screen.y };
        pFishNameFont->Render_Font(m_sFishName, &vPos, D3DXCOLOR(200 / 255.f, 177 / 255.f, 154 / 255.f, 1.f));
        _vec2 vPos1 = { screen1.x, screen1.y };
        pQuantityFont->Render_Font(m_sFishQuantity, &vPos1, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        _vec2 vPos2 = { screen2.x, screen2.y };
        pSushiNameFont->Render_Font(m_sSushiName, &vPos2, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

        _vec2 vPos3 = { screen3.x, screen3.y };
        pQuantityFont->Render_Font(m_sQuantitySelected, &vPos3, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

        _vec2 vPos4 = { screen4.x, screen4.y };
        pQuantityFont->Render_Font(m_sQuantitySelected, &vPos4, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    }

}

HRESULT CFishConfirmFrame::Ready_Component()
{
    // ¹öÆÛ
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // ÅØ½ºÃÄ
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_fishConfirmTex", L"Com_Texture", &m_pFishConfirmframeTextureCom))))
        return E_FAIL;
    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    //m_pTransformCom->m_vScale = { 0.6f, 0.2f, 1.f };
    return S_OK;
}


CFishConfirmFrame* CFishConfirmFrame::Create()
{
    CFishConfirmFrame* frame = new CFishConfirmFrame;

    if (FAILED(frame->Ready_GameObject()))
    {
        Safe_Release(frame);
        MSG_BOX("confirmframe Create Failed");
        return nullptr;
    }

    return frame;
}

void CFishConfirmFrame::Free()
{
    CGameObject::Free();
   /* for_each(fishPictures.begin(), fishPictures.end(), Safe_Release<CGameObject*>);
    fishPictures.clear();
    for_each(sushiPictures.begin(), sushiPictures.end(), Safe_Release<CGameObject*>);
    sushiPictures.clear();
    for_each(frameGameObjects.begin(), frameGameObjects.end(), Safe_Release<CGameObject*>);
    frameGameObjects.clear();*/
}
