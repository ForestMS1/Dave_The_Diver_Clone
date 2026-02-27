#include "CCameraMgr.h"
#include "CCamera.h"
#include "CHelper.h"
#include "CGraphicDev.h"
#include "CFrustrum.h"
IMPLEMENT_SINGLETON(CCameraMgr)

CCamera* g_pSelectedCamera = nullptr;

CCameraMgr::CCameraMgr()
	: m_pCurCamera(nullptr)
	, m_pFrustrumCom(CFrustrum::Create())
{
}

CCameraMgr::~CCameraMgr()
{
	Free();
}

void CCameraMgr::Set_Camera(wstring_view svCameraTag, Engine::CCamera* pCamera)
{
	auto iter = find_if(m_mapCamera.begin(), m_mapCamera.end(), CTag_FinderSV(svCameraTag));
	// 이미 존재한다면 리턴
	if (iter != m_mapCamera.end())
		return;

	m_mapCamera.insert({ svCameraTag, pCamera });

	if (m_pCurCamera == nullptr)
	{
		Change_CurCamera(svCameraTag);
	}
}

void CCameraMgr::Update_Camera(const _float& fTimeDelta)
{
	if (m_pCurCamera == nullptr)
		return;

	m_pCurCamera->Update_GameObject(fTimeDelta);
}

void CCameraMgr::LateUpdate_Camera(const _float& fTimeDelta)
{
	if (m_pCurCamera == nullptr)
		return;

	m_pCurCamera->LateUpdate_GameObject(fTimeDelta);
}

void CCameraMgr::Render_Camera()
{
	//if (m_pCurCamera == nullptr)
	//	return;
	//
	//m_pCurCamera->Render_GameObject();
	Update_Gizmo();
}

void CCameraMgr::Change_CurCamera(wstring_view svCameraTag)
{
	auto iter = find_if(m_mapCamera.begin(), m_mapCamera.end(), CTag_FinderSV(svCameraTag));
	// 존재 하지 않는 카메라라면 리턴
	if (iter == m_mapCamera.end())
		return;

	if(m_pCurCamera != nullptr)
		m_pCurCamera->Set_Acitve(false);

	m_pCurCamera = iter->second;

	m_pCurCamera->Set_Acitve(true);
}

CCamera* CCameraMgr::Find_Camera(wstring_view svCameraTag)
{
	auto iter = find_if(m_mapCamera.begin(), m_mapCamera.end(), CTag_FinderSV(svCameraTag));
	if (iter == m_mapCamera.end())
		return nullptr;

	return iter->second;
}

void CCameraMgr::Update_Gizmo()
{
#ifdef _DEBUG
	ImGui::Begin("Camera Change list");
	for (auto& iter : m_mapCamera)
	{
		CCamera* pCamera = iter.second;
		string sName(iter.first.begin(), iter.first.end());
		if (ImGui::Selectable(sName.c_str(), m_pCurCamera == pCamera))
		{
			Change_CurCamera(iter.first);
		}
	}
	ImGui::End();

	ImGui::Begin("Operation");
	if (ImGui::RadioButton("Translate", m_CurrentGizmoOperation == ImGuizmo::TRANSLATE))
		m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", m_CurrentGizmoOperation == ImGuizmo::ROTATE))
		m_CurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", m_CurrentGizmoOperation == ImGuizmo::SCALE))
		m_CurrentGizmoOperation = ImGuizmo::SCALE;
	

	ImGuizmo::BeginFrame();


	if (g_pSelectedCamera != nullptr)
	{
		CCamera* pSelectedCam = dynamic_cast<CCamera*>(g_pSelectedCamera);
		if (pSelectedCam == nullptr) return;

		LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

		_matrix matView, matProj;
		pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

		_matrix matGizmoWorld;
		_matrix matSelectedView = pSelectedCam->Get_ViewMatrix();
		D3DXMatrixInverse(&matGizmoWorld, nullptr, &matSelectedView);

		ImGuizmo::Manipulate(
			(float*)&matView,
			(float*)&matProj,
			m_CurrentGizmoOperation,
			ImGuizmo::WORLD,
			(float*)&matGizmoWorld
		);

		if (ImGuizmo::IsUsing())
		{
			_vec3 vNewEye = { matGizmoWorld._41, matGizmoWorld._42, matGizmoWorld._43 };
			_vec3 vNewLook = { matGizmoWorld._31, matGizmoWorld._32, matGizmoWorld._33 };
			D3DXVec3Normalize(&vNewLook, &vNewLook);
			_float fDist = 10.f;
			_vec3 vNewAt = vNewEye + vNewLook * fDist;

			pSelectedCam->Set_Pos(vNewEye);
			pSelectedCam->Set_At(vNewAt);
		}
		g_pSelectedCamera->Update_MatView();
		m_pFrustrumCom->Render_Buffer(g_pSelectedCamera->Get_ViewMatrix(), g_pSelectedCamera->Get_ProjMatrix());
	}

	ImGui::Begin("Camera Hierarchy");
	for (auto& LayerIter : m_mapCamera)
	{
		CCamera* pCamera = LayerIter.second;
		if (m_pCurCamera == pCamera)
			continue;
		string sName(LayerIter.first.begin(), LayerIter.first.end());
		if (ImGui::Selectable(sName.c_str(), g_pSelectedCamera == pCamera))
		{
			g_pSelectedCamera = pCamera;
		}
	}
	ImGui::End();
	ImGui::End();
#endif
}

void CCameraMgr::Free()
{
	Safe_Release(m_pFrustrumCom);
	for_each(m_mapCamera.begin(), m_mapCamera.end(), CDeleteMap());
	m_mapCamera.clear();
}

void CCameraMgr::Set_Ortho()
{
	if (m_pCurCamera == nullptr)
		return;
	m_pCurCamera->Set_Ortho();
}
void CCameraMgr::Set_Perspective()
{
	if (m_pCurCamera == nullptr)
		return;
	m_pCurCamera->Set_Perspective();
}