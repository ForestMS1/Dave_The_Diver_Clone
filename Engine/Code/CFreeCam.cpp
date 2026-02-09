#include "CFreeCam.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CProtoMgr.h"
#include "CManagement.h"
#include "CInfoMgr.h"

_uint CFreeCam::g_iNum = 0;

CFreeCam::CFreeCam()
	: m_fSpeed(0.f)
	, m_pCalculatorCom(nullptr)
	, m_iNum(g_iNum)
{
}

CFreeCam::CFreeCam(const CFreeCam& rhs)
	: CCamera(rhs), m_fSpeed(rhs.m_fSpeed)
{
	CFreeCam::m_iNum++;
}

CFreeCam::~CFreeCam()
{
}

HRESULT CFreeCam::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
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

_int CFreeCam::Update_GameObject(const _float& fTimeDelta)
{
	// ImGUI 출력 하기위해 렌더러에 집어넣음.
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	Key_Input(fTimeDelta);
	_int iExit = CCamera::Update_GameObject(fTimeDelta);

	return iExit;
}

void CFreeCam::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CCamera::LateUpdate_GameObject(fTimeDelta);
	//Mouse_Fix();
}

void CFreeCam::Render_GameObject()
{
#ifdef _DEBUG
	ImGui::Begin(to_string(m_iNum).c_str());
	ImGui::Text("FreeCam : %d", m_iNum);
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

void CFreeCam::Key_Input(const _float& fTimeDelta)
{
	_matrix matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	if (GetAsyncKeyState('D') & 0x8000)
	{
		_vec3       vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * m_fSpeed;

		m_vEye += vLength;
		m_vAt += vLength;

	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		_vec3       vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * m_fSpeed;

		m_vEye -= vLength;
		m_vAt -= vLength;

	}

	if (GetAsyncKeyState('W') & 0x8000)
	{
		_vec3       vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * m_fSpeed;

		m_vEye += vLength;
		m_vAt += vLength;

	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		_vec3       vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * m_fSpeed;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (CDInputMgr::GetInstance()->Key_Pressing(DIK_Q))
	{
		_vec3       vUp(0.f, 1.f, 0.f);

		_vec3 vLength = vUp * fTimeDelta * m_fSpeed;

		m_vEye += vLength;
		m_vAt += vLength;
	}
	if (CDInputMgr::GetInstance()->Key_Pressing(DIK_E))
	{
		_vec3       vUp(0.f, 1.f, 0.f);

		_vec3 vLength = vUp * fTimeDelta * m_fSpeed;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (CDInputMgr::GetInstance()->Mouse_Pressing(DIM_RB))
	{
		Mouse_Move();
	}
}

void CFreeCam::Mouse_Move()
{
	_matrix     matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	_long   dwMouseMove(0);

	if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_Y))
	{
		_vec3       vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3       vLook = m_vAt - m_vEye;

		_matrix     matRot;

		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_X))
	{
		_vec3       vUp{ 0.f, 1.f,0.f };

		_vec3       vLook = m_vAt - m_vEye;

		_matrix     matRot;

		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}
}

void CFreeCam::Mouse_Fix()
{

	POINT       ptMouse{ WINCX >> 1, WINCY >> 1 };



	ClientToScreen(CInfoMgr::GetInstance()->Get_HWND(), &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
	ShowCursor(false);
}

HRESULT CFreeCam::Add_Component()
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

CFreeCam* CFreeCam::Create(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CFreeCam* pCam = new CFreeCam;
	if (FAILED(pCam->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCam);
		MSG_BOX("FreeCam Create Failed");
		return nullptr;
	}
	g_iNum++;
	return pCam;
}

void CFreeCam::Free()
{
	CCamera::Free();
}
