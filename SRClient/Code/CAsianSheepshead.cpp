#include "pch.h"
#include "CAsianSheepshead.h"
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

CAsianSheepshead::CAsianSheepshead(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CAsianSheepshead::~CAsianSheepshead()
{
}

HRESULT CAsianSheepshead::Ready_GameObject()
{
    
    m_sFishName = L"혹돔";
    m_sThumbNailAssetName = L"Tex_FishThumb_Asian_Sheepshead";
    m_iRank = 2;  // 중간 랭크
    m_iStar = 2;  // 두 개 별
    m_fWeight = 0.7f + CHelper::RandRangeFromZero(0.5f);  // 0.5kg
    m_iPrice = 10;  // 가격 10
    m_fLength = 10.f + CHelper::RandRangeFromZero(0.5f);  // 길이 10cm
    m_iMeatCnt = 1;  // 고기 1개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_Asian_Sheepshead";
    m_iSushiMoney = 12;
    m_iSushiLv = 1;

    m_iHP = 4;

    //m_sRunFromSpineAniName = L"sprint";

    m_iAttackPower = 7;
    m_eFishType = Fish::FT_AGRESSIVE;
    m_sAttackSpineAniName = L"sprint";

    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { m_fScale, m_fScale, m_fScale };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fSpeed = 1.f;

    m_pSpineCom->Set_AniState(L"swim");

    return S_OK;
}

_int CAsianSheepshead::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);


    return iExit;
}

void CAsianSheepshead::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CAsianSheepshead::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CAsianSheepshead::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_AsianSheepshead");
    return S_OK;
}


CAsianSheepshead* CAsianSheepshead::Create(float fPosX, float fPosY, float fScale)
{
    CAsianSheepshead* pFish = new CAsianSheepshead{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CAsianSheepshead::Free()
{
    CFishGameObject::Free();
}
