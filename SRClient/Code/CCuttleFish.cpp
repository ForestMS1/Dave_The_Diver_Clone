#include "pch.h"
#include "CCuttleFish.h"
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

CCuttleFish::CCuttleFish(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CCuttleFish::~CCuttleFish()
{
}

HRESULT CCuttleFish::Ready_GameObject()
{

    m_sFishName = L"훔볼트 오징어";
    m_sThumbNailAssetName = L"Tex_FishThumb_Humboldt_Squid";
    m_iRank = 2;  // 랭크 2 (중간)
    m_iStar = 2;  // 별 2개
    m_fWeight = 1.0f;  // 1.0kg
    m_iPrice = 13;  // 가격 13
    m_fLength = 25.f;  // 길이 25cm
    m_iMeatCnt = 2;  // 고기 2개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_Humboldt_Squid";
    m_iSushiMoney = 20;  // 스시 가격 20
    m_iSushiLv = 2;  // 스시 레벨 2

    m_iHP = 2;

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

_int CCuttleFish::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CCuttleFish::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CCuttleFish::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CCuttleFish::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_CuttleFish");
    return S_OK;
}


CCuttleFish* CCuttleFish::Create(float fPosX, float fPosY, float fScale)
{
    CCuttleFish* pFish = new CCuttleFish{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CCuttleFish::Free()
{
    CFishGameObject::Free();
}
