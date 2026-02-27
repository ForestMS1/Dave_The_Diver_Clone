#include "pch.h"
#include "CCloseButton.h"
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
#include "CAssetDefaultFont.h"
#include "CAssetMgr.h"
#include "CGameMemMgr.h"
#include "CYellowbackR.h"
#include "CYellowTangR.h"
#include "CDartR.h"
#include "CClownFishR.h"
#include "CFishConfirmFrame.h"
#include "CSushiFrame.h"
#include "CMenuFrame.h"
#include "CCustomer1.h"
CCloseButton::CCloseButton()
    : CGameObject()
{
}

CCloseButton::CCloseButton(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CCloseButton::~CCloseButton()
{
}



HRESULT CCloseButton::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_pTransformCom->m_vInfo[INFO_POS] = { 2.94f,-1.88f,-6.f };
    m_pTransformCom->m_vScale = { 0.39f,0.11f,0.01f };
    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };
    _vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];

    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Close", this);
    return S_OK;
}

_int CCloseButton::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
        CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Frame", m_pAABB);
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
    m_pAABB->Transform(m_pTransformCom->Get_World());




  
    return iExit;
}

void CCloseButton::LateUpdate_GameObject(const _float& fTimeDelta)
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
                if (m_pAABB->Get_Tag() == L"AABB_Close")
                {
                    if (whichFrame == L"MenuFrame") {
                        // 다 닫기
                        CGameObject* menuFrame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"MenuFrame");
                        static_cast<CMenuFrame*>(menuFrame)->Hide();
                        list<CGameObject*>* addButton = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"AddButton");
                        if (addButton->size() == 0) {
                            CGameObject* openShop = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"OpenShop");
                            openShop->Set_Render(true);
                            for (int i = 0; i < 5; i++) {
                                CGameObject* pGameObject = CCustomer1::Create();
                                CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Customer", pGameObject);

                            }
                        }

                    }
                    else if (whichFrame == L"FishConfirmFrame") {

                        CGameObject* fishconfirm = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"FishConfirmFrame");
                        fishconfirm->Set_Render(false);
                        vector<CGameObject*> frameObjects = static_cast<CFishConfirmFrame*>(fishconfirm)->Get_CurObjects();
                        for (auto objects : frameObjects) {
                            objects->Set_Render(false);
                        }
                        CGameObject* overlay = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"Overlay");
                        overlay->Set_Render(false);

                        list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
                        list<CGameObject*>::iterator iter = frame->begin();
                        for (iter; iter != frame->end(); iter++) {
                            static_cast<CSushiFrame*>(*iter)->ConfirmOpened = false;
                        }
                    }
                    else if (whichFrame == L"RecipeFrame") {
                        // 레시피랑 스시리스트 닫기
                        CGameObject* recipe = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Recipe");

                        recipe->Set_Render(false);
                        CGameObject* button1 = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Close_2");
                        button1->Set_Render(false);
                        CGameObject* menuFrame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"MenuFrame");
                        static_cast<CMenuFrame*>(menuFrame)->Hide();
                        static_cast<CMenuFrame*>(menuFrame)->Reset_Frame();
                    }
                    else if (whichFrame == L"UpgradeConfirmFrame") {

                    }
                }
            }
        }
    }

}

void CCloseButton::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pCloseTextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);



    }



    //float scalex = CInfoMgr::GetInstance()->Get_ScaleFactor();
}

HRESULT CCloseButton::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_CloseButtonTex", L"Com_Texture", &m_pCloseTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;


    return S_OK;
}


CCloseButton* CCloseButton::Create()
{
    CCloseButton* closeButton = new CCloseButton;
    if (FAILED(closeButton->Ready_GameObject()))
    {
        Safe_Release(closeButton);
        MSG_BOX("closeButton Create Failed");
        return nullptr;
    }

    return closeButton;
}

void CCloseButton::Free()
{
    Safe_Release(m_pAABB);
    CGameObject::Free();
}
