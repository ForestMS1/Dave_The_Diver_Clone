#pragma once
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera();
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);

public:
	_vec3	Get_Pos()							{ return m_vEye; }
	void	Set_Pos(_vec3 vEye)					{ m_vEye = vEye; }
	_vec3	Get_At()							{ return m_vAt; }
	void	Set_At(_vec3 vAt)					{ m_vAt = vAt; }
	_vec3	Get_Up()							{ return m_vUp; }
	_matrix Get_ViewMatrix()					{ return m_matView; }
	_matrix Get_ProjMatrix()					{ return m_matProj; }

	void	Set_Acitve(bool OnOff)				{ m_bIsActive = OnOff; }

public:
	void	Update_MatView()					{ D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp); }
	void	Set_Ortho();
	void	Set_Perspective();
protected:
	_matrix		m_matView, m_matProj, m_matOrthoProj;

	_vec3		m_vEye, m_vAt, m_vUp;
	_float		m_fFov, m_fAspect, m_fNear, m_fFar;

	bool		m_bIsActive;
protected:
	virtual void	Free();
};

END

