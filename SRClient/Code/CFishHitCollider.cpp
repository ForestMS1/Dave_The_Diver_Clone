#include "CFishHitCollider.h"
#include "CColliderMgr.h"

CFishHitCollider::CFishHitCollider(_vec3* vPos, _vec3* vScale)
    : CGameObject()
    , m_vPos(*vPos)
    , m_vScale(*vScale)
    , m_pAABB(nullptr)
{
}

CFishHitCollider::~CFishHitCollider()
{
}

_int		CFishHitCollider::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = CGameObject::Update_GameObject(fTimeDelta);

    //_vec3 vParentPos;
    //m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vParentPos);

    _vec3 vParentPos;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vParentPos);

    _vec3 vParentAngle;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Rotation(&vParentAngle);

    _vec3 vParentScale;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Scale(&vParentScale);


    m_pTransformCom->Set_Rotation(&vParentAngle);

    float fOffsetX = m_vPos.x;
    float fOffsetY = m_vPos.y;
    vParentPos.x += fOffsetX;
    vParentPos.y += fOffsetY;

    m_pTransformCom->Set_Pos(vParentPos.x, vParentPos.y, vParentPos.z);
    //_vec3 vScale = { m_fFirstScaleX * m_fCustomScaleX, m_fFirstScaleY * m_fCustomScaleY, 0.f };

    //vParentScale.x *= m_vScale.x;
    //vParentScale.y *= m_vScale.y;
    //m_pTransformCom->Set_Scale(&vParentScale);


    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_FISH_HIT", m_pAABB);
    m_pAABB->Transform(m_pTransformCom->Get_World());
    return iExit;
}

void		CFishHitCollider::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

CFishHitCollider* CFishHitCollider::Create(_vec3* vPos, _vec3* vScale)
{
    CFishHitCollider* pBlueTang = new CFishHitCollider{ vPos, vScale };

    if (FAILED(pBlueTang->Ready_GameObject()))
    {
        Safe_Release(pBlueTang);
        MSG_BOX("pBlueTang Create Failed");
        return nullptr;
    }

    return pBlueTang;
}

void CFishHitCollider::Free()
{
	CGameObject::Free();
	Safe_Release(m_pAABB);
}

HRESULT CFishHitCollider::Ready_Component()
{    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
    return S_OK;
}

HRESULT CFishHitCollider::Ready_GameObject()
{
    if(FAILED(Ready_Component()))
        return E_FAIL;

    m_pAABB = CAABB::Create(&m_vPos, &m_vScale, L"ASDf", this);


    return S_OK;
}