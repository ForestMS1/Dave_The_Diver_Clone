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

using namespace Fish;

CBlueTang::CBlueTang(float fPosX, float fPosY)
    : CFishGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CBlueTang::~CBlueTang()
{
}

HRESULT CBlueTang::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 0.1, 0.1f, 0.1f };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fViewZ = 0.5f;

    return S_OK;
}

_int CBlueTang::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

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
    CFishGameObject::Ready_Component(L"Spine_Fish_BlueTang");
    return S_OK;
}


CBlueTang* CBlueTang::Create(float fPosX, float fPosY)
{
    CBlueTang* pBlueTang = new CBlueTang{fPosX, fPosY};

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
