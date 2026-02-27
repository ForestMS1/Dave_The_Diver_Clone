#include "pch.h"
#include "CRedLionfish.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetSpine.h"
#include "CFishAABBCollider.h"
#include "CManagement.h"


using namespace Fish;

CRedLionfish::CRedLionfish(float fPosX, float fPosY, float fScale)
    : CFishGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_fScale(fScale)
{
}

CRedLionfish::~CRedLionfish()
{
}

HRESULT CRedLionfish::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { m_fScale, m_fScale, m_fScale };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fViewZ = 0.5f;

    m_fSpeed = 1.f;

    m_pSpineCom->Set_AniState(L"swim");
    m_eFishType = Fish::FT_AGRESSIVE;
    m_sAttackSpineAniName = L"sprint";
    return S_OK;
}

_int CRedLionfish::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CRedLionfish::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CRedLionfish::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CRedLionfish::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_RedLionfish");
    return S_OK;
}


CRedLionfish* CRedLionfish::Create(float fPosX, float fPosY, float fScale)
{
    CRedLionfish* pFish = new CRedLionfish{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CRedLionfish::Free()
{
    CFishGameObject::Free();
}
