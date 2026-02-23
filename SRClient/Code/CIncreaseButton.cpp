#include "pch.h"
#include "CIncreaseButton.h"
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
#include "CSushiFrame.h"
#include "CFishConfirmFrame.h"
CIncreaseButton::CIncreaseButton()
    : CGameObject()
{
}

CIncreaseButton::CIncreaseButton(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CIncreaseButton::~CIncreaseButton()
{
}



HRESULT CIncreaseButton::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_pTransformCom->m_vInfo[INFO_POS] = { 2.11f,-1.88f,-6.f };
    m_pTransformCom->m_vScale = { 0.39f,0.11f,0.01f };
    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };
    _vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];

    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Increase", this);

        
    return S_OK;
}

_int CIncreaseButton::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
        CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Increase", m_pAABB);
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
            if (m_pAABB->Get_Tag() == L"AABB_Increase")
            {
                //현재 선택된 프레임의 물고기의 개수를 가져옴
                //프레임의 m_sQuantitySelected 를 increase한다;
                //m_sQuantitySelected가 물고기 개수를 넘으면 안올라간다.
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
                if (static_cast<CFishConfirmFrame*>(confirmFrame)->quantitySelected < max) {
                    static_cast<CFishConfirmFrame*>(confirmFrame)->quantitySelected++;
                }
               
            }
        }
    }
    return iExit;
}

void CIncreaseButton::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }

}

void CIncreaseButton::Render_GameObject()
{
    if (m_bRender) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pIncreaseTextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);



    }



    //float scalex = CInfoMgr::GetInstance()->Get_ScaleFactor();
}

HRESULT CIncreaseButton::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_arrowRightTex", L"Com_Texture", &m_pIncreaseTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;


    return S_OK;
}


CIncreaseButton* CIncreaseButton::Create()
{
    CIncreaseButton* increase = new CIncreaseButton;
    if (FAILED(increase->Ready_GameObject()))
    {
        Safe_Release(increase);
        MSG_BOX("increase Create Failed");
        return nullptr;
    }

    return increase;
}

void CIncreaseButton::Free()
{
    Safe_Release(m_pAABB);
    CGameObject::Free();
}
