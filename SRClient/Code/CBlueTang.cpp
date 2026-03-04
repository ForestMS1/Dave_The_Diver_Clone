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
    m_sFishName = L"ºí·çÅÊ";
    m_sThumbNailAssetName = L"Tex_FishThumb_Bluetang";
    m_iRank = 1;
    m_iStar = 1;
    m_fWeight = 0.5f;
    m_iPrice = 10;
    m_fLength = 10.f;
    m_iMeatCnt = 1;
    m_sSushiThumbNailAssetName = L"Tex_SushiThumb_Bluetang";
    m_iSushiMoney = 11;
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
