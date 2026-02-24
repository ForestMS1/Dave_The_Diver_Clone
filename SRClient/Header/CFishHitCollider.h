#pragma once
#include "CGameObject.h"
#include "CAABB.h"
class CFishHitCollider : public CGameObject
{
public:
	explicit CFishHitCollider(_vec3* vPos, _vec3* vScale);
	explicit CFishHitCollider(const CFishHitCollider& rhs) = delete;
	virtual ~CFishHitCollider();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() {};

private:
	HRESULT			Ready_Component();

private:
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

	const _vec3 m_vPos;
	const _vec3 m_vScale;

public:
	static CFishHitCollider* Create(_vec3* vPos, _vec3* vScale);

public:
	void Free() override;
};

