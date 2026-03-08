#include "pch.h"
#include "CUpgradeFrame.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
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
#include "CUpgradeConfirmButton.h"
#include "CCloseButton.h"
#include "CGameMemMgr.h"
CUpgradeFrame::CUpgradeFrame()
    : CGameObject()
{
    m_bRender = false;
    m_sFishName = L"";
    m_sFishQuantity = L"";
    m_sQuantityRequired = L"";
    m_sCost1 = L"";
    m_sCost2 = L"";
    m_sQuality1 = L"";
    m_sQuality2 = L"";
    quantityRequired = false;
}

CUpgradeFrame::CUpgradeFrame(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CUpgradeFrame::~CUpgradeFrame()
{
}

HRESULT CUpgradeFrame::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;
    CAssetMgr::GetInstance()->AddAsset(L"Font_upgradeNumber", CAssetDefaultFont::Create(L"Arial", 0, 20, FW_BOLD));
    CAssetMgr::GetInstance()->LoadAsset(L"Font_upgradeNumber");
    //CAssetMgr::GetInstance()->AddAsset(L"Font_FishName", CAssetDefaultFont::Create(L"말군 고딕", 0, 23, FW_BOLD));
    //CAssetMgr::GetInstance()->AddAsset(L"Font_FishQuantity", CAssetDefaultFont::Create(L"영향", 0, 26, FW_BOLD));
    CAssetMgr::GetInstance()->LoadAsset(L"Font_FishName");
    CAssetMgr::GetInstance()->LoadAsset(L"Font_FishQuantity");
    m_pTransformCom->m_vInfo[INFO_POS] = { 0.f,0.f,-5.1f };
    m_pTransformCom->m_vScale = { 1.8f,2.3f,1.0f };
    _vec3 FishName = { -1.0f,4.f,0.f };
    CHelper::GetScreenPointFromWorld(&screen, &FishName);

    _vec3 quantity = { 2.3f,-0.22f,0.f };
    CHelper::GetScreenPointFromWorld(&screen1, &quantity);

    _vec3 FishName1 = { -2.28f,-0.1f,0.f };

    CHelper::GetScreenPointFromWorld(&screen2, &FishName1);

    _vec3 cost1 = { -0.85f,1.58f,0.f };

    CHelper::GetScreenPointFromWorld(&screen3, &cost1);
    _vec3 quality1 = { -0.85f,1.28f,0.f };

    CHelper::GetScreenPointFromWorld(&screen4, &quality1);

    _vec3 cost2 = { 1.95f,1.56f,0.f };

    CHelper::GetScreenPointFromWorld(&screen5, &cost2);
    _vec3 quality2 = { 1.95f,1.26f,0.f };

    CHelper::GetScreenPointFromWorld(&screen6, &quality2);
    CGameObject* pGameObject = nullptr;

    pGameObject = CBluejongP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishUpgradePic", pGameObject)))
        return E_FAIL;
    fishPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);
    pGameObject = CYellowbackP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishUpgradePic", pGameObject)))
        return E_FAIL;

    fishPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CDartP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishUpgradePic", pGameObject)))
        return E_FAIL;

    fishPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CClownFishP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishUpgradePic", pGameObject)))
        return E_FAIL;
    fishPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CYellowTangP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"fishUpgradePic", pGameObject)))
        return E_FAIL;
    fishPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);


    pGameObject = CBluejongR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiUpgradePic", pGameObject)))
        return E_FAIL;
    sushiPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);
    pGameObject = CYellowbackR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiUpgradePic", pGameObject)))
        return E_FAIL;

    sushiPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CDartR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiUpgradePic", pGameObject)))
        return E_FAIL;

    sushiPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CClownFishR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiUpgradePic", pGameObject)))
        return E_FAIL;
    sushiPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);

    pGameObject = CYellowTangR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiUpgradePic", pGameObject)))
        return E_FAIL;
    sushiPictures.push_back(pGameObject);
    frameGameObjects.push_back(pGameObject);
    pGameObject = CDepthP::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Depth_2", pGameObject)))
        return E_FAIL;
    {
        CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { -0.75f,-0.22f,-6.1f };
        pTransform->m_vScale = { 0.15f,0.05f,0.01f };
    }
    frameGameObjects.push_back(pGameObject);

    pGameObject = CUpgradeConfirmButton::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"UpgradeConfirm", pGameObject)))
        return E_FAIL;
    //static_cast<CUpgrade*>(pGameObject)->Set_Okay(L"Add");
    {
        CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { 0.5f,-1.62f,-6.f };
        pTransform->m_vScale = { 0.45f,0.13f,0.01f };
    }
    frameGameObjects.push_back(pGameObject);


    pGameObject = CCloseButton::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Close_1", pGameObject)))
        return E_FAIL;


    static_cast<CCloseButton*>(pGameObject)->Set_Frame(L"UpgradeConfirmFrame");
    {
        CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { -0.5f,-1.62f,-6.f };
        pTransform->m_vScale = { 0.45f,0.13f,0.01f };
    }
    frameGameObjects.push_back(pGameObject);

    
    list<CGameObject*>* picture = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"fishUpgradePic");
    list<CGameObject*>::iterator iter1 = picture->begin();
    for (iter1; iter1 != picture->end(); iter1++) {
        CTransform* pTransform = static_cast<CTransform*>((*iter1)->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { -1.075f,-0.16f,-6.1f };
        pTransform->m_vScale = { 0.15f,0.15f,1.0f };
    }
    list<CGameObject*>* sushi = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"sushiUpgradePic");
    list<CGameObject*>::iterator iter2 = sushi->begin();
    for (iter2; iter2 != sushi->end(); iter2++) {
        CTransform* pTransform = static_cast<CTransform*>((*iter2)->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { -0.85f,0.5,-6.1f };
        pTransform->m_vScale = { 0.2f, 0.1f, 1.f };
    }
    return S_OK;
}

_int CUpgradeFrame::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
       // m_sQuantitySelected = to_wstring(quantitySelected);
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    return iExit;
}

void CUpgradeFrame::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }

}

void CUpgradeFrame::Render_GameObject()
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

       // pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


        list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
        list<CGameObject*>::iterator iter = frame->begin();
        for (iter; iter != frame->end(); iter++) {
            if (static_cast<CSushiFrame*>(*iter)->m_bSelected == true) {
                CGameObject* depth = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Depth_2");
                depth->Set_Render(true);

                m_sFishName = static_cast<CSushiFrame*>(*iter)->fishName;
                m_sFishQuantity = static_cast<CSushiFrame*>(*iter)->m_sQuanity + L"/3";
                if (stoi(static_cast<CSushiFrame*>(*iter)->m_sQuanity) > 2) {
                    quantityRequired = true;
                }
                else {
                    quantityRequired = false;
                }
                //m_sQuantitySelected = 
                vector<CGameObject*>::iterator iter3 = fishPictures.begin();
                for (iter3; iter3 != fishPictures.end(); iter3++) {
                    (*iter3)->Set_Render(false);
                }
                vector<CGameObject*>::iterator iter4 = sushiPictures.begin();
                for (iter4; iter4 != sushiPictures.end(); iter4++) {
                    (*iter4)->Set_Render(false);
                }
                if (m_sFishName == L"블루종") {
                    fishPictures[0]->Set_Render(true);
                    sushiPictures[0]->Set_Render(true);
                }
                else if (m_sFishName == L"노랑백") {
                    fishPictures[1]->Set_Render(true);
                    sushiPictures[1]->Set_Render(true);
                }
                else if (m_sFishName == L"코반아지") {
                    fishPictures[2]->Set_Render(true);
                    sushiPictures[2]->Set_Render(true);
                }
                else if (m_sFishName == L"흰동가리") {
                    fishPictures[3]->Set_Render(true);
                    sushiPictures[3]->Set_Render(true);
                }
                else if (m_sFishName == L"노랑탕") {
                    fishPictures[4]->Set_Render(true);
                    sushiPictures[4]->Set_Render(true);
                }
            }
        }
        CGameObject* upgradeConfirm = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"UpgradeConfirm");
        vector<CGameMemMgr::FISH*> fishes = CGameMemMgr::GetInstance()->getFishes(); 
        for (auto fish : fishes) {
            if (fish->name == m_sFishName) {
                m_sCost1 = to_wstring(fish->cost);
                m_sCost2 = to_wstring(fish->cost + 3);
                m_sQuality1 = to_wstring(fish->quality);
                m_sQuality2 = to_wstring(fish->quality + 14);
                static_cast<CUpgradeConfirmButton*>(upgradeConfirm)->Set_WhichFish(m_sFishName);
            }
        }

        CAssetDefaultFont* pSushiNameFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_SushiName");

        CAssetDefaultFont* pFishNameFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_FishName");
        CAssetDefaultFont* pQuantityFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_FishQuantity");
        CAssetDefaultFont* pNumberFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_upgradeNumber");
        _vec2 vPos = { screen.x, screen.y };
        pFishNameFont->Render_Font(m_sFishName, &vPos, D3DXCOLOR(217 / 255.f, 209 / 255.f, 0 / 255.f, 1.f));
        _vec2 vPos1 = { screen1.x, screen1.y };
        if (!quantityRequired) {
            pQuantityFont->Render_Font(m_sFishQuantity, &vPos1, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

        }
        else {
            pQuantityFont->Render_Font(m_sFishQuantity, &vPos1, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        }
        _vec2 vPos2 = { screen2.x, screen2.y };
        pSushiNameFont->Render_Font(m_sFishName, &vPos2, D3DXCOLOR(200 / 255.f, 177 / 255.f, 154 / 255.f, 1.f));

        _vec2 vPos3 = { screen3.x, screen3.y };
        pNumberFont->Render_Font(m_sCost1, &vPos3, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

        _vec2 vPos4 = { screen4.x, screen4.y };
        pNumberFont->Render_Font(m_sQuality1, &vPos4, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

        _vec2 vPos5 = { screen5.x, screen5.y };
        pNumberFont->Render_Font(m_sCost2, &vPos5, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

        _vec2 vPos6 = { screen6.x, screen6.y };
        pNumberFont->Render_Font(m_sQuality2, &vPos6, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    }

}

HRESULT CUpgradeFrame::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_upgradeFrameTex", L"Com_Texture", &m_pFishConfirmframeTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    //m_pTransformCom->m_vScale = { 0.6f, 0.2f, 1.f };
    return S_OK;
}


CUpgradeFrame* CUpgradeFrame::Create()
{
    CUpgradeFrame* frame = new CUpgradeFrame;

    if (FAILED(frame->Ready_GameObject()))
    {
        Safe_Release(frame);
        MSG_BOX("confirmframe Create Failed");
        return nullptr;
    }

    return frame;
}

void CUpgradeFrame::Free()
{
    CGameObject::Free();
 /*   for_each(fishPictures.begin(), fishPictures.end(), Safe_Release<CGameObject*>);
    fishPictures.clear();
    for_each(sushiPictures.begin(), sushiPictures.end(), Safe_Release<CGameObject*>);
    sushiPictures.clear();
    for_each(frameGameObjects.begin(), frameGameObjects.end(), Safe_Release<CGameObject*>);
    frameGameObjects.clear();*/
}
