#pragma once
#include "CGameObject.h"
class COverloadedIcon :
	public CGameObject
{
private:
	explicit COverloadedIcon();
	explicit COverloadedIcon(const COverloadedIcon& rhs);
	virtual ~COverloadedIcon();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

private:
	void		Add_Frame(const _float& fTimeDelta, const _float& fSpeed, _uint size);

private:
	HRESULT Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

	_float m_fFrame = 0.f;
	_vec3 m_vPos;

public:
	static COverloadedIcon* Create();

private:
	virtual void Free() override;
};

