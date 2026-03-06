#pragma once
#include "CGameObject.h"
#include "CFSM.h"
#include "CAABB.h"
enum class JOHNBULLETSTATE
{
	CHASE = 0,
	STOP,
	RETURN,
	STATE_END
};

class CJohnGuidedBullet :
    public CGameObject
{
private:
	explicit CJohnGuidedBullet(_vec3 vOrigin, _vec3 vDir, _float fZAngle, CGameObject* pOwner);
	explicit CJohnGuidedBullet(const CJohnGuidedBullet& rhs);
	virtual ~CJohnGuidedBullet();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	void		AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size, _bool loop = true);

private:
	HRESULT Ready_Component();
	void	FSM(const _float& fTimeDelta);

	void	Chase(const _float& fTimeDelta);
	void	StopReady(const _float& fTimeDelta);
	void	Return(const _float& fTimeDelta);
	void    Explosion(const _float& fTimeDelta);


private:
	enum STATE
	{
		CHASE,
		STOP,
		EXLPOSION,
		RETURN,
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
	CJohnGuidedBullet::STATE m_eCurState = CJohnGuidedBullet::STATE::CHASE;
	_float m_fFrame = 0.f;

	_float m_fChaseTime = 0.f;
	_float m_fExplosionReadyTime = 0.f;
	_float m_fExplosionTime = 0.f;


	// 사운드를 위한 하드코딩...
	_bool m_bSoundOverload = false;
	_bool m_bSoundExplosion = false;
	_bool m_bSoundMove = false;
	_bool m_bSoundReturn = false;

	_bool m_bCompleteSetSize = false;


	CTransform* m_pTargetTransform = nullptr;
	CTransform* m_pReturnTargetTransform = nullptr;
	CGameObject* m_pReturnTarget = nullptr;


public:
	static CJohnGuidedBullet* Create(_vec3 vOrigin, _vec3 vDir, _float fZAngle, CGameObject* pOwner);

private:
	virtual void Free() override;
};

