#include "pch.h"
#include "COkayButton.h"
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
#include "CAddMenuButton.h"
#include "CSelectedFrame.h"
COkayButton::COkayButton()
    : CGameObject()
{
    whichOkay = L"";
}

COkayButton::COkayButton(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

COkayButton::~COkayButton()
{
}



HRESULT COkayButton::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_pTransformCom->m_vInfo[INFO_POS] = { 2.11f,-1.88f,-6.f };
    m_pTransformCom->m_vScale = { 0.39f,0.11f,0.01f };
    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };
    _vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];

    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Okay", this);
    return S_OK;
}

_int COkayButton::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
        CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Frame", m_pAABB);
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
    m_pAABB->Transform(m_pTransformCom->Get_World());


  


    return iExit;
}

void COkayButton::LateUpdate_GameObject(const _float& fTimeDelta)
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
                if (m_pAABB->Get_Tag() == L"AABB_Okay")
                {

                    if (whichOkay == L"FishConfirmOpen") {
                        
                        CGameObject* fishconfirm = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"FishConfirmFrame");
                        fishconfirm->Set_Render(true);
                        static_cast<CFishConfirmFrame*>(fishconfirm)->quantitySelected = 1;
                        CGameObject* overlay = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"Overlay");
                        CTransform* pTransform = static_cast<CTransform*>(overlay->Get_Component(ID_DYNAMIC, L"Com_Transform"));
                        CTransform* confirmTransform = static_cast<CTransform*>(fishconfirm->Get_Component(ID_DYNAMIC, L"Com_Transform"));
                        pTransform->m_vInfo[INFO_POS] = confirmTransform->m_vInfo[INFO_POS];
                        pTransform->m_vInfo[INFO_POS].z += 0.05f;
                        overlay->Set_Render(true);
                        vector<CGameObject*> frameObjects = static_cast<CFishConfirmFrame*>(fishconfirm)->Get_CurObjects();
                        for (auto objects : frameObjects) {
                            objects->Set_Render(true);
                        }
                        list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
                        list<CGameObject*>::iterator iter = frame->begin();
                        for (iter; iter != frame->end(); iter++) {
                            static_cast<CSushiFrame*>(*iter)->ConfirmOpened = true;
                        }
                    }
                    else if (whichOkay == L"Add") {
                        CTransform* pTransform;
                        
                        _vec3 scale, pos;
                        list<CGameObject*>* addButton = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"AddButton");
                        list<CGameObject*>::iterator iter = addButton->begin();
                        for (iter; iter != addButton->end(); iter++) {
                            if (static_cast<CAddMenuButton*>(*iter)->m_bSelected == true) {
                                pTransform = static_cast<CTransform*>((*iter)->Get_Component(ID_DYNAMIC, L"Com_Transform"));
                                scale = pTransform->m_vScale;
                                pos = pTransform->m_vInfo[INFO_POS];
                                Safe_Release(*iter);
                            }
                        }
                        CGameObject* frameObject = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"FishConfirmFrame");
                        wstring cost, quantity, fishName;
                        quantity = static_cast<CFishConfirmFrame*>(frameObject)->m_sQuantitySelected;
                        fishName = static_cast<CFishConfirmFrame*>(frameObject)->m_sFishName;
                        CGameObject* pGameObject = CSelectedFrame::Create();
                        static_cast<CSelectedFrame*>(pGameObject)->Set_Quantity(quantity);
                        static_cast<CSelectedFrame*>(pGameObject)->Set_Plate(quantity);
                        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"SelectedFrame", pGameObject);
                        CTransform* frameTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
                        frameTransform->m_vInfo[INFO_POS] = pos;
                        frameTransform->m_vScale = scale;
                        pGameObject->Set_Render(true);
                        static_cast<CSelectedFrame*>(pGameObject)->created = true;
                        vector<CGameObject*> confirmObjects = static_cast<CFishConfirmFrame*>(frameObject)->Get_CurObjects();
                        static_cast<CFishConfirmFrame*>(frameObject)->Set_Render(false);
                        for (auto objects : confirmObjects) {
                            objects->Set_Render(false);
                        }

                        CGameObject* overlay = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"Overlay");
                        overlay->Set_Render(false);

                        vector<CGameMemMgr::FISH*> fishes = CGameMemMgr::GetInstance()->getFishes();
                        vector<CGameMemMgr::FISH*>::iterator iter1 = fishes.begin();
                        for (iter1; iter1 != fishes.end(); iter1++) {
                            if (fishName == (*iter1)->name) {
                                (*iter1)->quantity = (*iter1)->quantity - stoi(quantity);
                            }
                        }
                        list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
                        list<CGameObject*>::iterator iter2 = frame->begin();
                        for (iter2; iter2 != frame->end(); iter2++) {
                            static_cast<CSushiFrame*>(*iter2)->ConfirmOpened = false;
                          
                        }
                    }
                    else if (whichOkay == L"Upgrade") {

                    }

                }
            }
        }
    }

}

void COkayButton::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pOkayTextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);



    }



    //float scalex = CInfoMgr::GetInstance()->Get_ScaleFactor();
}

HRESULT COkayButton::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_OkayTex", L"Com_Texture", &m_pOkayTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;


    return S_OK;
}


COkayButton* COkayButton::Create()
{
    COkayButton* okay = new COkayButton;
    if (FAILED(okay->Ready_GameObject()))
    {
        Safe_Release(okay);
        MSG_BOX("okay Create Failed");
        return nullptr;
    }

    return okay;
}

void COkayButton::Free()
{
    Safe_Release(m_pAABB);
    CGameObject::Free();
}
