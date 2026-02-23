#include "pch.h"
#include "CSushiCamera.h"
#include "CDInputMgr.h"
#include "CInfoMgr.h"
#include "CParticleMgr.h"
#include "CGraphicDev.h"

CSushiCamera::CSushiCamera()
    : CCamera(), m_bFix(true), m_bCheck(true)
{
}

CSushiCamera::CSushiCamera(const CSushiCamera& rhs)
    : CCamera(rhs), m_bFix(true), m_bCheck(true)
{
}

CSushiCamera::~CSushiCamera()
{
}

HRESULT CSushiCamera::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
    m_vEye = *pEye;
    m_vAt = *pAt;
    m_vUp = *pUp;

    m_fFov = fFov;
    m_fAspect = fAspect;
    m_fNear = fNear;
    m_fFar = fFar;

    if (FAILED(CCamera::Ready_GameObject()))
        return E_FAIL;

    m_fSpeed = 10.f;

    return S_OK;
}

_int CSushiCamera::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CCamera::Update_GameObject(fTimeDelta);

    return iExit;
}

void CSushiCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CCamera::LateUpdate_GameObject(fTimeDelta);

}


CSushiCamera* CSushiCamera::Create(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
    CSushiCamera* pCamera = new CSushiCamera;


    if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
    {
        Safe_Release(pCamera);
        MSG_BOX("DynamicCamera Create Failed");
        return nullptr;
    }

    return pCamera;
}

void CSushiCamera::Free()
{
    CCamera::Free();
}
