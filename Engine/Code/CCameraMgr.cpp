#include "CCameraMgr.h"
#include "CCamera.h"
#include "CHelper.h"
#include "CGraphicDev.h"
IMPLEMENT_SINGLETON(CCameraMgr)

CCamera* g_pSelectedCamera = nullptr;

CCameraMgr::CCameraMgr()
	: m_pCurCamera(nullptr)
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

#ifdef _DEBUG
	ImGui::Begin("Camera Change list");
	for (auto& iter : m_mapCamera)
	{
		CCamera* pCamera = iter.second;
		string cName = to_string((_int)iter.second); // 카메라 이름을 매길 방법을 못찾아.. 그냥 주소값으로 구분해라
		if (ImGui::Selectable(cName.c_str(), m_pCurCamera == pCamera))
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
	ImGui::End();

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

		// 카메라의 현재 위치를 기반으로 임시 Gizmo용 월드 행렬 생성
		_matrix matGizmoWorld;
		_vec3 vEye = pSelectedCam->Get_Pos();
		_vec3 vAt = pSelectedCam->Get_At();

		D3DXMatrixIdentity(&matGizmoWorld);
		D3DXMatrixTranslation(&matGizmoWorld, vEye.x, vEye.y, vEye.z);


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

			_vec3 vMoveDir = vNewEye - vEye;
			_vec3 vNewAt = vAt + vMoveDir;

			pSelectedCam->Set_Pos(vNewEye);
			

			float vPos[3], vRot[3], vScale[3];

			ImGuizmo::DecomposeMatrixToComponents((float*)matGizmoWorld, vPos, vRot, vScale);

			// 회전 변환
			_matrix matRot[ROT_END];

			D3DXMatrixRotationX(&matRot[ROT_X], D3DXToRadian(vRot[0]));
			D3DXMatrixRotationY(&matRot[ROT_Y], D3DXToRadian(vRot[1]));
			D3DXMatrixRotationZ(&matRot[ROT_Z], D3DXToRadian(vRot[2]));


			for (_uint i = 0; i < ROT_END; ++i)
			{
				D3DXVec3TransformNormal(&vNewAt, &vNewAt, &matRot[i]);
			}
			pSelectedCam->Set_At(vNewAt);
		}
	}

	ImGui::Begin("Camera Hierarchy");
	for (auto& LayerIter : m_mapCamera)
	{
		CCamera* pCamera = LayerIter.second;
		if (m_pCurCamera == pCamera)
			continue;
		if (ImGui::Selectable(to_string((_int)pCamera).c_str(), g_pSelectedCamera == pCamera))
		{
			g_pSelectedCamera = pCamera;
		}
	}
	ImGui::End();
#endif
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

void CCameraMgr::Free()
{
	for_each(m_mapCamera.begin(), m_mapCamera.end(), CDeleteMap());
	m_mapCamera.clear();
}