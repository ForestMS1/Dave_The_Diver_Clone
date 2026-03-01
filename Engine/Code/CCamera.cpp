#include "CCamera.h"
#include "CPipeline.h"
#include "CGraphicDev.h"

CCamera::CCamera()
    : CGameObject()
    , m_bIsActive(true)
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
    D3DXMatrixOrthoLH(&m_matOrthoProj, (_float)WINCX, (_float)WINCY, 0.f, m_fFar);
    //D3DXMatrixOrthoOffCenterLH(&m_matOrthoProj, 0.f, (_float)WINCX, (_float)WINCY, 0.f, m_fNear, m_fFar);

    // CPipeline::MakeViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
    // CPipeline::MakeProjMatrix(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);

    pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
    pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

    return S_OK;
}

_int CCamera::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bIsActive)
        return 0;

    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
    //CPipeline::MakeViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
    pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

    return 0;
}

void CCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (!m_bIsActive)
        return;
}

void CCamera::Free()
{
    CGameObject::Free();
}

void CCamera::Set_Ortho()
{
    if (!m_bIsActive)
        return;
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    _matrix matViewIdentity;
    D3DXMatrixIdentity(&matViewIdentity);
    
    pGraphicDev->SetTransform(D3DTS_VIEW, &matViewIdentity);
    pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matOrthoProj);
}
void CCamera::Set_Perspective()
{
    if (!m_bIsActive)
        return;


    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);   // ← 이거 필수
    pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);


    //pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}