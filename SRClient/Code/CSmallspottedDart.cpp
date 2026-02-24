#include "pch.h"
#include "CSmallspottedDart.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetSpine.h"

using namespace Fish;

CSmallspottedDart::CSmallspottedDart(float fPosX, float fPosY)
    : CFishGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CSmallspottedDart::~CSmallspottedDart()
{
}

HRESULT CSmallspottedDart::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 0.1, 0.1f, 0.1f };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fViewZ = 0.5f;

    return S_OK;
}

_int CSmallspottedDart::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CSmallspottedDart::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CSmallspottedDart::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CSmallspottedDart::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_SmallspottedDart");
    return S_OK;
}


CSmallspottedDart* CSmallspottedDart::Create(float fPosX, float fPosY)
{
    CSmallspottedDart* pFish = new CSmallspottedDart{ fPosX, fPosY };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CSmallspottedDart::Free()
{
    CFishGameObject::Free();
}
