#include "pch.h"
#include "CBlobfish.h"
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

CBlobfish::CBlobfish(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CBlobfish::~CBlobfish()
{
}

HRESULT CBlobfish::Ready_GameObject()
{
    m_sFishName = L"블러브피쉬";
    m_sThumbNailAssetName = L"Tex_FishThumb_Blobfish";
    m_iRank = 3;  // 랭크 3 (상위)
    m_iStar = 3;  // 별 3개
    m_fWeight = 1.2f;  // 1.2kg
    m_iPrice = 15;  // 가격 15
    m_fLength = 20.f;  // 길이 20cm
    m_iMeatCnt = 2;  // 고기 2개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_Blobfish";
    m_iSushiMoney = 18;  // 스시 가격 18
    m_iSushiLv = 2;  // 스시 레벨 2


    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { m_fScale, m_fScale, m_fScale };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fSpeed = 1.f;

    m_pSpineCom->Set_AniState(L"swim");

    return S_OK;
}

_int CBlobfish::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CBlobfish::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CBlobfish::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CBlobfish::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_Blobfish");
    return S_OK;
}


CBlobfish* CBlobfish::Create(float fPosX, float fPosY, float fScale)
{
    CBlobfish* pFish = new CBlobfish{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CBlobfish::Free()
{
    CFishGameObject::Free();
}
