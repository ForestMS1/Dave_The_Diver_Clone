#include "pch.h"
#include "CSushiList.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CSushiFrame.h"
#include "CGameMemMgr.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CAssetMgr.h"
#include "CInfoMgr.h"
#include "CBluejongR.h"
#include "CYellowbackR.h"
#include "CYellowTangR.h"
#include "CDartR.h"
#include "CClownFishR.h"
#include "CFishFrame.h"
#include "CUpgradeButton.h"
#include "COkayButton.h"
#include "CFishConfirmFrame.h"
#include "CUpgradeFrame.h"
CSushiList::CSushiList()
    : CGameObject()
{
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

    _vec3 curPos;
    m_pTransformCom->Get_Info(INFO_POS, &curPos);
    CHelper::GetScreenPointFromWorld(&screen, &curPos);
    CAssetMgr::GetInstance()->AddAsset(L"Font_SushiName", CAssetDefaultFont::Create(L"Arial", 0, 20, FW_BOLD));
    CAssetMgr::GetInstance()->AddAsset(L"Font_SushiInfo", CAssetDefaultFont::Create(L"Arial", 0, 23, FW_BOLD));
    CAssetMgr::GetInstance()->AddAsset(L"Font_SushiNumber", CAssetDefaultFont::Create(L"Arial", 0, 20, FW_BOLD));
    CAssetMgr::GetInstance()->AddAsset(L"Font_SushiLevel", CAssetDefaultFont::Create(L"Arial", 0, 20, FW_BOLD));
    CAssetMgr::GetInstance()->LoadAsset(L"Font_SushiName");
    CAssetMgr::GetInstance()->LoadAsset(L"Font_Level");
    CAssetMgr::GetInstance()->LoadAsset(L"Font_SushiInfo");
    CAssetMgr::GetInstance()->LoadAsset(L"Font_SushiNumber");
    CAssetMgr::GetInstance()->LoadAsset(L"Font_SushiLevel");
    // CAssetMgr::GetInstance()->LoadAsset(L"Font_LevelInfo");

    CGameObject* pGameObject = nullptr;
    pGameObject = CBluejongR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiPic", pGameObject)))
        return E_FAIL;
    pictures.push_back(pGameObject);
    pGameObject = CYellowbackR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiPic", pGameObject)))
        return E_FAIL;
    pictures.push_back(pGameObject);

    pGameObject = CDartR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiPic", pGameObject)))
        return E_FAIL;
    pictures.push_back(pGameObject);

    pGameObject = CClownFishR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiPic", pGameObject)))
        return E_FAIL;
    pictures.push_back(pGameObject);

    pGameObject = CYellowTangR::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"sushiPic", pGameObject)))
        return E_FAIL;
    pictures.push_back(pGameObject);

    pGameObject = CFishFrame::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"FishCell", pGameObject)))
        return E_FAIL;
    if (pGameObject != nullptr) {
        CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vScale = { 1.7f, 0.3f, 1.f };
        pTransform->m_vInfo[INFO_POS] = { 3.4f,-0.3f,-4.6f };

    }
    pGameObject = CUpgradeButton::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Upgrade", pGameObject)))
        return E_FAIL;
    {
        CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { 3.76f ,-2.4f ,-4.9f };
        pTransform->m_vScale = { 0.5f,0.14f,0.01f };
    }
    pGameObject = COkayButton::Create();
 
    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Okay", pGameObject)))
        return E_FAIL;
    static_cast<COkayButton*>(pGameObject)->Set_Okay(L"FishConfirmOpen");
    CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
    pTransform->m_vInfo[INFO_POS] = { 2.7f ,-2.4f ,-4.9f };
    pTransform->m_vScale = { 0.5f,0.14f,0.01f };

    pGameObject = CFishConfirmFrame::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"FishConfirmFrame", pGameObject)))
        return E_FAIL;
    pGameObject = CUpgradeFrame::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"UpgradeFrame", pGameObject)))
        return E_FAIL;
        list<CGameObject*>* picture = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"sushiPic");
        list<CGameObject*>::iterator iter2 = picture->begin();
        for (iter2; iter2 != picture->end(); iter2++) {
            CTransform* pTransform = static_cast<CTransform*>((*iter2)->Get_Component(ID_DYNAMIC, L"Com_Transform"));
            pTransform->m_vScale = { 0.25f, 0.15f, 1.f };
            pTransform->m_vInfo[INFO_POS] = m_pTransformCom->m_vInfo[INFO_POS];
            pTransform->m_vInfo[INFO_POS].z -= 0.1f;
            pTransform->m_vInfo[INFO_POS].x -= 0.43f;
            pTransform->m_vInfo[INFO_POS].y += 1.9f;
        }
    return S_OK;
}

_int CSushiList::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    }
    else {
        CGameObject* upgradeButton = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Upgrade");
        upgradeButton->Set_Render(false);
        CGameObject* okayButton = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Okay");
        okayButton->Set_Render(false);
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    return iExit;
}

void CSushiList::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }

}

void CSushiList::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pListextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();
        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

        // 스시 설명란
        list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
        list<CGameObject*>::iterator iter = frame->begin();
        CGameObject* upgradeButton = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Upgrade");
        CGameObject* okayButton = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Okay");
        for (iter; iter != frame->end(); iter++) {
            if (static_cast<CSushiFrame*>(*iter)->m_bSelected == true) {
                upgradeButton->Set_Render(true);
                okayButton->Set_Render(true);
                m_sFishName = static_cast<CSushiFrame*>(*iter)->fishName;
                m_sSushiName = m_sFishName + L" 초밥";
                m_sFishInfo = m_sFishName + L"으로 만든 초밥이다.";
                vector<CGameObject*>::iterator iter3 = pictures.begin();
                for (iter3; iter3 != pictures.end(); iter3++) {
                    (*iter3)->Set_Render(false);
                }
                if (m_sFishName == L"블루종") {
                    pictures[0]->Set_Render(true);
                }else if (m_sFishName == L"노랑백") {
                    pictures[1]->Set_Render(true);
                }
                else if (m_sFishName == L"코반아지") {
                    pictures[2]->Set_Render(true);
                }
                else if (m_sFishName == L"흰동가리") {
                    pictures[3]->Set_Render(true);
                }
                else if (m_sFishName == L"노랑탕") {
                    pictures[4]->Set_Render(true);
                }
            }
        }
        vector<CGameMemMgr::FISH*> fishes = CGameMemMgr::GetInstance()->getFishes();
        vector<CGameMemMgr::FISH*>::iterator iter1 = fishes.begin();

        for (iter1; iter1 != fishes.end(); iter1++) {
            if ((*iter1)->name == m_sFishName) {
                m_sLevel = L"Lv." + std::to_wstring((*iter1)->level);
                m_sQuanity = std::to_wstring((*iter1)->quantity);
                m_sQuality = std::to_wstring((*iter1)->quality);
                m_sCost = std::to_wstring((*iter1)->cost);
                m_sPlate = L"1";
            }
        }
        //selected 된 스시 프레임을 찾아서 fishName을 가져와서

        CAssetDefaultFont* pNameFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_SushiName");
        CAssetDefaultFont* pInfoFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_SushiInfo");
        CAssetDefaultFont* pNumberFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_SushiNumber");
        CAssetDefaultFont* plevelFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_Level");
        _vec2 vPos = { screen.x + 11 * screen.x / 20, screen.y / 10 };
        pNameFont->Render_Font(m_sSushiName, &vPos, D3DXCOLOR(1.f, 1.f, 0.8f, 1.f));


        _vec2 vPos1 = { screen.x + 6 * screen.x / 20, screen.y - screen.y / 4 };
        pInfoFont->Render_Font(m_sFishInfo, &vPos1, D3DXCOLOR(181 / 255.f, 161 / 255.f, 144 / 255.f, 1.f));

        _vec2 vPos2 = { screen.x + 15 * screen.x / 22, screen.y / 3 - screen.y / 100 };
        pNumberFont->Render_Font(m_sCost, &vPos2, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

        _vec2 vPos3 = { screen.x + 15 * screen.x / 22, screen.y / 3 + screen.y / 20 };
        pNumberFont->Render_Font(m_sQuality, &vPos3, D3DXCOLOR(255 / 255.f, 171 / 255.f, 25 / 255.f, 1.f));

        _vec2 vPos4 = { screen.x + 15 * screen.x / 22,screen.y / 3 + 6 * screen.y / 55 };
        pNumberFont->Render_Font(m_sPlate, &vPos4, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

        _vec2 vPos5 = { screen.x + 16 * screen.x / 24,screen.y / 3 - 5 * screen.y / 57 };
        plevelFont->Render_Font(m_sLevel, &vPos5, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    }
    else {
        list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
        list<CGameObject*>::iterator iter = frame->begin();
        CGameObject* upgradeButton = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Upgrade");
        CGameObject* okayButton = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Okay");
        for (iter; iter != frame->end(); iter++) {
            if (static_cast<CSushiFrame*>(*iter)->m_bSelected == true) {
                upgradeButton->Set_Render(false);
                okayButton->Set_Render(false);
                vector<CGameObject*>::iterator iter3 = pictures.begin();
                for (iter3; iter3 != pictures.end(); iter3++) {
                    (*iter3)->Set_Render(false);
                }
              
            }
        }
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
        MSG_BOX("sushiList Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CSushiList::Free()
{
    CGameObject::Free();
    for_each(pictures.begin(), pictures.end(), Safe_Release<CGameObject*>);
    pictures.clear();
    
}
