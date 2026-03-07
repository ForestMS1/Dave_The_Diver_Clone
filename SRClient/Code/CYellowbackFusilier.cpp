#include "pch.h"
#include "CYellowbackFusilier.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetSpine.h"

using namespace Fish;

CYellowbackFusilier::CYellowbackFusilier(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CYellowbackFusilier::~CYellowbackFusilier()
{
}

HRESULT CYellowbackFusilier::Ready_GameObject()
{
    m_sFishName = L"노랑등이 나비돔";
    m_sThumbNailAssetName = L"Tex_FishThumb_Yellowback_Fusilier";
    m_iRank = 2;  // 랭크 2 (중간)
    m_iStar = 2;  // 별 2개
    m_fWeight = 0.7f;  // 0.7kg
    m_iPrice = 14;  // 가격 14
    m_fLength = 22.f;  // 길이 22cm
    m_iMeatCnt = 2;  // 고기 2개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_YellowbackFusilier";
    m_iSushiMoney = 20;  // 스시 가격 20
    m_iSushiLv = 2;  // 스시 레벨 2

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

_int CYellowbackFusilier::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CYellowbackFusilier::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CYellowbackFusilier::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CYellowbackFusilier::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_YellowbackFusilier");
    return S_OK;
}


CYellowbackFusilier* CYellowbackFusilier::Create(float fPosX, float fPosY, float fScale)
{
    CYellowbackFusilier* pFish = new CYellowbackFusilier{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CYellowbackFusilier::Free()
{
    CFishGameObject::Free();
}
