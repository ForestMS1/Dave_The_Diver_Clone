#pragma once
#include "CGameObject.h"
class CFishHQ : public CGameObject
{
private:
	explicit CFishHQ();
	explicit CFishHQ(const CFishHQ& rhs) = delete;
	virtual ~CFishHQ();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	Engine::CTransform* m_pTransformCom;

	_uint m_iCnt;

	float m_fTimer;

public:
	static CFishHQ* Create();

private:
	virtual void Free();
};

