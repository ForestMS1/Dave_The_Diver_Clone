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
    : CFishGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_fScale(fScale)
{
}

CGreenHumpheadParrotfish::~CGreenHumpheadParrotfish()
{
}

HRESULT CGreenHumpheadParrotfish::Ready_GameObject()
{
    m_sFishName = L"¹öÆÈ·ÎÆÐ·µÇÇ½¬";
    m_sThumbNailAssetName = L"Tex_FishThumb_Green_Humphead_Parrotfish";
    m_iRank = 1;
    m_iStar = 1;
    m_fWeight = 0.5f;
    m_iPrice = 10;
    m_fLength = 10.f;
    m_iMeatCnt = 1;
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_GreenHumpheadParrotfish";
    m_iSushiMoney = 12;
    m_iSushiLv = 1;

    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { m_fScale, m_fScale, m_fScale };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fViewZ = 0.5f;

    m_fSpeed = 1.f;

    m_pSpineCom->Set_AniState(L"swim");

    m_bNeedSlice = true;

    return S_OK;
}

_int CGreenHumpheadParrotfish::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

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
