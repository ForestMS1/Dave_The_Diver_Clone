#pragma once
#include "CScene.h"
class CDive : public CScene
{
private:
	explicit CDive();
	virtual ~CDive();

public:
	HRESULT		Ready_Scene() override;
	_int		Update_Scene(const _float& fTimeDelta) override;
	void		LateUpdate_Scene(const _float& fTimeDelta) override;
	void		Render_Scene() override;

private:
	HRESULT Ready_GameLogic_Layer(std::wstring_view svLayerTag);
public:
	static CDive* Create();

private:
	void Free() override;
};

