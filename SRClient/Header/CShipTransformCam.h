#pragma once
#include "CGameObject.h"
class CShipTransformCam : public CGameObject
{
private:
	explicit CShipTransformCam();
	virtual ~CShipTransformCam();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	virtual void Update_ImGui();

private:
	HRESULT			Ready_Component();

private:
	HRESULT			Add_Component();

private:
	_vec3	m_vAt;
	_vec3	m_vUp;

private:
	Engine::CTransform* m_pTransformCom;

public:
	static CShipTransformCam* Create();

private:
	virtual void Free();
};

