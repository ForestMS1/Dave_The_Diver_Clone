#include "pch.h"
#include "CJuvenileCircularBatFish.h"
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

CJuvenileCircularBatFish::CJuvenileCircularBatFish(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CJuvenileCircularBatFish::~CJuvenileCircularBatFish()
{
}

HRESULT CJuvenileCircularBatFish::Ready_GameObject()
{
    m_sFishName = L"나뭇잎고기";
    m_sThumbNailAssetName = L"Tex_FishThumb_Juvenile_Circular_BatFish";
    m_iRank = 1;  // 랭크 1 (하위)
    m_iStar = 1;  // 별 1개
    m_fWeight = 0.3f + CHelper::RandRangeFromZero(5.f);  // 0.3kg
    m_iPrice = 6;  // 가격 6
    m_fLength = 12.f + CHelper::RandRangeFromZero(5.f);  // 길이 12cm
    m_iMeatCnt = 1;  // 고기 1개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_JuvenileCircularBatFishFry";
    m_iSushiMoney = 8;  // 스시 가격 8
    m_iSushiLv = 1;  // 스시 레벨 1

    m_iHP = 1;

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

_int CJuvenileCircularBatFish::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CJuvenileCircularBatFish::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CJuvenileCircularBatFish::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CJuvenileCircularBatFish::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_JuvenileCircularBatFish");
    return S_OK;
}


CJuvenileCircularBatFish* CJuvenileCircularBatFish::Create(float fPosX, float fPosY, float fScale)
{
    CJuvenileCircularBatFish* pFish = new CJuvenileCircularBatFish{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CJuvenileCircularBatFish::Free()
{
    CFishGameObject::Free();
}
