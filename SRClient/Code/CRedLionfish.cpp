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
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CRedLionfish::~CRedLionfish()
{
}

HRESULT CRedLionfish::Ready_GameObject()
{
    m_sFishName = L"쏠배감펭";
    m_sThumbNailAssetName = L"Tex_FishThumb_Red_Lionfish";
    m_iRank = 3;  // 랭크 3 (상위)
    m_iStar = 3;  // 별 3개
    m_fWeight = 1.8f;  // 1.8kg
    m_iPrice = 22;  // 가격 22
    m_fLength = 28.f;  // 길이 28cm
    m_iMeatCnt = 3;  // 고기 3개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_RedLionfish";
    m_iSushiMoney = 35;  // 스시 가격 35
    m_iSushiLv = 3;  // 스시 레벨 3

    m_iAttackPower = 3;

    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { m_fScale, m_fScale, m_fScale };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fSpeed = 1.f;

    m_pSpineCom->Set_AniState(L"swim");
    m_eFishType = Fish::FT_AGRESSIVE;
    m_sAttackSpineAniName = L"sprint";
    return S_OK;
}

_int CRedLionfish::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

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
