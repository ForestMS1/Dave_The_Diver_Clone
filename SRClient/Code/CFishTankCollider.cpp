#include "pch.h"
#include "CFishTankCollider.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CColliderMgr.h"

CFishTankCollider::CFishTankCollider(wstring_view svColliderTag, wstring_view svColliderGroupTag)
    : CGameObject()
    //, m_fPosX(fPosX)
    //, m_fPosY(fPosY)
    , m_pTransformCom(nullptr)
    , m_pAABB(nullptr)
    , m_sColliderTag(svColliderTag)
    , m_sColliderGroupTag(svColliderGroupTag)
    , m_funcOnEnter(nullptr)
    , m_funcOnExit(nullptr)
    , m_funcOnStay(nullptr)
{
}

CFishTankCollider::~CFishTankCollider()
{
}


HRESULT		CFishTankCollider::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vPos = { 0.0f, 0.0f, 0.0f };
    _vec3 vExtents = { 1.0f, 1.0f, 0.001f };
    //m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_pAABB = CAABB::Create(&vPos, &vExtents, m_sColliderTag, this);
    m_pAABB->Set_OriginalColor(D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
    m_pAABB->Set_IntersectColor(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

    return S_OK;
}

_int		CFishTankCollider::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    CColliderMgr::GetInstance()->AddColliderGroup(m_sColliderGroupTag, m_pAABB);
    m_pAABB->Transform(m_pTransformCom->Get_World());

    return iExit;
}

void		CFishTankCollider::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CFishTankCollider::Render_GameObject()
{

}

HRESULT			CFishTankCollider::Ready_Component()
{
    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CFishTankCollider* CFishTankCollider::Create(wstring_view svColliderTag, wstring_view svColliderGroupTag)
{
    CFishTankCollider* pFishTankCollider = new CFishTankCollider{ svColliderTag , svColliderGroupTag };

    if (FAILED(pFishTankCollider->Ready_GameObject()))
    {
        Safe_Release(pFishTankCollider);
        MSG_BOX("pFishTankCollider Create Failed");
        return nullptr;
    }

    return pFishTankCollider;
}

void CFishTankCollider::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}