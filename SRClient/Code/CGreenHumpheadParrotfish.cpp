#include "pch.h"
#include "CGreenHumpheadParrotfish.h"
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

CGreenHumpheadParrotfish::CGreenHumpheadParrotfish(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CGreenHumpheadParrotfish::~CGreenHumpheadParrotfish()
{
}

HRESULT CGreenHumpheadParrotfish::Ready_GameObject()
{
    m_sFishName = L"버팔로패럿피쉬";
    m_sThumbNailAssetName = L"Tex_FishThumb_Green_Humphead_Parrotfish";
    m_iRank = 3;  // 랭크 3 (상위)
    m_iStar = 3;  // 별 3개
    m_fWeight = 1.5f + CHelper::RandRangeFromZero(5.f);  // 1.5kg
    m_iPrice = 18;  // 가격 18
    m_fLength = 28.f + CHelper::RandRangeFromZero(5.f);  // 길이 28cm
    m_iMeatCnt = 2;  // 고기 2개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_GreenHumpheadParrotfish";
    m_iSushiMoney = 25;  // 스시 가격 25
    m_iSushiLv = 2;  // 스시 레벨 2

    m_bNeedSlice = true;
    m_iHP = 5;

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

_int CGreenHumpheadParrotfish::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CGreenHumpheadParrotfish::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CGreenHumpheadParrotfish::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CGreenHumpheadParrotfish::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_GreenHumpheadParrotfish");
    return S_OK;
}


CGreenHumpheadParrotfish* CGreenHumpheadParrotfish::Create(float fPosX, float fPosY, float fScale)
{
    CGreenHumpheadParrotfish* pFish = new CGreenHumpheadParrotfish{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CGreenHumpheadParrotfish::Free()
{
    CFishGameObject::Free();
}
