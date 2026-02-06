#include "CCameraMgr.h"
#include "CCamera.h"
#include "CHelper.h"
IMPLEMENT_SINGLETON(CCameraMgr)

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
	ImGui::Begin("Camera list");
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