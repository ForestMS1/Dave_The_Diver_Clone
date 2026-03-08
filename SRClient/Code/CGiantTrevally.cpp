#include "pch.h"
#include "CGiantTrevally.h"
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

CGiantTrevally::CGiantTrevally(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CGiantTrevally::~CGiantTrevally()
{
}

HRESULT CGiantTrevally::Ready_GameObject()
{
    m_sFishName = L"자이언트 트레발리";
    m_sThumbNailAssetName = L"Tex_FishThumb_Giant_Trevally";
    m_iRank = 4;  // 랭크 4 (최상위)
    m_iStar = 4;  // 별 4개
    m_fWeight = 2.0f + CHelper::RandRangeFromZero(5.f);  // 2.0kg
    m_iPrice = 25;  // 가격 25
    m_fLength = 35.f + CHelper::RandRangeFromZero(5.f);  // 길이 35cm
    m_iMeatCnt = 3;  // 고기 3개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_GiantTrevally";
    m_iSushiMoney = 40;  // 스시 가격 40
    m_iSushiLv = 3;  // 스시 레벨 3

    m_bNeedSlice = true;
    m_iHP = 5;

    m_sRunFromSpineAniName = L"sprint";

    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { m_fScale, m_fScale, m_fScale };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fSpeed = 1.f;

    m_pSpineCom->Set_AniState(L"swim");

    return S_OK;
}

_int CGiantTrevally::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CGiantTrevally::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CGiantTrevally::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CGiantTrevally::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_GiantTrevally");
    return S_OK;
}


CGiantTrevally* CGiantTrevally::Create(float fPosX, float fPosY, float fScale)
{
    CGiantTrevally* pFish = new CGiantTrevally{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CGiantTrevally::Free()
{
    CFishGameObject::Free();
}
