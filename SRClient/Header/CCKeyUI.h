#pragma once
#include "CGameObject.h"
class CCKeyUI :
    public CGameObject
{
private:
	explicit CCKeyUI(_float x, _float y, _float z);
	explicit CCKeyUI(const CCKeyUI& rhs);
	virtual ~CCKeyUI();


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

private:
	_vec3 m_vPos;

public:
	static CCKeyUI* Create(_float x, _float y, _float z);

private:
	virtual void Free() override;
};

