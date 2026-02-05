#pragma once
#include "CGameObject.h"
#

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);

public:
	_vec3 Get_Pos() { return m_vEye; }
	_vec3 Get_At() { return m_vAt; }
	_vec3 Get_Up() { return m_vUp; }
	_matrix Get_ViewMatrix() { return m_matView; }
	_matrix Get_ProjMatrix() { return m_matProj; }
protected:
	_matrix		m_matView, m_matProj;

	_vec3		m_vEye, m_vAt, m_vUp;
	_float		m_fFov, m_fAspect, m_fNear, m_fFar;

protected:
	virtual void	Free();
};

END

