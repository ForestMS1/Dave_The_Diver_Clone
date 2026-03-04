#include "pch.h"
#include "CHumboldtSquid.h"
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

CHumboldtSquid::CHumboldtSquid(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CHumboldtSquid::~CHumboldtSquid()
{
}

HRESULT CHumboldtSquid::Ready_GameObject()
{

    m_sFishName = L"ÈÉº¼Æ® ¿ÀÂ¡¾î";
    m_sThumbNailAssetName = L"Tex_FishThumb_Humboldt_Squid";
    m_iRank = 1;
    m_iStar = 1;
    m_fWeight = 0.5f;
    m_iPrice = 10;
    m_fLength = 10.f;
    m_iMeatCnt = 1;
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_Humboldt_Squid";
    m_iSushiMoney = 12;
    m_iSushiLv = 1;

    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { m_fScale, m_fScale, m_fScale };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fSpeed = 1.f;

    m_pSpineCom->Set_AniState(L"swim");

    return S_OK;
}

_int CHumboldtSquid::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CHumboldtSquid::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CHumboldtSquid::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CHumboldtSquid::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_HumboldtSquid");
    return S_OK;
}


CHumboldtSquid* CHumboldtSquid::Create(float fPosX, float fPosY, float fScale)
{
    CHumboldtSquid* pFish = new CHumboldtSquid{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CHumboldtSquid::Free()
{
    CFishGameObject::Free();
}
