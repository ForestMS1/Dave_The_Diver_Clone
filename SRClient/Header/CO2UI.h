#pragma once
#include "CGameObject.h"
class CO2UI :
    public CGameObject
{
private:
	explicit CO2UI();
	explicit CO2UI(const CO2UI& rhs);
	virtual ~CO2UI();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

private:
	HRESULT Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CO2UI* Create();

private:
	virtual void Free() override;
};

