#include "CDiveDaveCam.h"
#include "CGraphicDev.h"
CDiveDaveCam::CDiveDaveCam()
	: m_fLerpSpeed(0.f)
	, m_pCalculatorCom(nullptr)
{
}

CDiveDaveCam::CDiveDaveCam(const CDiveDaveCam& rhs)
	: CCamera(rhs), m_fLerpSpeed(rhs.m_fLerpSpeed)
{
}

CDiveDaveCam::~CDiveDaveCam()
{
}
HRESULT		CDiveDaveCam::Ready_GameObject(const _vec3* pEye,
	const _vec3* pAt,
	const _vec3* pUp,
	const _float& fFov,
	const _float& fAspect,
	const _float& fNear,
	const _float& fFar)
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
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fLerpSpeed = 3.f;

	return S_OK;
}

_int CDiveDaveCam::Update_GameObject(const _float& fTimeDelta)
{
	//_int iExit = CCamera::Update_GameObject(fTimeDelta);
	if (!m_pTargetPos)
		return 0;

	// 카메라 쉐이킹
	FightShakingUpdate(fTimeDelta);

	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	_vec3 vDesiredAt = *m_pTargetPos;
	_vec3 vDesiredEye = vDesiredAt + m_vOffset;

	m_vEye += (vDesiredEye - m_vEye) * m_fLerpSpeed * fTimeDelta;
	m_vAt += (vDesiredAt - m_vAt) * m_fLerpSpeed * fTimeDelta;

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return 0;
}

void CDiveDaveCam::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CCamera::LateUpdate_GameObject(fTimeDelta);
}

void CDiveDaveCam::Render_GameObject()
{
}

void CDiveDaveCam::FightShakingStart(const _float& fDuration)
{
	m_vOriginEye = m_vEye;
	m_vOriginAt = m_vAt;
	m_fAccShakingDuration = fDuration;
	m_bShaked = true;
	SetFov(45.f);
}

void CDiveDaveCam::FightShakingUpdate(const _float& fTimeDelta)
{
	if (m_bShaked == true && m_fAccShakingDuration > 0.f)
	{
		m_fAccShakingDuration -= fTimeDelta;

		_vec3 vShake = { 1.f, 1.f, - 10.f };
		_float fIntensity = 0.002f;
		_float r = _float(rand() % 100) * fIntensity;
		vShake *= r;
		vShake.z = -10.f;

		m_vEye = *m_pTargetPos + vShake;
		m_vEye.z = -10.f;
		m_vAt = m_vEye + m_vOffset;
		m_vAt.z = -10.f;
	}
	else if(m_bShaked == true)
	{
		m_vEye = m_vOriginEye;
		m_vAt = m_vOriginAt;
		m_bShaked = false;
	}
}

void CDiveDaveCam::FightShakingEnd()
{
	m_bShaked = false;
	//SetFov(60.f);
}

HRESULT	CDiveDaveCam::Add_Component()
{
	CComponent* pComponent = nullptr;


	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Calculator"));
	if (pComponent == nullptr)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Calculator", pComponent });

	return S_OK;
}

CDiveDaveCam* CDiveDaveCam::Create(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CDiveDaveCam* pCam = new CDiveDaveCam;
	if (FAILED(pCam->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCam);
		MSG_BOX("DiveDaveCam Create Failed");
		return nullptr;
	}
	return pCam;
}

void CDiveDaveCam::Free()
{
	CCamera::Free();
}
