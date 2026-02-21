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
    CAssetMgr::GetInstance()->AddAsset(L"Font_LevelInfo", CAssetDefaultFont::Create(L"Arial", 0, 20, FW_SEMIBOLD));
    CAssetMgr::GetInstance()->LoadAsset(L"Font_LevelInfo");
    CAssetMgr::GetInstance()->LoadAsset(L"Font_DefaultXX");
   // CAssetMgr::GetInstance()->LoadAsset(L"Font_LevelInfo");
    return S_OK;
}

_int CSushiList::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
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
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pListextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

        // 스시 설명란
        list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
        list<CGameObject*>::iterator iter = frame->begin();
        for (iter; iter != frame->end(); iter++) {
            if (static_cast<CSushiFrame*>(*iter)->m_bSelected == true) {
                m_sFishName = static_cast<CSushiFrame*>(*iter)->fishName;
                m_sSushiName = m_sFishName + L" 초밥";
                m_sFishInfo = m_sFishName + L"으로 만든 초밥이다.";
            }
        }
        vector<CGameMemMgr::FISH*> fishes = CGameMemMgr::GetInstance()->getFishes();
        vector<CGameMemMgr::FISH*>::iterator iter1 = fishes.begin();

        for (iter1; iter1 != fishes.end(); iter1++) {
            if ((*iter1)->name == m_sFishName) {
                m_sLevel = L"Lv." + std::to_wstring((*iter1)->level);
                m_sQuanity = std::to_wstring((*iter1)->quantity);
                m_sQuality = L"30";
                m_sCost = std::to_wstring((*iter1)->cost);
                m_sPlate = L"1";
            }
        }
     
    }
    //selected 된 스시 프레임을 찾아서 fishName을 가져와서

    CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_LevelInfo");
    CAssetDefaultFont* plevelFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_LevelInfo");
    _vec2 vPos = { screen.x +  screen.x / 2, screen.y / 10 };
    pDefFont->Render_Font(m_sSushiName, &vPos, D3DXCOLOR(1.f, 1.f, 0.87f, 1.f));

    _vec2 vPos1 = { screen.x + screen.x/3, screen.y - screen.y/ 6 };
    plevelFont->Render_Font(m_sFishInfo, &vPos1, D3DXCOLOR(1.f, 0.9f, 0.8f, 1.f));
   



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
