#include "pch.h"
#include "CChamberedNautilus.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetSpine.h"

using namespace Fish;

CChamberedNautilus::CChamberedNautilus(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CChamberedNautilus::~CChamberedNautilus()
{
}

HRESULT CChamberedNautilus::Ready_GameObject()
{
    m_sFishName = L"앵무조개";
    m_sThumbNailAssetName = L"Tex_FishThumb_ChamberedNautilus";
    m_iRank = 2;  // 랭크 2 (중간)
    m_iStar = 2;  // 별 2개
    m_fWeight = 0.6f + CHelper::RandRangeFromZero(5.f);  // 0.6kg
    m_iPrice = 12;  // 가격 12
    m_fLength = 15.f + CHelper::RandRangeFromZero(5.f);  // 길이 15cm
    m_iMeatCnt = 2;  // 고기 2개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_ChamberedNautilus";
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

_int CChamberedNautilus::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CChamberedNautilus::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CChamberedNautilus::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CChamberedNautilus::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_ChamberedNautilus");
    return S_OK;
}


CChamberedNautilus* CChamberedNautilus::Create(float fPosX, float fPosY, float fScale)
{
    CChamberedNautilus* pFish = new CChamberedNautilus{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CChamberedNautilus::Free()
{
    CFishGameObject::Free();
}
