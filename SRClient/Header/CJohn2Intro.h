#pragma once
#include "CGameObject.h"
class CJohn2Intro :
    public CGameObject
{
private:
	explicit CJohn2Intro();
	explicit CJohn2Intro(const CJohn2Intro& rhs);
	virtual ~CJohn2Intro();


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

	_bool	m_bSound = false;

public:
	static CJohn2Intro* Create();

private:
	virtual void Free() override;
};

