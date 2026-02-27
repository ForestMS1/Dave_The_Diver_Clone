#pragma once
#include "CGameObject.h"
class CRKeyUI :
    public CGameObject
{
private:
	explicit CRKeyUI(_float x, _float y, _float z);
	explicit CRKeyUI(const CRKeyUI& rhs);
	virtual ~CRKeyUI();


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
	static CRKeyUI* Create(_float x, _float y, _float z);

private:
	virtual void Free() override;
};

