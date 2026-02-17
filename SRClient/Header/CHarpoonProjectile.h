#pragma once
#include "CGameObject.h"
#include "CAttackReadyArmTex.h"
#include "CAABB.h"

enum class PROJECTILESTATE { READY, FIRE, HIT, NONE_HIT, STATE_END };

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

	void		TriggerOn() { m_eState = PROJECTILESTATE::FIRE; }
	_bool		IsTriggerOn() { return m_eState != PROJECTILESTATE::READY; }

private:
	HRESULT Ready_Component();
	void	Set_ParentTransform();
	void	Rotate_ToMouse();
	void	Shot_ToMouse();
	void	Go_ToDir(const _float& fTimeDelta);

	void	Change_ProjectileState();

private:
	void FSM(const _float& fTimeDelta);
	void Ready_Act();
	void Fire_Act(const _float& fTimeDelta);
	void Hit_Act(const _float& fTimeDelta);
	void NoneHit_Act(const _float& fTimeDelta);

public:
	//enum PROJECTILESTATE { READY, FIRE, HIT, NONE_HIT, STATE_END };
	PROJECTILESTATE GetProjectilState() { return m_eState; }

private:
	PROJECTILESTATE			m_eState = PROJECTILESTATE::READY;
	_float					m_fSpeed = 10.f;
	_float					m_fRange = 5.f;
	_float					m_fAccRange = 0.f;
	_vec3					m_vDir;
private:
	Engine::CAttackReadyArmTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

private:
	_bool m_bIsHitFish = false;

public:
	static CHarpoonProjectile* Create();

private:
	virtual void Free() override;
};

