#include "CFishAABBCollider.h"
#include "CColliderMgr.h"

CFishAABBCollider::CFishAABBCollider(CAABB* pAABB, std::wstring_view svCollGroup)
    : CGameObject()
    , m_pAABB(pAABB)
    , m_sCollGroup(svCollGroup)
    , m_bApplyParentAngle(true)
    , m_vPosOffset({ 0.f, 0.f, 0.f })
{
}

CFishAABBCollider::~CFishAABBCollider()
{
}

_int		CFishAABBCollider::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = CGameObject::Update_GameObject(fTimeDelta);

    //_vec3 vParentPos;
    //m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vParentPos);

    _vec3 vParentPos;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vParentPos);

    if (m_bApplyParentAngle)
    {
        _vec3 vParentAngle;
        m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Rotation(&vParentAngle);
        m_pTransformCom->Set_Rotation(&vParentAngle);
    }

    _vec3 vParentScale;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Scale(&vParentScale);

    //float fOffsetX = m_vPos.x;
    //float fOffsetY = m_vPos.y;
    //vParentPos.x += fOffsetX;
    //vParentPos.y += fOffsetY;
    vParentPos += m_vPosOffset;

    m_pTransformCom->Set_Pos(vParentPos.x, vParentPos.y, vParentPos.z);
    //_vec3 vScale = { m_fFirstScaleX * m_fCustomScaleX, m_fFirstScaleY * m_fCustomScaleY, 0.f };

    //vParentScale.x *= m_vScale.x;
    //vParentScale.y *= m_vScale.y;
    //m_pTransformCom->Set_Scale(&vParentScale);


    CColliderMgr::GetInstance()->AddColliderGroup(m_sCollGroup, m_pAABB);
    m_pAABB->Transform(m_pTransformCom->Get_World());
    return iExit;
}

void		CFishAABBCollider::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

CFishAABBCollider* CFishAABBCollider::Create(CAABB* pAABB, std::wstring_view svCollGroup)
{
    CFishAABBCollider* pBlueTang = new CFishAABBCollider{ pAABB, svCollGroup };

    if (FAILED(pBlueTang->Ready_GameObject()))
    {
        Safe_Release(pBlueTang);
        MSG_BOX("pBlueTang Create Failed");
        return nullptr;
    }

    return pBlueTang;
}

void CFishAABBCollider::Free()
{
	CGameObject::Free();
	Safe_Release(m_pAABB);
}

HRESULT CFishAABBCollider::Ready_Component()
{    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
    return S_OK;
}

HRESULT CFishAABBCollider::Ready_GameObject()
{
    if(FAILED(Ready_Component()))
        return E_FAIL;

    return S_OK;
}