#pragma once
#include "CGameObject.h"
#include "CAABB.h"

class CJohnBullet :
    public CGameObject
{
private:
	explicit CJohnBullet(_vec3 vOrigin, _vec3 vDir, _float fZAngle);
	explicit CJohnBullet(const CJohnBullet& rhs);
	virtual ~CJohnBullet();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	void		AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size, _bool loop = true);

private:
	HRESULT Ready_Component();
	void	FSM(const _float& fTimeDelta);

	void	Fire(const _float& fTimeDelta);
	void    Explosion(const _float& fTimeDelta);


private:
	enum STATE
	{
		CHASE,
		EXLPOSION,
		STATE_END
	};

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

private:
	_vec3 m_vOrigin;
	_vec3 m_vDir;
	_float m_fZAngle = 0.f;
	_float m_fLifeTime = 0.f;
	CAABB* m_pAABB;
	wstring_view m_wsTexName;

	STATE m_eCurState = STATE::CHASE;
	_float m_fFrame = 0.f;

	_float m_fChaseTime = 0.f;
	_float m_fExplosionReadyTime = 0.f;
	_float m_fExplosionTime = 0.f;


	CTransform* m_pTargetTransform = nullptr;
	CTransform* m_pReturnTargetTransform = nullptr;



	_bool  m_bInitFire = false;


public:
	static CJohnBullet* Create(_vec3 vOrigin, _vec3 vDir, _float fZAngle);

private:
	virtual void Free() override;
};

