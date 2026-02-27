#include "pch.h"
#include "CUpgradeConfirmButton.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CHelper.h"
#include "CDInputMgr.h"
#include "CAssetDefaultFont.h"
#include "CAssetMgr.h"
#include "CGameMemMgr.h"
#include "CSushiFrame.h"
#include "CUpgradeFrame.h"
CUpgradeConfirmButton::CUpgradeConfirmButton()
    : CGameObject()
{
    whichFish = L"";
}

CUpgradeConfirmButton::CUpgradeConfirmButton(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CUpgradeConfirmButton::~CUpgradeConfirmButton()
{
}



HRESULT CUpgradeConfirmButton::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_pTransformCom->m_vInfo[INFO_POS] = { 2.94f,-1.88f,-6.f };
    m_pTransformCom->m_vScale = { 0.39f,0.11f,0.01f };
    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };
    _vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];

    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_UpgradeConfirm", this);
    return S_OK;
}

_int CUpgradeConfirmButton::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject* upgradeFrame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"UpgradeFrame");

    if (m_bRender && static_cast<CUpgradeFrame*>(upgradeFrame)->quantityRequired) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
        CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Frame", m_pAABB);
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
    m_pAABB->Transform(m_pTransformCom->Get_World());




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
            if (m_pAABB->Get_Tag() == L"AABB_UpgradeConfirm")
            {

               // vector<CGameMemMgr::FISH*>& fishes = CGameMemMgr::GetInstance()->getFishes();
                for (auto &fish : CGameMemMgr::GetInstance()->getFishes()) {
                    if (fish->name == whichFish) {
                        fish->level += 1;
                        fish->quantity -= 3;
                        fish->cost += 3;
                        fish->quality += 14;
                    }
                }


                CGameObject* upgradeFrame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"UpgradeFrame");
                upgradeFrame->Set_Render(false);
                vector<CGameObject*> frameObjects = static_cast<CUpgradeFrame*>(upgradeFrame)->Get_CurObjects();
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
                CGameObject* button1 = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Close_2");
                button1->Set_Render(true);
            /*    if (whichFish == L"블루종") {

                }
                else if (whichFish == L"노랑백") {
                }
                else if (whichFish == L"코반아지") {
                }
                else if (whichFish == L"흰동가리") {
                }
                else if (whichFish == L"노랑탕") {
                }*/

            }
        }
    }
    return iExit;
}

void CUpgradeConfirmButton::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }

}

void CUpgradeConfirmButton::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pUpgradeTextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);



    }



    //float scalex = CInfoMgr::GetInstance()->Get_ScaleFactor();
}

HRESULT CUpgradeConfirmButton::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_upgradeConfirmTex", L"Com_Texture", &m_pUpgradeTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;


    return S_OK;
}


CUpgradeConfirmButton* CUpgradeConfirmButton::Create()
{
    CUpgradeConfirmButton* upgrade = new CUpgradeConfirmButton;
    if (FAILED(upgrade->Ready_GameObject()))
    {
        Safe_Release(upgrade);
        MSG_BOX("UpgradeConfirm Create Failed");
        return nullptr;
    }

    return upgrade;
}

void CUpgradeConfirmButton::Free()
{
    Safe_Release(m_pAABB);
    CGameObject::Free();
}
