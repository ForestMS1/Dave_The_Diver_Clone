#pragma once
#include "CGameObject.h"
class CWeightIcon :
	public CGameObject
{
private:
	explicit CWeightIcon();
	explicit CWeightIcon(const CWeightIcon& rhs);
	virtual ~CWeightIcon();


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

	_vec3 m_vPos;

public:
	static CWeightIcon* Create();

private:
	virtual void Free() override;
};

