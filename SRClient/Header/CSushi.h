#pragma once
#include "CScene.h"
class CSushi : public CScene
{
private:
	explicit CSushi();
	virtual ~CSushi();

public:
	HRESULT		Ready_Scene() override;
	_int		Update_Scene(const _float& fTimeDelta) override;
	void		LateUpdate_Scene(const _float& fTimeDelta) override;
	void		Render_Scene() override;

public:
	static CSushi* Create();

private:
	HRESULT			Ready_Environment_Layer(std::wstring_view svLayerTag);
	HRESULT			Ready_GameLogic_Layer(std::wstring_view svLayerTag);
	HRESULT			Ready_UI_Layer(std::wstring_view svLayerTag);
	void Free() override;
private:
	ImGuizmo::OPERATION m_CurrentGizmoOperation;
};

