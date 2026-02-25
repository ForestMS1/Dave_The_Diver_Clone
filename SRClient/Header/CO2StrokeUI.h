#pragma once
#include "CGameObject.h"
class CO2StrokeUI :
    public CGameObject
{
private:
	explicit CO2StrokeUI();
	explicit CO2StrokeUI(const CO2StrokeUI& rhs);
	virtual ~CO2StrokeUI();


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
	static CO2StrokeUI* Create();

private:
	virtual void Free() override;
};

