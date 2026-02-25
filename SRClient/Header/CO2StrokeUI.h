#pragma once
#include "CGameObject.h"
#include "CRcDynamicTex.h"
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
	Engine::CRcDynamicTex* m_pDynamicBufferCom;
	Engine::CTransform* m_pTransformCom;

private:
	_float m_fRatio = 0.f;
public:
	static CO2StrokeUI* Create();

private:
	virtual void Free() override;
};

