#include "pch.h"
#include "CFishHQ.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CManagement.h"
#include "FishInclude.h"

CFishHQ::CFishHQ()
    : CGameObject()
{
}

CFishHQ::~CFishHQ()
{
}

HRESULT CFishHQ::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_fTimer = 0.f;
    m_iCnt = 0;
    return S_OK;
}

_int CFishHQ::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
    m_fTimer += fTimeDelta;

    if (m_fTimer > 1.f)
    {
        if (m_iCnt < 5)
        {
            float randX = rand() % 10;
            float randY = rand() % 5;
            if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
            {
                Fish::AddLayer_BlueTang(pLayer, randX, randY, 0.3f, this);
                ++m_iCnt;
            }
        }
        
        m_fTimer = 0.f;
    }



    return iExit;
}

void CFishHQ::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFishHQ::Render_GameObject()
{
}

HRESULT CFishHQ::Ready_Component()
{
    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CFishHQ* CFishHQ::Create()
{
    CFishHQ* pHQ = new CFishHQ;

    if (FAILED(pHQ->Ready_GameObject()))
    {
        Safe_Release(pHQ);
        MSG_BOX("pHQ Create Failed");
        return nullptr;
    }

    return pHQ;
}

void CFishHQ::Free()
{
    CGameObject::Free();
}
