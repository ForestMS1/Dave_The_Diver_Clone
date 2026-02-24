#include "pch.h"
#include "CYellowbackFusilier.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetSpine.h"

using namespace Fish;

CYellowbackFusilier::CYellowbackFusilier(float fPosX, float fPosY)
    : CFishGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CYellowbackFusilier::~CYellowbackFusilier()
{
}

HRESULT CYellowbackFusilier::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 0.1, 0.1f, 0.1f };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fViewZ = 0.5f;

    return S_OK;
}

_int CYellowbackFusilier::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CYellowbackFusilier::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CYellowbackFusilier::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CYellowbackFusilier::Ready_Component()
{
    CFishGameObject::Ready_Component(L"Spine_YellowbackFusilier");
    return S_OK;
}


CYellowbackFusilier* CYellowbackFusilier::Create(float fPosX, float fPosY)
{
    CYellowbackFusilier* pFish = new CYellowbackFusilier{ fPosX, fPosY };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CYellowbackFusilier::Free()
{
    CGameObject::Free();
}
