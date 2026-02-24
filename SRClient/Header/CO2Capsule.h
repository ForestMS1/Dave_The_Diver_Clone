#pragma once
#include "CDiveItem.h"

class CO2Capsule : public CDiveItem
{
private:
	explicit CO2Capsule(_vec3 vOriginPos);
	explicit CO2Capsule(const CO2Capsule& rhs);
	virtual ~CO2Capsule();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	// CDiveItem을(를) 통해 상속됨
	void UseItem(CGameObject*) override;

private:
	HRESULT Ready_Component();

public:
	static CO2Capsule* Create(_vec3 vOriginPos);

private:
	virtual void Free() override;
};

