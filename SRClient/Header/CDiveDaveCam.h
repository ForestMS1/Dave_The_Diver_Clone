#pragma once
#include "CCamera.h"
class CDiveDaveCam : public CCamera
{
private:
	explicit CDiveDaveCam();
	explicit CDiveDaveCam(const CDiveDaveCam& rhs);
	virtual ~CDiveDaveCam();

public:
	HRESULT		Ready_GameObject(const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual			_int		Update_GameObject(const _float& fTimeDelta)				override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta)			override;
	virtual			void		Render_GameObject()										override;

public:
	_vec3*		Get_vEye() { return &m_vEye; }
	_vec3*		Get_vAt() { return &m_vAt; }
	_matrix*	Get_ViewMatrix() { return &m_matView; }
	void		Set_vEye(_vec3* vEye) { memcpy(&m_vEye, vEye, sizeof(_vec3)); }
	void		Set_vAt(_vec3* vAt) { memcpy(&m_vAt, vAt, sizeof(_vec3)); }
	void		Set_Target(_vec3* pTarget) { m_pTargetPos = pTarget; }
	void		ZoomIn(const _float& fAngle) { m_fFov -= D3DXToRadian(fAngle); }
	void		ZoomOut(const _float& fAngle) { m_fFov += D3DXToRadian(fAngle); }
	void		SetFov(const _float& fAngle) { m_fFov = D3DXToRadian(fAngle); }
	_float		GetFov() { return m_fFov; }

	//Fight 상태에서 쉐이킹
	void		FightShakingStart(const _float& fDuration); //쉐이킹 시작, 외부에서 호출
	void		FightShakingUpdate(const _float& fTimeDelta);
	void		FightShakingEnd(); //쉐이킹 끝내기, 외부에서 호출

private:
	HRESULT		Add_Component();

private:
	_float			m_fLerpSpeed;
private:
	Engine::CCalculator* m_pCalculatorCom;

public:
	static CDiveDaveCam* Create(
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (_float)WINCX / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);

private:
	_vec3* m_pTargetPos = nullptr;
	_vec3   m_vOffset = { 0.f, 0.f, -10.f };

	// 흔들기용
	_float	m_fAccShakingDuration = 0.f;
	_vec3	m_vOriginEye;
	_vec3	m_vOriginAt;
	_bool   m_bShaked = false;

private:
	virtual void Free() override;
};

