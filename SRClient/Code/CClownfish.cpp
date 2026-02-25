#include "pch.h"
#include "CClownfish.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetSpine.h"

using namespace Fish;

CClownfish::CClownfish(float fPosX, float fPosY, float fScale)
    : CFishGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_fScale(fScale)
{
}

CClownfish::~CClownfish()
{
}

HRESULT CClownfish::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { m_fScale, m_fScale, m_fScale };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fViewZ = 0.5f;

    m_fSpeed = 1.f;

    m_pSpineCom->Set_AniState(L"swim");


    return S_OK;
}

_int CClownfish::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CClownfish::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CClownfish::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CClownfish::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_Clownfish");
    return S_OK;
}


CClownfish* CClownfish::Create(float fPosX, float fPosY, float fScale)
{
    CClownfish* pFish = new CClownfish{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CClownfish::Free()
{
    CFishGameObject::Free();
}
