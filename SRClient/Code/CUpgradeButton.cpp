#include "pch.h"
#include "CUpgradeButton.h"
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
#include "CUpgradeFrame.h"
#include "CSushiFrame.h"
#include "CUpgradeConfirmButton.h"
CUpgradeButton::CUpgradeButton()
    : CGameObject()
{
}

CUpgradeButton::CUpgradeButton(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CUpgradeButton::~CUpgradeButton()
{
}



HRESULT CUpgradeButton::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_pTransformCom->m_vInfo[INFO_POS] = { 2.94f,-1.88f,-6.f };
    m_pTransformCom->m_vScale = { 0.39f,0.11f,0.01f };
    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };
    _vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];

    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Upgrade", this);
    return S_OK;
}

_int CUpgradeButton::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
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
            if (m_pAABB->Get_Tag() == L"AABB_Upgrade")
            {
                CGameObject* upgradeFrame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"UpgradeFrame");
                upgradeFrame->Set_Render(true);
                CGameObject* overlay = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"Overlay");
                CTransform* pTransform = static_cast<CTransform*>(overlay->Get_Component(ID_DYNAMIC, L"Com_Transform"));
                CTransform* confirmTransform = static_cast<CTransform*>(upgradeFrame->Get_Component(ID_DYNAMIC, L"Com_Transform"));
                pTransform->m_vInfo[INFO_POS] = confirmTransform->m_vInfo[INFO_POS];
                pTransform->m_vInfo[INFO_POS].z += 0.05f;
                overlay->Set_Render(true);
                vector<CGameObject*> frameObjects = static_cast<CUpgradeFrame*>(upgradeFrame)->Get_CurObjects();
                for (auto objects : frameObjects) {
                    objects->Set_Render(true);
                }


                list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
                list<CGameObject*>::iterator iter = frame->begin();
                for (iter; iter != frame->end(); iter++) {
                    static_cast<CSushiFrame*>(*iter)->ConfirmOpened = true;
                
                }
                CGameObject* button1 = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"Close_2");
                button1->Set_Render(false);
            }
        }
    }
    return iExit;
}

void CUpgradeButton::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }

}

void CUpgradeButton::Render_GameObject()
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

HRESULT CUpgradeButton::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_UpgradeTex", L"Com_Texture", &m_pUpgradeTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;


    return S_OK;
}


CUpgradeButton* CUpgradeButton::Create()
{
    CUpgradeButton* upgrade = new CUpgradeButton;
    if (FAILED(upgrade->Ready_GameObject()))
    {
        Safe_Release(upgrade);
        MSG_BOX("Upgrade Create Failed");
        return nullptr;
    }

    return upgrade;
}

void CUpgradeButton::Free()
{
    Safe_Release(m_pAABB);
    CGameObject::Free();
}
