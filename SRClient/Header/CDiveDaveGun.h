#pragma once
#include "CGameObject.h"
#include "CAttackReadyArmTex.h"
class CDiveDaveGun :
    public CGameObject
{
private:
	explicit CDiveDaveGun();
	explicit CDiveDaveGun(const CDiveDaveGun& rhs);
	virtual ~CDiveDaveGun();


public:
	void		Init();
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	void	Fire();

private:
	HRESULT Ready_Component();
	void	Set_ParentTransform();
	void	Rotate_ToMouse();

private:
	Engine::CAttackReadyArmTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	_bool m_bIsFlip = false;
	_bool m_bInitComplete = false;

public:
	static CDiveDaveGun* Create();

private:
	virtual void Free() override;
};

