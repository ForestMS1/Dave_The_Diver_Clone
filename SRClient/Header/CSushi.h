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
	void Free() override;
};

