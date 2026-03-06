#pragma once
#include "IObserver.h"
class CSpaceKeyUI :
	public IObserver
{
private:
	explicit CSpaceKeyUI();
	explicit CSpaceKeyUI(const CSpaceKeyUI& rhs);
	virtual ~CSpaceKeyUI();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	void		OnNotify(const Event& e) override;

private:
	HRESULT Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

private:
	_bool m_bFlip = false;

public:
	static CSpaceKeyUI* Create();

private:
	virtual void Free() override;
};

