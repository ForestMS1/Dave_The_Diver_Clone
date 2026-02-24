#pragma once
#include "CGameObject.h"
class CGaugeBarUI :
    public CGameObject
{
private:
	explicit CGaugeBarUI();
	explicit CGaugeBarUI(const CGaugeBarUI& rhs);
	virtual ~CGaugeBarUI();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	void		Set_Render(_bool onOff) { m_bRender = onOff; }
private:
	HRESULT Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

private:
	_bool m_bRender = false;
	_bool m_bFlip = false;

public:
	static CGaugeBarUI* Create();

private:
	virtual void Free() override;
};

