#include "CCamera.h"
#include "CPipeline.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
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
    m_bSwitch = false;

    
    


    return S_OK;
}

_int CCamera::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bSwitch) {
        D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
        //CPipeline::MakeViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
        m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
    }
    return 0;
}

void CCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CCamera::Reset_Camera()
{
    D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
    D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);

    // CPipeline::MakeViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
    // CPipeline::MakeProjMatrix(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);

    m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
    m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
    
    m_bSwitch = true;
}

void CCamera::Set_SwitchCamera(_bool bSwitch)
{
    bSwitch = m_bSwitch;
}



void CCamera::Free()
{
    CGameObject::Free();
}
