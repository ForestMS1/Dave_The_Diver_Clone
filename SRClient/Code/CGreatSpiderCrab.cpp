#include "pch.h"
#include "CGreatSpiderCrab.h"
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

CGreatSpiderCrab::CGreatSpiderCrab(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CGreatSpiderCrab::~CGreatSpiderCrab()
{
}

HRESULT CGreatSpiderCrab::Ready_GameObject()
{
    m_sFishName = L"키다리게";
    m_sThumbNailAssetName = L"Tex_FishThumb_Great_Spider_Crab";
    m_iRank = 3;  // 랭크 3 (상위)
    m_iStar = 3;  // 별 3개
    m_fWeight = 3.0f + CHelper::RandRangeFromZero(0.5f);  // 3.0kg
    m_iPrice = 30;  // 가격 30
    m_fLength = 40.f + CHelper::RandRangeFromZero(0.5f);  // 길이 40cm
    m_iMeatCnt = 4;  // 고기 4개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_GreatSpiderCrab";
    m_iSushiMoney = 50;  // 스시 가격 50
    m_iSushiLv = 3;  // 스시 레벨 3

    m_iHP = 2;


    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { m_fScale, m_fScale, m_fScale };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fSpeed = 1.f;

    m_pSpineCom->Set_AniState(L"swim");

    return S_OK;
}

_int CGreatSpiderCrab::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CGreatSpiderCrab::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CGreatSpiderCrab::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CGreatSpiderCrab::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_GreatSpiderCrab");
    return S_OK;
}


CGreatSpiderCrab* CGreatSpiderCrab::Create(float fPosX, float fPosY, float fScale)
{
    CGreatSpiderCrab* pFish = new CGreatSpiderCrab{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CGreatSpiderCrab::Free()
{
    CFishGameObject::Free();
}
