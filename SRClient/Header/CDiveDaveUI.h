#pragma once
#include "CGameObject.h"
class CDiveDaveUI :
    public CGameObject
{
private:
	explicit CDiveDaveUI();
	explicit CDiveDaveUI(const CDiveDaveUI& rhs);
	virtual ~CDiveDaveUI();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

private:
	HRESULT Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CDiveDaveUI* Create();

private:
	virtual void Free() override;
};

