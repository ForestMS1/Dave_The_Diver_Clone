#include "pch.h"
#include "CGreyTriggerfish.h"
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

CGreyTriggerfish::CGreyTriggerfish(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CGreyTriggerfish::~CGreyTriggerfish()
{
}

HRESULT CGreyTriggerfish::Ready_GameObject()
{
    m_sFishName = L"회색쥐치";
    m_sThumbNailAssetName = L"Tex_FishThumb_Grey_Triggerfish";
    m_iRank = 2;  // 랭크 2 (중간)
    m_iStar = 2;  // 별 2개
    m_fWeight = 0.8f;  // 0.8kg
    m_iPrice = 12;  // 가격 12
    m_fLength = 18.f;  // 길이 18cm
    m_iMeatCnt = 2;  // 고기 2개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_GreyTriggerfish";
    m_iSushiMoney = 18;  // 스시 가격 18
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

_int CGreyTriggerfish::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CGreyTriggerfish::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CGreyTriggerfish::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CGreyTriggerfish::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_GreyTriggerfish");
    return S_OK;
}


CGreyTriggerfish* CGreyTriggerfish::Create(float fPosX, float fPosY, float fScale)
{
    CGreyTriggerfish* pFish = new CGreyTriggerfish{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CGreyTriggerfish::Free()
{
    CFishGameObject::Free();
}
