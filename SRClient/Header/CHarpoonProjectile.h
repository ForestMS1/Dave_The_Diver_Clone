#pragma once
#include "CGameObject.h"
#include "CAttackReadyArmTex.h"
#include "CAABB.h"
#include "CPlayerState.h"
#include "CLineBuffer.h"

enum class PROJECTILESTATE { READY, FIRE, HIT, RETURN, STATE_END };

class CHarpoonProjectile : public CGameObject
{
	friend class CProjectileReady;
	friend class CProjectileFire;
	friend class CProjectileHit;
	friend class CProjectileReturn;

private:
	explicit CHarpoonProjectile();
	explicit CHarpoonProjectile(const CHarpoonProjectile& rhs);
	virtual ~CHarpoonProjectile();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	void		TriggerOn() { Set_State(PROJECTILESTATE::FIRE); }
	_bool		IsTriggerOn() { return m_eState != PROJECTILESTATE::READY; }
	_vec3*		Get_Dir()	{ return &m_vDir; }
	void		Set_State(PROJECTILESTATE state);

private:
	HRESULT Ready_Component();
	void	Go_ToDir(const _float& fTimeDelta);
	void	Update_Points();

public:
	PROJECTILESTATE GetProjectilState() { return m_eState; }

private:
	CPlayerState*			m_pState = nullptr;
	PROJECTILESTATE			m_eState = PROJECTILESTATE::READY;
	unordered_map<PROJECTILESTATE, CPlayerState*> m_mapState;
	_float					m_fSpeed = 20.f;
	_float					m_fRange = 8.f;
	_float					m_fAccRange = 0.f;
	_vec3					m_vDir;

	CGameObject*			m_pCaughtFish = nullptr;
private:
	Engine::CAttackReadyArmTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CLineBuffer* m_pLineBuffer;
	CAABB* m_pAABB;

	vector<_vec3> m_vecHarpoonToProjectilePoints;

public:
	static CHarpoonProjectile* Create();

private:
	virtual void Free() override;
};

