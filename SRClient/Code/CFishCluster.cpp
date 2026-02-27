#include "pch.h"
#include "CFishCluster.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"

CFishCluster::CFishCluster()
    : CGameObject()
{
}

CFishCluster::~CFishCluster()
{
}

HRESULT CFishCluster::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    return S_OK;
}

_int CFishCluster::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CFishCluster::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFishCluster::Render_GameObject()
{
}

HRESULT CFishCluster::Ready_Component()
{
    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CFishCluster* CFishCluster::Create()
{
    CFishCluster* pCluster = new CFishCluster;

    if (FAILED(pCluster->Ready_GameObject()))
    {
        Safe_Release(pCluster);
        MSG_BOX("pCluster Create Failed");
        return nullptr;
    }

    return pCluster;
}

void CFishCluster::Free()
{
    CGameObject::Free();
}
