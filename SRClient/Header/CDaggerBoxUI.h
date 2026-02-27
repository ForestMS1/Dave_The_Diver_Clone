#pragma once
#include "CGameObject.h"
class CDaggerBoxUI :
    public CGameObject
{
private:
	explicit CDaggerBoxUI();
	explicit CDaggerBoxUI(const CDaggerBoxUI& rhs);
	virtual ~CDaggerBoxUI();


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
	static CDaggerBoxUI* Create();

private:
	virtual void Free() override;
};

