#pragma once
#include "CGameObject.h"
class CJohnIntro :
    public CGameObject
{
private:
	explicit CJohnIntro();
	explicit CJohnIntro(const CJohnIntro& rhs);
	virtual ~CJohnIntro();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	void		AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size, _bool loop = true);


private:
	HRESULT Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

	_float m_fFrame = 0.f;

public:
	static CJohnIntro* Create();

private:
	virtual void Free() override;
};

