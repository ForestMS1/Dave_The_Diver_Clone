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
	void		Set_Open(bool flag) { sushiOpen = flag; }

public:
	static CSushi* Create();

private:
	HRESULT			Ready_Environment_Layer(std::wstring_view svLayerTag);
	HRESULT			Ready_GameLogic_Layer(std::wstring_view svLayerTag);
	HRESULT			Ready_UI_Layer(std::wstring_view svLayerTag);

	void Key_Input();
	void Free() override;
private:
	ImGuizmo::OPERATION m_CurrentGizmoOperation;
	bool	sushiOpen;
	float   customerSpawn;
	float   gameEnd = false;

public:
	int CustomerLeave = 0;
	int   maxCustomer;

};

