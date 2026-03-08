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
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CClownfish::~CClownfish()
{
}

HRESULT CClownfish::Ready_GameObject()
{
    m_sFishName = L"흰동가리";
    m_sThumbNailAssetName = L"Tex_FishThumb_ClownFish";
    m_iRank = 1;  // 랭크 1 (하위)
    m_iStar = 1;  // 별 1개
    m_fWeight = 0.7f + CHelper::RandRangeFromZero(0.5f);  // 0.1kg
    m_iPrice = 4;  // 가격 4
    m_fLength = 7.f + CHelper::RandRangeFromZero(0.5f);  // 길이 7cm
    m_iMeatCnt = 1;  // 고기 1개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_ClownFish";
    m_iSushiMoney = 15;  // 스시 가격 6
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

_int CClownfish::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

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
