#include "pch.h"
#include "CAtlanticBonito.h"
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

CAtlanticBonito::CAtlanticBonito(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CAtlanticBonito::~CAtlanticBonito()
{
}

HRESULT CAtlanticBonito::Ready_GameObject()
{
    m_sFishName = L"타이탄쥐치";
    m_sThumbNailAssetName = L"Tex_FishThumb_Titan_Triggerfish";
    m_iRank = 4;  // 랭크 4 (최상위)
    m_iStar = 3;  // 별 4개
    m_fWeight = 2.5f;  // 2.5kg
    m_iPrice = 28;  // 가격 28
    m_fLength = 40.f;  // 길이 40cm
    m_iMeatCnt = 4;  // 고기 4개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_TitanTriggerfish";
    m_iSushiMoney = 45;  // 스시 가격 45
    m_iSushiLv = 3;  // 스시 레벨 3

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

_int CAtlanticBonito::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CAtlanticBonito::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CAtlanticBonito::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CAtlanticBonito::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_AtlanticBonito");
    return S_OK;
}


CAtlanticBonito* CAtlanticBonito::Create(float fPosX, float fPosY, float fScale)
{
    CAtlanticBonito* pFish = new CAtlanticBonito{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CAtlanticBonito::Free()
{
    CFishGameObject::Free();
}
