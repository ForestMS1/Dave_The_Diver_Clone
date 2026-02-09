#pragma once
#include "CBase.h"
#include "Engine_Define.h"

// "카메라"는 씬 Layer에 Add하지 않고 카메라 매니저에 넣는다. 
// 카메라 매니저가 씬 Update,LateUpdate 함수 안에서 직접 Update,LateUpdate를 호출

BEGIN(Engine)

class CCamera;
class CFrustrum;

class ENGINE_DLL CCameraMgr : public CBase
{
	DECLARE_SINGLETON(CCameraMgr)

private:
	explicit CCameraMgr();
	virtual ~CCameraMgr();

public:
	void	Set_Camera(wstring_view svCameraTag, Engine::CCamera* pCamera); // map에 카메라 넣어주는 함수
	void	Update_Camera(const _float& fTimeDelta);
	void	LateUpdate_Camera(const _float& fTimeDelta);
	void	Render_Camera();

	void	Change_CurCamera(wstring_view svCameraTag);

	CCamera* Get_CurCamera() { return m_pCurCamera; }

private:
	void Update_Gizmo();

private:
	//여러 카메라들을 관리할 map
	unordered_map<wstring_view, Engine::CCamera*> m_mapCamera;

	// 현재 Update, LateUpdate 돌릴 카메라
	Engine::CCamera* m_pCurCamera;

	// 디버그용 절두체
	Engine::CFrustrum* m_pFrustrumCom;
private:
	ImGuizmo::OPERATION m_CurrentGizmoOperation;
private:
	virtual void Free();

};

END

