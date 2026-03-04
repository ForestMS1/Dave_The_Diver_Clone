#include "pch.h"
#include "CSmallspottedDart.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetSpine.h"

using namespace Fish;

CSmallspottedDart::CSmallspottedDart(float fPosX, float fPosY, float fScale)
    : CFishGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_fScale(fScale)
{
}

CSmallspottedDart::~CSmallspottedDart()
{
}

HRESULT CSmallspottedDart::Ready_GameObject()
{
    m_sFishName = L"타원전갱이";
    m_sThumbNailAssetName = L"Tex_FishThumb_Smallspotted_dart";
    m_iRank = 1;
    m_iStar = 1;
    m_fWeight = 0.5f;
    m_iPrice = 10;
    m_fLength = 10.f;
    m_iMeatCnt = 1;
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_SmallspottedDart";
    m_iSushiMoney = 18;
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

_int CSmallspottedDart::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CSmallspottedDart::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CSmallspottedDart::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CSmallspottedDart::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_SmallspottedDart");
    return S_OK;
}


CSmallspottedDart* CSmallspottedDart::Create(float fPosX, float fPosY, float fScale)
{
    CSmallspottedDart* pFish = new CSmallspottedDart{ fPosX, fPosY, fScale };

    if (FAILED(pFish->Ready_GameObject()))
    {
        Safe_Release(pFish);
        MSG_BOX("pFish Create Failed");
        return nullptr;
    }

    return pFish;
}

void CSmallspottedDart::Free()
{
    CFishGameObject::Free();
}
