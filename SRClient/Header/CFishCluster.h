#pragma once
#include "CGameObject.h"
class CFishCluster :  public CGameObject
{
private:
	explicit CFishCluster();
	explicit CFishCluster(const CFishCluster& rhs) = delete;
	virtual ~CFishCluster();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	Engine::CTransform* m_pTransformCom;

public:
	static CFishCluster* Create();

private:
	virtual void Free();
};

