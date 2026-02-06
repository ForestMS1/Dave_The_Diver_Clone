#include "CPlayerCam.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CProtoMgr.h"
#include "CManagement.h"
#include "CInfoMgr.h"
//#include "CPlayer.h"

CPlayerCam::CPlayerCam()
	: m_fSpeed(0.f)
	, m_pCalculatorCom(nullptr)
{
}

CPlayerCam::CPlayerCam(const CPlayerCam& rhs)
	: CCamera(rhs), m_fSpeed(rhs.m_fSpeed)
{
}

CPlayerCam::~CPlayerCam()
{
}

HRESULT CPlayerCam::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
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

	m_fSpeed = 10.f;

	return S_OK;
}

_int CPlayerCam::Update_GameObject(const _float& fTimeDelta)
{
	// ImGUI 출력 하기위해 렌더러에 집어넣음.
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	_int iExit = CCamera::Update_GameObject(fTimeDelta);

	return iExit;
}

void CPlayerCam::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CCamera::LateUpdate_GameObject(fTimeDelta);
	//Mouse_Fix();
}

void CPlayerCam::Render_GameObject()
{
#ifdef _DEBUG
	ImGui::Begin("PlayerCam");
	ImGui::DragFloat3("Eye", (float*)&m_vEye, 1.f - 1.f, 1.f);
	ImGui::DragFloat3("At", (float*)&m_vAt, 1.f - 1.f, 1.f);
	if (ImGui::Button("Reset_Pos"))
	{
		m_vEye = { 0.f, 10.f, 0.f };
		m_vAt = { 0.f, 0.f, 1.f };
		m_vUp = { 0.f, 1.f, 0.f };
	}
	ImGui::End();
#endif
}

void CPlayerCam::Mouse_Fix()
{
	
	POINT       ptMouse{ WINCX >> 1, WINCY >> 1 };



	ClientToScreen(CInfoMgr::GetInstance()->Get_HWND(), &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
	ShowCursor(false);
}

HRESULT CPlayerCam::Add_Component()
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

CPlayerCam* CPlayerCam::Create(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CPlayerCam* pPlayerCam = new CPlayerCam;
	if (FAILED(pPlayerCam->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pPlayerCam);
		MSG_BOX("PlayerCam Create Failed");
		return nullptr;
	}
	return pPlayerCam;
}

void CPlayerCam::Free()
{
	CCamera::Free();
}
