#include "CCamera.h"
#include "CPipeline.h"
#include "CGraphicDev.h"

CCamera::CCamera()
    : CGameObject()
{
}

CCamera::CCamera(const CCamera& rhs)
    : CGameObject(rhs)
{
}

CCamera::~CCamera()
{
}

HRESULT CCamera::Ready_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
    D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);

    // CPipeline::MakeViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
    // CPipeline::MakeProjMatrix(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);

    pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
    pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

    return S_OK;
}

_int CCamera::Update_GameObject(const _float& fTimeDelta)
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
    //CPipeline::MakeViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
    pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

    return 0;
}

void CCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CCamera::Free()
{
    CGameObject::Free();
}
