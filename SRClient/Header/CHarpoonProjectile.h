#pragma once
#include "CGameObject.h"
#include "CAttackReadyArmTex.h"
#include "CAABB.h"
class CHarpoonProjectile : public CGameObject
{
private:
	explicit CHarpoonProjectile();
	explicit CHarpoonProjectile(const CHarpoonProjectile& rhs);
	virtual ~CHarpoonProjectile();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	void		TriggerOn() { m_eState = FIRE; }
	_bool		IsTriggerOn() { return m_eState != READY; }

private:
	HRESULT Ready_Component();
	void	Set_ParentTransform();
	void	Rotate_ToMouse();
	void	Shot_ToMouse();
	void	Go_ToDir(const _float& fTimeDelta);

	void	Change_ProjectileState();

private:
	enum PROJECTILESTATE { READY, FIRE, HIT, NONE_HIT, STATE_END };

private:
	PROJECTILESTATE			m_eState = READY;
	_float					m_fSpeed = 5.f;
	_float					m_fRange = 2.f;
	_float					m_fAccRange = 0.f;
	_vec3					m_vDir;
private:
	Engine::CAttackReadyArmTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

public:
	static CHarpoonProjectile* Create();

private:
	virtual void Free() override;
};

