#pragma once
#include "CGameObject.h"
#include "CAABB.h"
#include "CFSM.h"

class CJohnMine :
    public CGameObject
{
private:
	explicit CJohnMine(_vec3 vOrigin, _vec3 vDir);
	explicit CJohnMine(const CJohnMine& rhs);
	virtual ~CJohnMine();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	void		AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size, _bool loop = true);

private:
	HRESULT Ready_Component();
	void FSM(const _float& fTimeDelta);
	void Move(const _float& fTimeDelta);
	void StopReady(const _float& fTimeDelta);
	void Explosion(const _float& fTimeDelta);
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	enum MINESTATE
	{
		MOVE = 0,
		EXPLOSION_READY,
		EXPLOSION,
		STATE_END
	};
	MINESTATE m_eCurState = MOVE;

private:
	_vec3 m_vOrigin;
	_vec3 m_vDir;
	_float m_fZAngle = 0.f;
	_float m_fLifeTime = 0.f;
	CAABB* m_pAABB;
	wstring_view m_wsTexName;

	_float m_fFrame = 0.f;

	_float m_fChaseTime = 0.f;
	_float m_fExplosionReadyTime = 0.f;
	_float m_fExplosionTime = 0.f;


	CTransform* m_pTargetTransform = nullptr;
	CTransform* m_pReturnTargetTransform = nullptr;


public:
	static CJohnMine* Create(_vec3 vOrigin, _vec3 vDir);

private:
	virtual void Free() override;
};

