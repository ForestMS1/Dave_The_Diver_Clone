#pragma once
#include "CCamera.h"


namespace Engine
{
	class CCalculator;
}

class CPlayerCam : public CCamera
{
private:
	explicit CPlayerCam();
	explicit CPlayerCam(const CPlayerCam& rhs);
	virtual ~CPlayerCam();

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
	_vec3* Get_vEye() { return &m_vEye; }
	_vec3* Get_vAt() { return &m_vAt; }
	_matrix* Get_ViewMatrix() { return &m_matView; }
	void	Set_vEye(_vec3* vEye) { memcpy(&m_vEye, vEye, sizeof(_vec3)); }
	void	Set_vAt(_vec3* vAt) { memcpy(&m_vAt, vAt, sizeof(_vec3)); }
private:
	HRESULT		Add_Component();
	void	Key_Input();
	void	Mouse_Fix();

private:
	_float			m_fSpeed;

private:
	Engine::CCalculator* m_pCalculatorCom;

public:
	static CPlayerCam* Create(
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (_float)WINCX / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);
private:
	virtual void Free() override;
};

