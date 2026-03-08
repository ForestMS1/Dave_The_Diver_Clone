#include "pch.h"
#include "CYellowTang.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetSpine.h"

using namespace Fish;

CYellowTang::CYellowTang(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CYellowTang::~CYellowTang()
{
}

HRESULT CYellowTang::Ready_GameObject()
{
    m_sFishName = L"옐로우탱";
    m_sThumbNailAssetName = L"Tex_FishThumb_Yellow_Tang";
    m_iRank = 1;  // 랭크 1 (하위)
    m_iStar = 1;  // 별 1개
    m_fWeight = 0.3f + CHelper::RandRangeFromZero(5.f);  // 0.3kg
    m_iPrice = 8;  // 가격 8
    m_fLength = 10.f + CHelper::RandRangeFromZero(5.f);  // 길이 10cm
    m_iMeatCnt = 1;  // 고기 1개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_YellowTang";
    m_iSushiMoney = 13;  // 스시 가격 12
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

_int CYellowTang::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CYellowTang::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CYellowTang::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CYellowTang::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_YellowTang");
    return S_OK;
}


CYellowTang* CYellowTang::Create(float fPosX, float fPosY, float fScale)
{
    CYellowTang* pFish = new CYellowTang{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CYellowTang::Free()
{
    CFishGameObject::Free();
}
