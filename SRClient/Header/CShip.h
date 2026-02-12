#pragma once
#include "CScene.h"
class CShip : public CScene
{
private:
	explicit CShip();
	virtual ~CShip();

public:
	HRESULT		Ready_Scene() override;
	_int		Update_Scene(const _float& fTimeDelta) override;
	void		LateUpdate_Scene(const _float& fTimeDelta) override;
	void		Render_Scene() override;

public:
	static CShip* Create();

private:
	void Free() override;
};

