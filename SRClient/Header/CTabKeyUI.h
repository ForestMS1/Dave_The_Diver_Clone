#pragma once
#include "IObserver.h"
class CTabKeyUI :
    public IObserver
{
private:
	explicit CTabKeyUI(_float x, _float y, _float z);
	explicit CTabKeyUI(const CTabKeyUI& rhs);
	virtual ~CTabKeyUI();


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
	_vec3 m_vPos;

public:
	static CTabKeyUI* Create(_float x, _float y, _float z);

private:
	virtual void Free() override;
};

