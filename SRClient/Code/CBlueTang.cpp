#include "pch.h"
#include "CBlueTang.h"
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

CBlueTang::CBlueTang(float fPosX, float fPosY, float fScale)
    : CFishGameObject(fPosX, fPosY, fScale)
{
}

CBlueTang::~CBlueTang()
{
}

HRESULT CBlueTang::Ready_GameObject()
{
    

    m_sFishName = L"블루탱";
    m_sThumbNailAssetName = L"Tex_FishThumb_Bluetang";
    m_iRank = 1;  // 랭크 1 (하위)
    m_iStar = 1;  // 별 1개
    m_fWeight = 0.2f + CHelper::RandRangeFromZero(5.f);  // 0.2kg
    m_iPrice = 5;  // 가격 5
    m_fLength = 8.f + CHelper::RandRangeFromZero(5.f);  // 길이 8cm
    m_iMeatCnt = 1;  // 고기 1개
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_Bluetang";
    m_iSushiMoney = 11;  // 스시 가격 8
    m_iSushiLv = 1;  // 스시 레벨 1

    m_iHP = 1;


    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { m_fScale, m_fScale, m_fScale };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fSpeed = 1.f;

    m_pSpineCom->Set_AniState(L"swim");

    return S_OK;
}

_int CBlueTang::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CBlueTang::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CBlueTang::Render_GameObject()
{
    CFishGameObject::Render();
}

HRESULT CBlueTang::Ready_Component()
{
    CFishGameObject::Ready(L"Spine_Fish_BlueTang");
    return S_OK;
}


CBlueTang* CBlueTang::Create(float fPosX, float fPosY, float fScale)
{
    CBlueTang* pBlueTang = new CBlueTang{fPosX, fPosY, fScale };

    if (FAILED(pBlueTang->Ready_GameObject()))
    {
        Safe_Release(pBlueTang);
        MSG_BOX("pBlueTang Create Failed");
        return nullptr;
    }

    return pBlueTang;
}

void CBlueTang::Free()
{
    CFishGameObject::Free();
}
