#include "pch.h"
#include "CDevilScorpionFish.h"
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

CDevilScorpionFish::CDevilScorpionFish(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CDevilScorpionFish::~CDevilScorpionFish()
{
}

HRESULT CDevilScorpionFish::Ready_GameObject()
{

    m_sFishName = L"악마전갈고기";
    m_sThumbNailAssetName = L"Tex_FishThumb_Devil_ScorpionFish";
    m_iRank = 4;  // 랭크 4 (최상위)
    m_iStar = 4;  // 별 4개
    m_fWeight = 1.5f;  // 1.5kg
    m_iPrice = 20;  // 가격 20
    m_fLength = 25.f;  // 길이 25cm
    m_iMeatCnt = 3;  // 고기 3개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_Devil_ScorpionFish";
    m_iSushiMoney = 30;  // 스시 가격 30
    m_iSushiLv = 3;  // 스시 레벨 3


    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { m_fScale, m_fScale, m_fScale };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fSpeed = 1.f;

    m_pSpineCom->Set_AniState(L"swim");

    return S_OK;
}

_int CDevilScorpionFish::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CDevilScorpionFish::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CDevilScorpionFish::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CDevilScorpionFish::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_DevilScorpionFish");
    return S_OK;
}


CDevilScorpionFish* CDevilScorpionFish::Create(float fPosX, float fPosY, float fScale)
{
    CDevilScorpionFish* pFish = new CDevilScorpionFish{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CDevilScorpionFish::Free()
{
    CFishGameObject::Free();
}
