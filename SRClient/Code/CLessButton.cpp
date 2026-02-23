#include "pch.h"
#include "CLessButton.h"
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
CLessButton::CLessButton()
    : CGameObject()
{
}

CLessButton::CLessButton(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CLessButton::~CLessButton()
{
}



HRESULT CLessButton::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_pTransformCom->m_vInfo[INFO_POS] = { 2.11f,-1.88f,-6.f };
    m_pTransformCom->m_vScale = { 0.39f,0.11f,0.01f };
    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };
    _vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];

    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Less", this);
    return S_OK;
}

_int CLessButton::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
        CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Frame", m_pAABB);
    }
    m_pAABB->Transform(m_pTransformCom->Get_World());
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);





   
    return iExit;
}

void CLessButton::LateUpdate_GameObject(const _float& fTimeDelta)
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
                if (m_pAABB->Get_Tag() == L"AABB_Less")
                {
                    int max = 0;
                    list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
                    list<CGameObject*>::iterator iter = frame->begin();
                    CGameObject* confirmFrame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"FishConfirmFrame");
                    int curQuantity = static_cast<CFishConfirmFrame*>(confirmFrame)->quantitySelected;
                    for (iter; iter != frame->end(); iter++) {
                        if (static_cast<CSushiFrame*>(*iter)->m_bSelected == true) {
                            max = std::stoi(static_cast<CSushiFrame*>(*iter)->m_sQuanity);
                        }
                    }
                    if (static_cast<CFishConfirmFrame*>(confirmFrame)->quantitySelected > 0) {
                        static_cast<CFishConfirmFrame*>(confirmFrame)->quantitySelected--;
                    }


                }
            }
        }
    }

}

void CLessButton::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pLessTextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);



    }



    //float scalex = CInfoMgr::GetInstance()->Get_ScaleFactor();
}

HRESULT CLessButton::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_arrowLeftTex", L"Com_Texture", &m_pLessTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;


    return S_OK;
}


CLessButton* CLessButton::Create()
{
    CLessButton* less = new CLessButton;
    if (FAILED(less->Ready_GameObject()))
    {
        Safe_Release(less);
        MSG_BOX("less Create Failed");
        return nullptr;
    }

    return less;
}

void CLessButton::Free()
{
    Safe_Release(m_pAABB);
    CGameObject::Free();
}
